#!/usr/bin/env python3
import sys
import os
import re
import argparse
import subprocess # xxd
import traceback
import shutil

class smutcParse(object):

    """Docstring for smutcParse. """

    def __init__(self, smutcDir, cpptcDir):
        self.workDir = smutcDir
        self.targetDir = cpptcDir
        print(self.workDir, self.targetDir)
        self.smutcInDir = os.path.join(smutcDir, 'input')
        self.smutcOutDir = os.path.join(smutcDir, 'output')
        self.cpptcInDir = os.path.join(cpptcDir, 'input')
        self.cpptcOutDir = os.path.join(cpptcDir, 'output')

        if not os.path.exists(self.targetDir):
            os.makedirs(self.targetDir)
            print(self.targetDir + 'created!!!')

    def replaceWithTargetDir(self, srcPathName):
        rel = os.path.relpath(srcPathName, self.workDir)
        return os.path.join(self.targetDir, rel)

    def convert_regbuffer_txt_to_header(self, srcFileName, dstFileName):
        """convert into c language header file """
        if not os.path.isfile(srcFileName):
            print('failed to convert regbuffer,', srcFileName, 'is not exist')
            return False
        srcFile = open(srcFileName, 'r')

        if not os.path.isfile(dstFileName):
            os.makedirs(os.path.dirname(dstFileName), exist_ok=True) # starting from python3.2
        dstFile = open(dstFileName, 'w+')

        regBufferName = os.path.relpath(srcFileName, self.workDir).replace('.txt', '').replace('/', '_')
        dstFile.write("static const unsigned int "+regBufferName+"[][3] = {\n")
        for lines in srcFile:
            result = re.sub(r'([0-9a-fA-F]{8}) ([0-9a-fA-F]{8}) ([0-9a-fA-F]{8})', r'{0x\1, 0x\2, 0x\3},', lines)
            dstFile.write(result)
        dstFile.write("};\n")

        print(srcFileName, '=>', dstFileName)
        return True

    def generate_regbuffer_header_files(self):
        targetTxt = ['RegBuffer.txt', 'InitRegBuffer.txt']
        for dirpath, dirnames, filenames in os.walk(self.workDir):
            for filename in [f for f in filenames if f in targetTxt]:
                srcFileName = os.path.join(dirpath, filename)
                dstFileName = self.replaceWithTargetDir(srcFileName).replace('txt', 'h')
                self.convert_regbuffer_txt_to_header(srcFileName, dstFileName)

    def convert_TotalRegBuffer_txt_to_header(self, srcFileName, dstFileName):
        """convert into c language header file """
        if not os.path.isfile(srcFileName):
            print('failed to convert regbuffer,', srcFileName, 'is not exist')
            return False
        srcFile = open(srcFileName, 'r')

        if not os.path.isfile(dstFileName):
            os.makedirs(os.path.dirname(dstFileName), exist_ok=True) # starting from python3.2
        dstFile = open(dstFileName, 'w+')

        regBufferName = os.path.relpath(srcFileName, self.workDir).replace('.txt', '').replace('/', '_')
        dstFile.write("static const unsigned int "+regBufferName+"[] = {\n")
        for lines in srcFile:
            result = re.sub(r'([0-9a-fA-F]{8})', r'0x\1,', lines)
            dstFile.write(result)
        dstFile.write("};\n")

        print(srcFileName, '=>', dstFileName)
        return True

    def generate_cmdlist_header_files(self):
        targetTxt = ['TotalRegBuffer.txt']
        for dirpath, dirnames, filenames in os.walk(self.workDir):
            for filename in [f for f in filenames if f in targetTxt]:
                srcFileName = os.path.join(dirpath, filename)
                dstFileName = self.replaceWithTargetDir(srcFileName).replace('txt', 'h')
                self.convert_TotalRegBuffer_txt_to_header(srcFileName, dstFileName)

    def static_const_header_files(self, f):
        """TODO: try to improve speed"""
        f.seek(0) # return to the begining
        fileLines = f.readlines()
        f.seek(0) # return to the begining
        for line in fileLines:
            result = re.sub(r'^unsigned', r'static const unsigned', line)
            f.write(result)

    def convert_imagedata_raw_to_header(self, srcFileName, dstFileName):
        if not os.path.isfile(srcFileName):
            print('failed to convert regbuffer,', srcFileName, 'is not exist')
            return False

        if not os.path.isfile(dstFileName):
            os.makedirs(os.path.dirname(dstFileName), exist_ok=True) # starting from python3.2

        dstFile = open(dstFileName, 'w+') # both read and write
        try:
            wd = os.getcwd()
            os.chdir(self.workDir)
            rc = subprocess.check_call(['xxd', '-i', os.path.relpath(srcFileName, self.workDir)], stdout=dstFile)
            os.chdir(wd)
        except subprocess.CalledProcessError as exc:
            print(exc.cmd, 'failed')
            return False

        self.static_const_header_files(dstFile)
        print(srcFileName, '=>', dstFileName)
        return True

    def generate_imagedata_header_files(self):
        # input headers
        for dirpath, dirnames, filenames in os.walk(self.smutcInDir):
            for filename in [f for f in filenames if f.endswith(('.nv12', '.p010', '.raw'))]:
                srcFileName = filename
                dstFileName = filename.replace('.', '_') + '.txt'
                srcFilePath = os.path.join(dirpath, srcFileName)
                dstFilePath = self.replaceWithTargetDir(os.path.join(dirpath, dstFileName))
                self.convert_imagedata_raw_to_header(srcFilePath, dstFilePath)
        # output headers
        for dirpath, dirnames, filenames in os.walk(self.smutcOutDir):
            for filename in [f for f in filenames if f.endswith(('.nv12', '.p010', '.raw')) and not f.startswith(('PPS', 'NPS'))]:
                srcFileName = filename
                dstFileName = filename.replace('.', '_') + '.txt'
                srcFilePath = os.path.join(dirpath, srcFileName)
                dstFilePath = self.replaceWithTargetDir(os.path.join(dirpath, dstFileName))
                self.convert_imagedata_raw_to_header(srcFilePath, dstFilePath)

    def create_test_source_file(self):
        if not os.path.exists(self.targetDir):
            os.makedirs(self.targetDir)

        sourceFileName = os.path.basename(self.targetDir) + '.c'
        sourceFilePath = os.path.join(self.targetDir, sourceFileName)
        """create source file if not exist"""
        if not os.path.exists(sourceFilePath):
            f = open(sourceFilePath, 'w')
        else:
            print(sourceFilePath, 'exists')

    def include_RegBuffer_headers(self, dirPath):
        strBuf = ''
        RegBufferHeader = ['RegBuffer.h', 'InitRegBuffer.h', 'TotalRegBuffer.h']
        # sorted dirpath for clarity
        for dirpath, dirnames, filenames in sorted(os.walk(dirPath)):
            for filename in [f for f in filenames if f in RegBufferHeader]:
                rel = os.path.relpath(dirpath, self.targetDir)
                linestr = "#include \"" + os.path.join(rel, filename) + "\"\n"
                strBuf += linestr
        return strBuf

    def include_ImgBuffer_headers(self, dirPath):
        strBuf = ''
        imgBufHeaderPattern = ('_raw.txt', '_nv12.txt', '_p010.txt')
        for dirpath, dirnames, filenames in sorted(os.walk(dirPath)):
            for filename in [f for f in filenames if f.endswith(imgBufHeaderPattern)]:
                rel = os.path.relpath(dirpath, self.targetDir)
                linestr = "#include \"" + os.path.join(rel, filename) + "\"\n"
                strBuf += linestr
        return strBuf

    def format_cpptcRegsBuf_t(self, blockList):
        strBuf = ''
        for frameNum, layerNum, tileNum, sliceNum in blockList:
            strBuf +="static cpptcRegsBuf_t f%sl%st%ss%sRegBufArr[] = {\n"%(frameNum, layerNum, tileNum, sliceNum)
            strBuf += "\tARRAYED_ITEM(output_%s_layer%s_tile%s_slice%s_InitRegBuffer),\n"%(frameNum, layerNum, tileNum, sliceNum)
            strBuf += "\tARRAYED_ITEM(output_%s_layer%s_tile%s_slice%s_RegBuffer),\n"%(frameNum, layerNum, tileNum, sliceNum)
            strBuf += "};\n\n"
        return strBuf

    def format_cpptcBlockRegs_t(self, blockList):
        strBuf = ''
        for frame in sorted({item[0] for item in blockList}):
            for layer in sorted({item[1] for item in blockList}):
                strBuf += "static cpptcBlockRegs_t f%sl%sBlockArr[] = {\n"%(frame, layer)
                for frameNum, layerNum, tileNum, sliceNum in [b for b in blockList if b[1] == layer]:
                    strBuf += "\tARRAYED_ITEM(f%sl%st%ss%sRegBufArr),\n"%(frameNum, layerNum, tileNum, sliceNum)
                strBuf += "};\n\n"

        return strBuf

    def format_cpptcLayerRegs_t(self, blockList):
        strBuf = ''

        for frame in sorted({item[0] for item in blockList}):
            strBuf += "static cpptcLayerRegs_t f%sLayerArr[] = {\n"%(frame)
            for layer in sorted({item[1] for item in blockList}):
                strBuf += "\t[%s] = ARRAYED_ITEM(f%sl%sBlockArr),\n"%(layer, frame, layer)
            strBuf += "};\n\n"

        return strBuf

    def format_cpptcGoldenBuf_t(self, blockList):
        strBuf = ''
        goldenDict = {
            "CPP30GOLD_RCSC_REC" : "L",
            "CPP30GOLD_WCSC_3DNR" : "Wcsc0_L",
            "CPP30GOLD_WCSC_SCL0" : "Wcsc1_L",
            "CPP30GOLD_WCSC_SCL1" : "Wcsc2_L",
            "CPP30GOLD_KGAIN_OUT" : "K_L",
            "CPP30GOLD_LDC0_PROFILE" : "Ldc0Layer",
            "CPP30GOLD_LDC1_PROFILE" : "Ldc1Layer",
        }
        for frame in sorted({item[0] for item in blockList}):
            for layer in sorted({item[1] for item in blockList}):
                strBuf += "static cpptcGoldenBuf_t f%sl%sGoldenBufArr[CPP30GOLD_MAX] = {\n"%(frame, layer)
                strBuf += "\t/* input */\n"
                if os.path.isdir(os.path.join(self.cpptcInDir, frame)): # check if input frame directory exist
                    for filename in [f for f in os.listdir(os.path.join(self.cpptcInDir, frame)) if f.endswith('.txt')]:
                        if filename.startswith('L'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_RCSC_REC, input_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                strBuf += "\t/* output */\n"
                if os.path.isdir(os.path.join(self.cpptcOutDir, frame)): # check if output frame directory exist
                    for filename in [f for f in os.listdir(os.path.join(self.cpptcOutDir, frame)) if f.endswith('.txt')]:
                        if filename.startswith('Wcsc0_L'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_WCSC_3DNR, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                        if filename.startswith('Wcsc1_L'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_WCSC_SCL0, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                        if filename.startswith('Wcsc2_L'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_WCSC_SCL1, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                        if filename.startswith('K_L'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_KGAIN_OUT, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                        if filename.startswith('Ldc0Layer'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_LDC0_PROFILE, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                        if filename.startswith('Ldc1Layer'+layer):
                            strBuf += "\tINDEXED_ITEM(CPP30GOLD_LDC1_PROFILE, output_%s_%s),\n"%(frame, filename.replace('.txt', ''))
                strBuf += "};\n\n"

        return strBuf

    def format_cpptcLayerGolden_t(self, blockList):
        strBuf = ''

        for frame in sorted({item[0] for item in blockList}):
            strBuf += "static cpptcLayerGolden_t f%sLayerGoldenArr[] = {\n"%(frame)
            for layer in sorted({item[1] for item in blockList}):
                strBuf += "\t[%s] = ARRAYED_ITEM(f%sl%sGoldenBufArr),\n"%(layer, frame, layer)
            strBuf += "};\n\n"

        return strBuf

    def atoi(self, text):
        return int(text) if text.isdigit() else text

    def natural_keys(self, text):
        return [ self.atoi(c) for c in re.split(r'(\d+)', text) ]

    def get_frame_layer_tile_slice_list(self, frameNum, frmDirPath):
        blockList = []
        frmNumPath = os.path.join(frmDirPath, frameNum)
        for blockName in sorted(os.listdir(frmNumPath)):
            if os.path.isdir(os.path.join(frmNumPath, blockName)):
                match = re.search('layer(\d+)_tile(\d+)_slice(\d+)', blockName)
                layerNum = match.group(1)
                tileNum = match.group(2)
                sliceNum = match.group(3)
                blockList.append((frameNum, layerNum, tileNum, sliceNum))
        return sorted(blockList, key=lambda row: (int(row[0]),int(row[1]),int(row[2]),int(row[3])))

    def create_frame_header_file(self, frmNum):
        headerFileName = 'f' + frmNum + '.h'
        headerFilePath = os.path.join(self.targetDir, headerFileName)
        inputFrmNumPath = os.path.join(self.cpptcInDir, frmNum)
        outputFrmNumPath = os.path.join(self.cpptcOutDir, frmNum)
        strBuf = ''

        """create header file if not exist"""
        if not os.path.exists(headerFilePath):
            headerFile = open(headerFilePath, 'w')
        else:
            print(headerFilePath, 'exists')
            headerFile = open(headerFilePath, 'w')

        blockList = self.get_frame_layer_tile_slice_list(frmNum, self.cpptcOutDir)

        strBuf += '#ifndef __' + headerFileName.replace('.', '_').upper() + '__\n'
        strBuf += '#define __' + headerFileName.replace('.', '_').upper() + '__\n'
        strBuf += '\n'
        strBuf += self.include_RegBuffer_headers(outputFrmNumPath)
        strBuf += self.include_ImgBuffer_headers(inputFrmNumPath)
        strBuf += self.include_ImgBuffer_headers(outputFrmNumPath)
        strBuf += '\n'
        strBuf += self.format_cpptcRegsBuf_t(blockList)
        strBuf += self.format_cpptcBlockRegs_t(blockList)
        strBuf += self.format_cpptcLayerRegs_t(blockList)
        strBuf += self.format_cpptcGoldenBuf_t(blockList)
        strBuf += self.format_cpptcLayerGolden_t(blockList)
        strBuf += '#endif\n'

        headerFile.write(strBuf)
        print(headerFilePath, 'created!!!')
        return True

    def create_test_header_file(self):
        for framePath in sorted(os.listdir(self.cpptcOutDir)):
            self.create_frame_header_file(framePath)

    def run(self):
        if not os.path.isdir(self.smutcInDir):
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), self.smutcInDir)
        if not os.path.isdir(self.smutcOutDir):
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), self.smutcOutDir)
        print(self.smutcInDir, self.smutcOutDir)

        if os.path.isdir(self.cpptcOutDir):
            try:
                shutil.rmtree(self.cpptcOutDir)
            except OSError as e:
                print("Error: %s - %s." % (e.filename, e.strerror))
                return -1

        try:
            self.generate_regbuffer_header_files()
            self.generate_cmdlist_header_files()
            self.generate_imagedata_header_files()
            #  self.create_test_source_file()
            self.create_test_header_file()
        except Exception as e:
            print(traceback.format_exc())
            return False

        return True

    def run_without_golden(self):
        if not os.path.isdir(self.smutcInDir):
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), self.smutcInDir)
        if not os.path.isdir(self.smutcOutDir):
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), self.smutcOutDir)
        print(self.smutcInDir, self.smutcOutDir)

        try:
            self.generate_regbuffer_header_files()
            self.generate_cmdlist_header_files()
            self.create_test_header_file()
        except Exception as e:
            print(traceback.format_exc())
            return False

        return True


def main():
    parser = argparse.ArgumentParser(description="make up cppv3 test case")
    parser.add_argument("-s", "--source", dest="src",
                      required=True, help="source directory")
    parser.add_argument("-d", "--destination", dest="dst",
                      required=True, help="destination directory")
    parser.add_argument("-n", "--nogolden", dest="nogolden",
                      action='store_true', help="not to parse golden")
    args, unknown = parser.parse_known_args()

    smutcParseObj = smutcParse(args.src, args.dst)
    if args.nogolden == True:
        smutcParseObj.run_without_golden()
    else:
        smutcParseObj.run()

if __name__ == "__main__":
    rc = 1
    try:
        main()
        rc = 0
    except Exception as e:
        print("Error: %s" % e, file=sys.stderr)
    sys.exit(rc)
