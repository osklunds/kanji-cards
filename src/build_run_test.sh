#!/bin/bash

# Convenience script for building, running and testing

# Invoke as ./build_run_test.sh to run the tests
# and ./build_run_test.sh run_main to run the main function (which happens to
# be a test case too)

cd "$(git rev-parse --show-toplevel)/build"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && make -j8 && ./main "$@"
