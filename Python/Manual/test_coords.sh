#!/usr/bin/env bash

. ./setenv.sh

python test_angle.py "$@"
python test_Cartesian.py "$@"

# python test_spherical.py "$@"

