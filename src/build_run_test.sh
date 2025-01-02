#!/bin/bash

# Convenience script for building, running and testing

# Invoke as ./build_run_test.sh to run the tests
# and ./build_run_test.sh run_main to run the main function (which happens to
# be a test case too)

build_dir="$(git rev-parse --show-toplevel)/build"
mkdir -p "$build_dir"
cd "$build_dir"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && make -j8 && ./main "$@"
