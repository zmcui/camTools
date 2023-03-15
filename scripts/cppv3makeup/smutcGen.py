#!/usr/bin/env python3
import os
import sys
import argparse
import shlex
import subprocess # cppv3_simulator_ipe
import traceback
import shutil

from dwtv3Gen import *

class cppv3smu(object):
    def parse_cmds(self, scriptPath):
        print(scriptPath)

        parser = argparse.ArgumentParser(description="cppv3_simulator_ipe")
        parser.add_argument('exe')
        parser.add_argument('-rcsc0', nargs='+', help='file number is from stoplevel to startlevel')
        parser.add_argument('-rcsc0fullwidth', help='Rcsc0 input Full/Stride Width')
        parser.add_argument('-rcsc0fullheight', help='Rcsc0 input Full/Stride Height')
        parser.add_argument('-rcsc2', nargs='+', help='file number is from stoplevel to startlevel')
        parser.add_argument('-rcsc2fullwidth', help='Rcsc2 input Full/Stride Width')
        parser.add_argument('-rcsc2fullheight', help='Rcsc2 input Full/Stride Height')
        parser.add_argument('-wcsc0', nargs='+', help='file number is from stoplevel to startlevel')
        parser.add_argument('-workmode', help='0:MCTF or 1:MFNR')
        parser.add_argument('-startlevel', help='[0~4] for multi pass start layer level')
        parser.add_argument('-stoplevel', help='[0~4] for multi pass stop layer level')
        parser.add_argument('-zoom', help='[256 ~ 4096] if zoom is Enable, 1X = 256')
        parser.add_argument('-framenum', help='[0 ~ 2047] if MFNR Mode, Current Frame Number')
        parser.add_argument('-frametotalnum', help='[0 ~ 2047] if MFNR Mode, Frame Total Number')
        #  parser.add_argument('-test', required=True, help='to make trouble')
        self.parser = parser
        self.workDir = os.path.dirname(scriptPath) or '.'
        self.cmdlines = []

        file = open(scriptPath, 'r')
        Lines = file.readlines()
        for n, line in enumerate(Lines):
            if line.startswith('#') or not line.rstrip():
                Lines[n] = ''
            else:
                Lines[n] = line.replace('\\\n', '')
        Lines = ' '.join(Lines).splitlines()
        print('zmc:', Lines)
        for line in Lines:
            shellCmd = shlex.split(line)
            args, unknown = self.parser.parse_known_args(shellCmd)
            self.cmdlines.append(line)
        self.inputWidth = int(args.rcsc0fullwidth)
        self.inputHeight = int(args.rcsc0fullheight)
        #  print(self.cmdlines)
        #  print(self.workDir)

    def run(self):
        if len(self.cmdlines) == 0:
            print("cppv3smu has no cmdlines")
            return 0

        if os.path.isdir(os.path.join(self.workDir, 'output')):
            try:
                shutil.rmtree(os.path.join(self.workDir, 'output'))
            except OSError as e:
                print("Error: %s - %s." % (e.filename, e.strerror))
                return -1

        for line in self.cmdlines:
            shellCmd = shlex.split(line)
            args, unknown = self.parser.parse_known_args(shellCmd)
            print('####frame', args.framenum, '####')
            p = subprocess.Popen(shellCmd, cwd=self.workDir)
            exitCode = p.wait()
            #  if exitCode:
                #  print('exit code is', exitCode)
                #  return exitCode

        return 0

    def get_revision(self):
        cwd = os.getcwd()
        os.chdir(self.workDir)
        rc = subprocess.run(self.args.exe, capture_output=True)
        os.chdir(cwd)

        for item in rc.stdout.decode().split('\n'):
            if 'SVN-Revision' in item:
                print(item)

        return rc.stdout.decode()

def main():
    parser = argparse.ArgumentParser(description="simulation generate from cmd.sh")
    parser.add_argument("-s", "--script", dest="scr",
                      required=True, help="bash script")
    args, unknown = parser.parse_known_args()

    print(args)
    scriptPath = args.scr
    if not os.path.exists(scriptPath):
        print(scriptPath, 'missed!!!')
        return -1

    try:
        cppv3smuObj = cppv3smu()
        cppv3smuObj.parse_cmds(scriptPath)
    except Exception as e:
        print(traceback.format_exc())
        sys.exit("cppv3smuObj create failed")

    dwtv3Obj = dwtv3()
    inputDir = os.path.join(os.path.dirname(scriptPath), 'input')
    for dirpath, dirnames, filenames in os.walk(inputDir):
        for filename in [f for f in filenames if f.endswith(('.nv12'))]:
            imagePath = os.path.join(dirpath, filename)
            dwtv3Obj.gen(imagePath, 'nv12', cppv3smuObj.inputWidth, cppv3smuObj.inputHeight, dirpath)
        for filename in [f for f in filenames if f.endswith(('.p010'))]:
            imagePath = os.path.join(dirpath, filename)
            dwtv3Obj.gen(imagePath, 'p010', cppv3smuObj.inputWidth, cppv3smuObj.inputHeight, dirpath)

    cppv3smuObj.run()

if __name__ == "__main__":
    rc = 1
    try:
        main()
        rc = 0
    except Exception as e:
        print("Error: %s" % e, file=sys.stderr)
    sys.exit(rc)
