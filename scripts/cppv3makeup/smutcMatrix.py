#!/usr/bin/env python3
import sys
import os
import argparse
import numpy as np

def main():
    """generate ldc PerspectiveMatrix randomly

    (cosΘ*k1) (-sinΘ*k) (Δx)
    (sinΘ*k)  (cosΘ*k2) (Δy)
    0         0          1

    :slicewidth: TODO
    :sliceheight:
    :output:

    """
    parser = argparse.ArgumentParser(description="generate cpp v3 ldc matrix")
    parser.add_argument("-x", "--slicewidth", type=int, dest="width",
                      required=True, help="move every point in a horizontal direction")
    parser.add_argument("-y", "--sliceheight", type=int, dest="height",
                      required=True, help="move every point in a vertical direction")
    parser.add_argument("-o", "--output", type=str, dest="output",
                      required=True, help="output file name")
    #  parser.add_argument("-t", "--translation", type=int, dest="translation",
    #                    required=True, help="move every point in a fixed direction")
    #  parser.add_argument("-r", "--rotate", type=int, dest="rotate",
    #                    required=True, help="move around an axis")
    #  parser.add_argument("-z", "--zoom", dest="zoom",
    #                    required=True, help="enlarge or decrease the size")
    #  parser.add_argument("-v", "--transvection", dest="transvection",
    #                    required=True, help="linear map in a fixed direction")

    args, unknown = parser.parse_known_args()
    print(args)

    translationCoef0 = round(np.random.uniform(-0.5, 0.5), 1)
    translationCoef1 = round(np.random.uniform(-0.5, 0.5), 1)
    print("translationCoef:", translationCoef0, translationCoef1)
    translationX = args.width * translationCoef0
    translationY = args.height * translationCoef1
    #  translationMatrix = np.array([[1, 0, translationX], [0, 1, translationY], [0, 0, 1]])

    rotateAngle = np.random.uniform(0, np.pi / 4)
    print("rotateAngle:", rotateAngle / np.pi * 180)
    #  rotateMatrix = np.array([[round(np.cos(rotateAngle), 6), round(-np.sin(rotateAngle), 6), 0], [round(np.sin(rotateAngle), 6), round(np.cos(rotateAngle), 6), 0], [0, 0, 0]])

    zoomK1 = round(np.random.uniform(0.125, 8), 6)
    zoomK2 = round(np.random.uniform(0.125, 8), 6)
    print("zoom:", zoomK1, zoomK2)
    #  zoomMatrix = np.array([[zoomK1, 0, 0], [0, zoomK2, 0], [0, 0, 0]])

    transvectionK = round(np.random.uniform(0, 10), 6)
    print("transvectionK:", transvectionK)
    #  transvectionMatix0 = np.array([[1, transvectionK, 0], [0, 1, 0], [0, 0, 0]])
    #  transvectionMatix1 = np.array([[1, 0, 0], [transvectionK, 1, 0], [0, 0, 0]])

    #  mat = translationMatrix + rotateMatrix + zoomMatrix + transvectionMatix0 + transvectionMatix1
    mat = np.array([[round(np.cos(rotateAngle) * zoomK1, 6), round(-np.sin(rotateAngle) * transvectionK, 6), translationX], \
                    [round(np.sin(rotateAngle) * transvectionK, 6), round(np.cos(rotateAngle) * zoomK2, 6), translationY], \
                    [0, 0, 1]])

    with open(args.output,'w') as f:
        print(mat)
        np.savetxt(f, mat, fmt='%.6f')
        print('-----------------------------', file=f)
        print('translationCoef:', translationCoef0, translationCoef1, file=f)
        print('rotateAngle:', rotateAngle / np.pi * 180, file=f)
        print('zoom:', zoomK1, zoomK2, file=f)
        print('transvectionK:', transvectionK, file=f)


if __name__ == "__main__":
    rc = 1
    try:
        main()
        rc = 0
    except Exception as e:
        print("Error: %s" % e, file=sys.stderr)
    sys.exit(rc)
