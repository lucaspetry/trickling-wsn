#!/usr/bin/env python
# -*- coding: utf-8 -*-

from json import load
from sys import argv

assert len(argv) == 3, "Usage: python {} input output".format(argv[0])

with open(argv[1], 'r') as fd, open(argv[2], 'w') as out:
    for i in load(fd)['queries'][0]['results'][0]['values']:
        out.write("{}\n".format(i[1]))
