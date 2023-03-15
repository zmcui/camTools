#!/usr/bin/env python3
import sys
import os
import subprocess
import shlex
import argparse

class dwtv3(object):
    def settingFileReplceLastLine(self, fileName, newLine):
        #print(settingFileReplceLastLine.__name__, newLine)
        file = open(fileName, "r")
        lines = file.readlines()[:-1]
        lines.append(newLine)
        file.close
        file = open(fileName, "w")
        file.writelines(lines)

    def gen(self, imgPath, imgFmt, imgW, imgH, outputDir):
        dwtv3SmuExe = "simulator/dwtv3_simulator_20220926/Simulator_DWTv3"
        dwtv3SmuPath = os.path.join(os.getcwd(), dwtv3SmuExe)
        dwtv3ScalerSetting = "simulator/dwtv3_simulator_20220926/CSimpleScalingFilter_P0.c"
        dwtv3FormatSetting = "simulator/dwtv3_simulator_20220926/CFORMATERFilter_P0.c"
        dwtv3SmuOut = "extyuv_LL1.raw extyuv_LL2.raw extyuv_LL3.raw extyuv_LL4.raw y_LL1.raw y_LL2.raw y_LL3.raw y_LL4.raw uv_LL1.raw uv_LL2.raw uv_LL3.raw uv_LL4.raw"
        cmdDwtIndex = 0
        cmdDwtFormat = int(imgFmt == "p010")

        """replace register size in CSimpleScalingFilter_P0.c"""
        regvalSize = imgW + (imgH << 16)
        self.settingFileReplceLastLine(dwtv3ScalerSetting, format(regvalSize, '08x'))
        cmd_list = shlex.split(dwtv3SmuPath + " " + imgPath + " " + dwtv3ScalerSetting + " " + dwtv3FormatSetting + " " + dwtv3SmuOut + " " + str(cmdDwtIndex) + " " + str(cmdDwtFormat))
        proc = subprocess.check_call(cmd_list)
        print(cmd_list, 'success!!!')

        if proc == 0:
            os.rename('extyuv_LL1.raw', os.path.join(outputDir, 'L1.raw'))
            os.rename('extyuv_LL2.raw', os.path.join(outputDir, 'L2.raw'))
            os.rename('extyuv_LL3.raw', os.path.join(outputDir, 'L3.raw'))
            os.rename('extyuv_LL4.raw', os.path.join(outputDir, 'L4.raw'))
            os.remove('y_LL1.raw')
            os.remove('y_LL2.raw')
            os.remove('y_LL3.raw')
            os.remove('y_LL4.raw')
            os.remove('uv_LL1.raw')
            os.remove('uv_LL2.raw')
            os.remove('uv_LL3.raw')
            os.remove('uv_LL4.raw')

        return True

def main():
    parser = argparse.ArgumentParser(description="generate dwt layer from image file v3")
    parser.add_argument("-f", "--format", dest="fmt",
                      choices=["nv12", "p010"],
                      required=True, help="input binrary image format")
    parser.add_argument("-w", "--width", type=int, dest="imgw",
                      required=True, help="input binary image width")
    parser.add_argument("-g", "--height", type=int, dest="imgh",
                      required=True, help="input binary image height")
    parser.add_argument("-i", "--input", dest="input",
                      required=True, help="input binary image file path")
    parser.add_argument("-o", "--output", dest="output",
                      required=True, help="output dwt directory path")

    args, unknown = parser.parse_known_args()
    print(args)

    dwtv3Obj = dwtv3()
    dwtv3Obj.gen(args.input, args.fmt, args.imgw, args.imgh, args.output)

if __name__ == "__main__":
    rc = 1
    try:
        main()
        rc = 0
    except Exception as e:
        print("Error: %s" % e, file=sys.stderr)
    sys.exit(rc)
