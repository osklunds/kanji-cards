#!/bin/bash

# Copyright (C) 2025 Oskar Lundström

# This file is part of kanji-cards.

# kanji-cards is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.

# kanji-cards is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with
# kanji-cards. If not, see <https://www.gnu.org/licenses/>.


# Convenience script for building, running and testing

# Invoke as ./build_run_test.sh to run the tests
# and ./build_run_test.sh run_main to run the main function (which happens to
# be a test case too)

build_dir="$(git rev-parse --show-toplevel)/build"
mkdir -p "$build_dir"
cd "$build_dir"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && make -j8 && ./main "$@"
