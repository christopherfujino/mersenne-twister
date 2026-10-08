#!/usr/bin/env bash

set -euo pipefail

mk clean

DEBUG_FLAGS=" " mk libmtrand.a
