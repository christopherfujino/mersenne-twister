#!/usr/bin/env bash

set -euo pipefail

mk clean

DEBUG_FLAGS="-g -O0" mk libmtrand.a
