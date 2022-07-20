#!/usr/bin/env python3

#
# Test program for Operating Systemen, Opdracht 3.
#
# Copyright (C) 2022  Leiden University, The Netherlands.
#

import os
import sys
import hashlib  # for MD5
from pathlib import Path
from argparse import ArgumentParser

from typing import List, Callable

checksums = {
    "small.txt": "a4195c1cd88942fdcbc747029db5fb5a",
    "large.txt": "acbec5cf4d59cecdf22ef5129beec07f",
    "file1.txt": "6b8d91211b247b4eef395dc8789ed52d",
    "file2.txt": "7320de03916e08e86693ad75ef8800dc",
    "file3.txt": "9fdb85c793cf43095c11acff998b9a2f",
    "file4.txt": "9fdb85c793cf43095c11acff998b9a2f",
    "file5.txt": "93b47be308f91225e4716ac09f839609",
}


def read_file_default(filename : Path) -> str:
    '''Read file and compute MD5 checksum.'''
    contents = filename.read_bytes()

    m = hashlib.md5()
    m.update(contents)

    return m.hexdigest()

def read_file_partial(filename : Path, start : int, count : int) -> str:
    '''Read @count bytes from file starting at @start, compute MD5
    checksum of these bytes.'''
    m = hashlib.md5()

    fd = os.open(filename, os.O_DIRECT)
    os.lseek(fd, start, os.SEEK_SET)

    b = os.read(fd, count)
    m.update(b)

    os.close(fd)

    return m.hexdigest()

def read_file_blocksize(filename : Path, blocksize : int) -> str:
    '''Read file using a block size of @blocksize and compute MD5
    checksum.'''
    m = hashlib.md5()

    fd = os.open(filename, os.O_DIRECT)

    b = os.read(fd, blocksize)
    while b:
        m.update(b)
        b = os.read(fd, blocksize)

    os.close(fd)

    return m.hexdigest()

def read_file_different_blocksizes(filename : Path, blocksizes : List[int]) -> str:
    '''Read file using varying block sizes by cycling through @blocksizes
    and compute MD5 checksum.'''
    i = 0
    m = hashlib.md5()

    fd = os.open(filename, os.O_DIRECT)

    b = os.read(fd, blocksizes[i])
    i = (i + 1) % len(blocksizes)
    while b:
        m.update(b)
        b = os.read(fd, blocksizes[i])
        i = (i + 1) % len(blocksizes)

    os.close(fd)

    return m.hexdigest()


def run_tests(mp : Path, descr : str, func : Callable[[Path], str]) -> None:
    for filename in sorted(checksums.keys()):
        if func(mp / filename) != checksums[filename]:
            print("{}: test {} failed.".format(filename, descr))

def run_tests_partial_reads(mp : Path) -> None:
    md5 = read_file_partial(mp / "large.txt", 400, 256)
    if md5 != "05a7a37999df55409ea9aafe6e621284":
        print("{}: partial read test 1 failed.".format("large.txt"))

    md5 = read_file_partial(mp / "large.txt", 564, 2356)
    if md5 != "c07f8fa023db9aee1ad2600d14bb15c7":
        print("{}: partial read test 2 failed.".format("large.txt"))

    md5 = read_file_partial(mp / "large.txt", 16345, 3502)
    if md5 != "2d51acc59036443d687cdaba6e77ae4f":
        print("{}: partial read test 3 failed.".format("large.txt"))


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("mountpoint", type=str,
                        help="Mountpoint of EdFS file system")

    args = parser.parse_args()

    mp = Path(args.mountpoint)
    if not mp.exists() or not mp.is_dir():
        print("error: {}: does not exist or is not a directory.".format(mp), file=sys.stderr)
        exit(-1)

    run_tests(mp, "read with standard block size", read_file_default)
    run_tests(mp, "read with block size 256",
              lambda f: read_file_blocksize(f, 256))
    run_tests(mp, "read with block size 512",
              lambda f: read_file_blocksize(f, 512))
    run_tests(mp, "read with block size 223",
              lambda f: read_file_blocksize(f, 223))
    run_tests(mp, "read with different block sizes",
              lambda f: read_file_different_blocksizes(f, [564, 12, 226, 16, 54, 90]))
    run_tests_partial_reads(mp)

    print("done.")
