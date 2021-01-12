#!/bin/bash
find . -executable -name rel_cli -exec {} "$@" \;
