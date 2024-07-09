#!/usr/bin/env python3

import argparse
import os
import subprocess


def main():
    p = argparse.ArgumentParser()
    p.add_argument("cmake_path", help="The cmake binary to use")
    p.add_argument("nextcode_src_dir", help="The nextcode source directory")
    p.add_argument("clang", help="The path to the clang binary to use")
    p.add_argument(
        "nextcode_root_dir",
        help="A path to a nextcode root produced by installing "
        "NeXTCode and Foundation together. We infer codec "
        "from here",
    )
    p.add_argument("destdir", help="The directory to perform the actual " "build in")
    p.add_argument(
        "--clean", action="store_true", help="Delete destdir before performing a build."
    )
    args = p.parse_args()

    if args.clean:
        print("Asked to clean... Cleaning!")
        subprocess.check_output(["/bin/rm", "-rfv", args.destdir])
    subprocess.check_call(["/bin/mkdir", "-p", args.destdir])
    os.chdir(args.destdir)
    configureInvocation = [
        args.cmake_path,
        "-GNinja",
        "-DNEXTCODE_EXEC={}/bin/codec".format(args.code_root_dir),
        "-DCLANG_EXEC={}".format(args.clang),
        "-DNEXTCODE_LIBRARY_PATH={}/lib/nextcode".format(args.code_root_dir),
        "{}/benchmark".format(args.code_src_dir),
    ]
    print("COMMAND: {}".format(" ".join(configureInvocation)))
    subprocess.check_call(configureInvocation)

    buildInvocation = [
        args.cmake_path,
        "--build",
        args.destdir,
        "--",
        "nextcode-benchmark-linux-x86_64",
    ]
    print("COMMAND: {}".format(" ".join(buildInvocation)))
    subprocess.check_call(buildInvocation)


if __name__ == "__main__":
    main()
