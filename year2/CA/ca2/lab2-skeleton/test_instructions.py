#!/usr/bin/env python3

# test_instructions.py
#
# Run all unit tests in the tests/ directory and provide usable output.
# The output is inspired by pytest.
#
# Copyright (C) 2020  Leiden University, The Netherlands
#

import os
import sys
from pathlib import Path
import subprocess

from argparse import ArgumentParser
try:
    from colorama import init, Fore, Style
    enable_color = True
except ImportError:
    enable_color = False

# Returns posix on POSIX platform, nt on NT/Windows
def posix_nt(posix, nt):
    return posix if os.name == "posix" else nt


# Wrapper functions for optional color output
def bright(s):
    if enable_color:
        s = Style.BRIGHT + s + Style.RESET_ALL
    return s

def passed(s):
    if enable_color:
        s = Fore.GREEN + s + Style.RESET_ALL
    return s

def failed(s):
    if enable_color:
        s = Fore.RED + s + Style.RESET_ALL
    return s


if enable_color:
    init(autoreset=True)

# Need emulator available
RV64_EMU = Path(posix_nt("rv64-emu", "Windows\\rv64-emu.exe"))
if not RV64_EMU.exists():
    print("rv64-emu{} executable not available, compile it first.".format(posix_nt('', '.exe')), file=sys.stderr)
    exit(1)
RV64_EMU = RV64_EMU.resolve()

# Parse arguments
parser = ArgumentParser()
parser.add_argument("-v", dest="verbose", action="store_true",
                    help="Enable verbose output")
parser.add_argument("-f", dest="fail", action="store_true",
                    help="Stop on first failure")
parser.add_argument("-p", dest="pipeline", action="store_true",
                    help="Enable pipelining on emulator")
parser.add_argument("testfile", type=str, nargs="?",
                    help="Optional path to single test (.conf file) to run")
args = parser.parse_args()

# Determine which unit tests to run
if args.testfile:
    if not Path(args.testfile).exists():
        print("Test {} does not exist".format(args.testfile), file=sys.stderr)
        exit(1)

    all_tests = [args.testfile]
else:
    all_tests = list(Path("./tests").glob("*.conf"))
    all_tests.sort()

# Initialize stats
collected = len(all_tests)
tests_pass = 0
tests_fail = 0
fail_log = ""

print(bright("collected {} tests".format(len(all_tests))))
print()

# Run the tests
if args.pipeline:
    cmd = [str(RV64_EMU), '-p', '-t']
else:
    cmd = [str(RV64_EMU), '-t']

for test in all_tests:
    try:
        result = subprocess.run(cmd + [str(test)], stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=3)
    except subprocess.TimeoutExpired:
        result = None

    if result and result.returncode == 0:
        tests_pass += 1
        if not args.verbose:
            print(passed("."), end='')
        else:
            print(passed("PASS"), test)
    else:
        tests_fail += 1
        if not args.verbose:
            print(failed("F"), end='')
            fail_log += failed("FAIL ") + str(test) + "\n"
            if result:
                fail_log += result.stderr.decode() + "\n"
        else:
            print(failed("FAIL"), test)
            if result:
                print(result.stderr.decode())

        # Stop after first failure, if requested
        if args.fail:
            break

print()

# Output fail log if necessary
if not args.verbose:
    print()
    print(fail_log)

# Output final banner
banner = " {} tests, {} pass, {} fail ".format(collected, tests_pass, tests_fail)
banner = "=" * 20 + banner + "=" * 20
if tests_fail == 0:
    print(passed(banner))
    status = 0
else:
    print(failed(banner))
    status = 1

exit(status)
