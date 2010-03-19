#!/usr/bin/python
# -*- coding: utf-8 -*-

# This script reads a prp file (passed as first argument) and prints the names of all scene objects.
# Alternatively, you can specifiy the type to be listed as second argument.
# by Diafero

import PyHSPlasma, sys

if len(sys.argv) == 1:
    print "Usage: %s filename [typename]" % sys.argv[0]
    sys.exit(0)

rm = PyHSPlasma.plResManager()
page = rm.ReadPage(sys.argv[1])
if len(sys.argv) <= 2: # default to scene objects
    type = PyHSPlasma.plFactory.kSceneObject
else:
    type = PyHSPlasma.plFactory.ClassIndex(sys.argv[2])
    if type < 0:
        print "Type '%s' is invalid" % sys.argv[2]
        sys.exit(1)
keys = rm.getKeys(page.location, type)

for key in keys:
    print key.name