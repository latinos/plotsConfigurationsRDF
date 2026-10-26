#!/usr/bin/env python3
"""
Scale the MC histograms of a mkShapes file.

It does not use UPDATE: it reads the file (copied locally) and writes a NEW file in
tmpdir with the scaled MC histograms, then it overwrites the original one.
Writing from scratch is linear; overwriting in UPDATE is quadratic in the number
of keys (each delete updates the list of free segments).

Example:
  python3 scale_mc.py ../2024/rootFile/WW_260923_test24/mkShapes__WW_260923_test24.root 2.5
"""
import argparse
import logging
import os
import shutil
import subprocess
import sys
import time

import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.TH1.AddDirectory(False)   # no objects attached to the directories in memory

MARKER = "mc_rescaled_factor"

parser = argparse.ArgumentParser()
parser.add_argument("--file", help="input root file (also on /eos)")
parser.add_argument("--factor", type=float, help="scale factor for the MC")
parser.add_argument("--tmpdir", default="/tmp/abulla")
parser.add_argument("--skip", nargs="+", default=["DATA", "Fake"],
                    help="samples NOT to be scaled (exact match or prefix + '_')")
parser.add_argument("--keep-backup", action="store_true",
                    help="keep in tmpdir the original not scaled copy")
parser.add_argument("--xrdcp", action="store_true",
                    help="copy with xrdcp instead of via mount (often much faster on EOS)")
parser.add_argument("--log-every", type=int, default=2000,
                    help="print the progress every N histograms")
parser.add_argument("--dry-run", action="store_true",
                    help="do everything locally but do NOT overwrite the original")
args = parser.parse_args()

logging.basicConfig(level=logging.INFO, format="%(asctime)s  %(message)s", datefmt="%H:%M:%S")
log = logging.getLogger()


def human(n):
    for unit in ["B", "KB", "MB", "GB"]:
        if n < 1024:
            return f"{n:.1f} {unit}"
        n /= 1024
    return f"{n:.1f} TB"


def to_xrootd(path):
    # /eos/user/a/abulla/... -> root://eosuser.cern.ch//eos/user/a/abulla/...
    if path.startswith("/eos/user"):
        return "root://eosuser.cern.ch/" + path
    if path.startswith("/eos/cms"):
        return "root://eoscms.cern.ch/" + path
    return path


def copy(a, b, label):
    t0 = time.time()
    if args.xrdcp and (a.startswith("/eos") or b.startswith("/eos")):
        subprocess.run(["xrdcp", "-f", "-s", to_xrootd(a), to_xrootd(b)], check=True)
    else:
        shutil.copyfile(a, b)
    dt = time.time() - t0
    size = os.path.getsize(b) if os.path.exists(b) else os.path.getsize(a)
    log.info(f"{label}: {human(size)} in {dt:.1f} s ({human(size / max(dt, 1e-6))}/s)")


src = os.path.abspath(args.file)
os.makedirs(args.tmpdir, exist_ok=True)
base = os.path.basename(src)
local_in = os.path.join(args.tmpdir, base + ".orig")
local_out = os.path.join(args.tmpdir, base)

T0 = time.time()

# 1) local copy of the original
log.info(f"[1/4] Copying locally: {src}")
copy(src, local_in, "      copy EOS -> tmp")

# 2) write a new file with the scaled MC
fin = ROOT.TFile.Open(local_in, "READ")
if not fin or fin.IsZombie():
    sys.exit(f"Cannot open {local_in}")
if fin.Get(MARKER):
    prev = fin.Get(MARKER).GetTitle()
    fin.Close()
    sys.exit(f"ERROR: file already scaled (factor {prev}). Nothing done.")

fout = ROOT.TFile.Open(local_out, "RECREATE")
fout.SetCompressionSettings(fin.GetCompressionSettings())

stats = {"scaled": 0, "skipped": 0, "other": 0, "dirs": 0}
skipped_prefixes = set()
t_scan = time.time()
t_last = t_scan


def is_skipped(sample):
    return any(sample == s or sample.startswith(s + "_") for s in args.skip)


def progress():
    global t_last
    n = stats["scaled"] + stats["skipped"]
    if n and n % args.log_every == 0:
        now = time.time()
        rate = args.log_every / max(now - t_last, 1e-6)
        log.info(f"      {n} histograms processed  ({rate:.0f} histo/s, "
                 f"{stats['dirs']} directory, {now - t_scan:.1f} s)")
        t_last = now


def process(din, dout):
    stats["dirs"] += 1
    seen = set()
    # static list of the keys: no changes to the list during the iteration
    keys = list(din.GetListOfKeys())
    for key in keys:
        name = key.GetName()
        if name in seen:          # keep only the highest cycle (the first in the list)
            continue
        seen.add(name)

        cls = ROOT.TClass.GetClass(key.GetClassName())

        if cls.InheritsFrom("TDirectory"):
            sub_in = din.Get(name)
            sub_out = dout.mkdir(name)
            process(sub_in, sub_out)
            continue

        obj = key.ReadObj()

        if cls.InheritsFrom("TH1") and name.startswith("histo_"):
            sample = name[len("histo_"):]
            if is_skipped(sample):
                stats["skipped"] += 1
                skipped_prefixes.add(sample.split("_")[0])
            else:
                obj.Scale(args.factor)
                stats["scaled"] += 1
            dout.WriteTObject(obj, name)
            progress()
        elif cls.InheritsFrom("TTree"):
            dout.cd()
            obj.CloneTree(-1, "fast").Write(name)
            stats["other"] += 1
        else:
            dout.WriteTObject(obj, name)
            stats["other"] += 1


log.info(f"[2/4] Scaling the MC by a factor {args.factor} and writing {local_out}")
process(fin, fout)
fout.cd()
ROOT.TNamed(MARKER, str(args.factor)).Write()
fout.Close()
fin.Close()

log.info(f"      scaled: {stats['scaled']}, skipped: {stats['skipped']}, "
         f"other objects: {stats['other']}, directories: {stats['dirs']}")
log.info(f"      skipped prefixes: {sorted(skipped_prefixes)}")
log.info(f"      scan completed in {time.time() - t_scan:.1f} s "
         f"(in: {human(os.path.getsize(local_in))}, out: {human(os.path.getsize(local_out))})")

# 3) overwrite the original
if args.dry_run:
    log.info("[3/4] --dry-run: the original is NOT overwritten. Output in " + local_out)
else:
    log.info(f"[3/4] Overwriting {src}")
    copy(local_out, src, "      copy tmp -> EOS")
    if os.path.getsize(local_out) != os.path.getsize(src):
        sys.exit("ERROR: size of the original different from the local one!")
    os.remove(local_out)

# 4) cleanup
if not args.keep_backup and not args.dry_run:
    os.remove(local_in)
else:
    log.info(f"      original not scaled file kept in {local_in}")

log.info(f"[4/4] Done in {time.time() - T0:.1f} s in total.")