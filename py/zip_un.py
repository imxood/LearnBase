#!/usr/bin/env python
# coding: utf-8

import os
import sys
import zipfile
import chardet

print("Processing File " + sys.argv[1])

file=zipfile.ZipFile(sys.argv[1],"r")

parent_dir = os.path.dirname(sys.argv[1])

for filename in file.namelist():
    result = chardet.detect(filename) # 获取当前文件的编码格式串，返回类型为字典类型
    coding = result.get('encoding')
    print("\ncurrent coding: %s" % coding)
    if coding != 'UTF-8':
        # if coding.lower().startswith("iso-8859-"):
            # utf8_filename = os.path.join(parent_dir, filename.decode(coding).encode("UTF-8"))
        utf8_filename = os.path.join(parent_dir, filename.decode(coding).encode("UTF-8"))
        print("Extracting " + utf8_filename)
        dirname = os.path.dirname(utf8_filename)
        if dirname != "" and not os.path.exists(dirname):
            os.makedirs(dirname)
        data = file.read(filename)
        if not os.path.exists(utf8_filename):
            fo = open(utf8_filename, "w")
            fo.write(data)
            fo.close
file.close()