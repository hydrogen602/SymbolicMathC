#!/usr/bin/env python3

import sys
import re

if len(sys.argv) != 2:
    print(f'Usage: {sys.argv[0]} file')
    sys.exit(1)

cmd, file = sys.argv

with open(file) as f:

    x = [re.sub(r'\s*{\s*', ';', s) for s in f.readlines() if re.match(r'\w', s) and ';' not in s]


    print('\n'.join(x))