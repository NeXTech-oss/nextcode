#!/usr/bin/env python3

import argparse
import os
import shutil
import subprocess


def perform_build(args, nextcodebuild_path, config, binary_name, opt_flag):
    assert config in ["debug", "release"]
    assert binary_name in ["Benchmark_O", "Benchmark_Osize", "Benchmark_Onone"]
    assert opt_flag in ["-O", "-Osize", "-Onone"]

    inner_build_dir = os.path.join(args.build_path, binary_name)
    nextcodebuild_args = [
        nextcodebuild_path,
        "--package-path",
        args.package_path,
        "--scratch-path",
        inner_build_dir,
        "--configuration",
        config,
        "-Xnextcodec",
        "-Xllvm",
        "-Xnextcodec",
        "-align-module-to-page-size",
        "-Xnextcodec",
        opt_flag,
    ]
    if args.verbose:
        nextcodebuild_args.append("--verbose")
    subprocess.call(nextcodebuild_args)

    # Copy the benchmark file into the final ./bin directory.
    binpath = os.path.join(inner_build_dir, config, "NeXTCodeBench")
    finalpath = os.path.join(args.build_path, "bin", binary_name)
    shutil.copy(binpath, finalpath)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--verbose", "-v", action="store_true")
    parser.add_argument("--package-path", type=str, required=True)
    parser.add_argument("--build-path", type=str, required=True)
    parser.add_argument("--toolchain", type=str, required=True)

    args = parser.parse_args()

    # Create our bin directory so we can copy in the binaries.
    bin_dir = os.path.join(args.build_path, "bin")
    if not os.path.isdir(bin_dir):
        os.makedirs(bin_dir)

    nextcodebuild_path = os.path.join(args.toolchain, "usr", "bin", "nextcode-build")
    perform_build(args, nextcodebuild_path, "debug", "Benchmark_Onone", "-Onone")
    perform_build(args, nextcodebuild_path, "release", "Benchmark_Osize", "-Osize")
    perform_build(args, nextcodebuild_path, "release", "Benchmark_O", "-O")


if __name__ == "__main__":
    main()
