#!/bin/bash
#
# Shell wrapper to set up gtest environment
#
#

. ./setenv.sh

./angle_unittest "$@"

