#!/bin/bash
#
# Shell wrapper to set up gtest environment
#
#

. ./setenv.sh

./datetime_unittest "$@"

