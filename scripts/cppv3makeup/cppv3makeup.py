#!/usr/bin/env python3
import os
import sys
import argparse
import traceback

from smutcGen import *
from smutcParse import *
from dwtv3Gen import *

def main():
    parser = argparse.ArgumentParser(description="make up cppv3 test case")
    parser.add_argument("-s", "--script", dest="scr",
                      required=True, help="bash script")
    parser.add_argument("-d", "--destination", dest="dst",
                      required=True, help="destination directory")
    args, unknown = parser.parse_known_args()

    print(args)
    scriptPath = args.scr
    if not os.path.exists(scriptPath):
        print(scriptPath, 'missed!!!')
        return -1
    srcDir = os.path.dirname(scriptPath)

    try:
        cppv3smuObj = cppv3smu()
        cppv3smuObj.parse_cmds(scriptPath)
    except Exception as e:
        #  print(e)
        print(traceback.format_exc())
        sys.exit("cppv3smuObj create failed")

    dwtv3Obj = dwtv3()
    inputDir = os.path.join(srcDir, 'input')
    for dirpath, dirnames, filenames in os.walk(inputDir):
        for filename in [f for f in filenames if f.endswith(('.nv12'))]:
            imagePath = os.path.join(dirpath, filename)
            dwtv3Obj.gen(imagePath, 'nv12', cppv3smuObj.inputWidth, cppv3smuObj.inputHeight, dirpath)
        for filename in [f for f in filenames if f.endswith(('.p010'))]:
            imagePath = os.path.join(dirpath, filename)
            dwtv3Obj.gen(imagePath, 'p010', cppv3smuObj.inputWidth, cppv3smuObj.inputHeight, dirpath)

    exitCode = cppv3smuObj.run()
    if exitCode:
        return exitCode

    if not os.path.exists(args.dst):
        os.makedirs(args.dst)
    smutcParseObj = smutcParse(srcDir, args.dst)
    smutcParseObj.run()

if __name__ == "__main__":
    rc = 1
    try:
        main()
        rc = 0
    except Exception as e:
        print("Error: %s" % e, file=sys.stderr)
    sys.exit(rc)
