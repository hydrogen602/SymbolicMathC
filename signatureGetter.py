#!/usr/bin/env python3

import sys
import re

if len(sys.argv) != 2:
    print(f'Usage: {sys.argv[0]} file')
    sys.exit(1)

cmd, file = sys.argv

def findAndPrint(regex, lines):
    tmp = [re.sub(r'\s*{\s*', ';', s) for s in lines if re.fullmatch(regex, s)]
    print('\n'.join(tmp))

with open(file) as f:
    lines = f.readlines()
    regex = r'\w+(\s+\w+)*(\s*[*]+)?\s+(__\w+)\s*\([^\)]*\)\s*(\{\s*)?'

    findAndPrint(regex, lines)

    print()
    regex = r'\w+(\s+\w+)*(\s*[*]+)?\s+([^_]\w+)\s*\([^\)]*\)\s*(\{\s*)?'

    findAndPrint(regex, lines)

