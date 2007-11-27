#!/usr/bin/python
import sys
import os
import os.path
import explorer
import stridgen

def process (res):
    result = []
    for line in res:
	line = line.strip()
	if len(line) == 0:
            continue
        mapline = stridgen.makeid(line)
        if mapline is None:
            toadd = "# NO MATCH FOR %s" % line
        else:
            toadd = "%s: %s" % (mapline, line)
        if toadd not in result:
            result.append(toadd)
    result.sort()
    return result

def get_resources(d):
    result = []
    packages = [explorer.read_list(x) for x in explorer.get_lists(d)]
    for package in packages:
        for line in package:
            if line[1] not in result:
                result.append(line[1])
    return result

if __name__ == "__main__":
    if len(sys.argv) < 2:
        d = os.path.join(os.path.pardir, os.path.pardir, "sc2", "content")
    else:
        d = sys.argv[1]
    resources = get_resources(d)
    resmap = process(resources)
    for r in resmap:
        print r

    