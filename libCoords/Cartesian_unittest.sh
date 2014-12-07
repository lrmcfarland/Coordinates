#!/usr/bin/env bash
#
# Shell wrapper to set up gtest environment
#
# TODO: add linux support
#

. ./setenv.sh

./Cartesian_unittest "$@"

