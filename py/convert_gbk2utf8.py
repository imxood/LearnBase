#!/usr/bin/python
# coding:utf-8
# /home/peak/develop/git/StudyNote/test/driver
import os,sys
import chardet

def convert(filename, out_enc='utf-8'):
    try:
        print "convert " + filename
        content = open(filename).read()
        result = chardet.detect(content) # 获取当前文件的编码格式串，返回类型为字典类型
        coding = result.get('encoding')
        print "coding: " + coding
        if coding != 'UTF-8':
            print coding + " to utf-8!"
            new_content = content.decode(coding).encode(out_enc)
            open(filename, 'w').write(new_content)
            print 'done'
    except Exception as e:
        print e


def explore(dir):
    for root, dirs, files in os.walk(dir):
        for file in files:
            path = os.path.join(root, file)
            convert(path)

explore("/home/peak/develop/git/StudyNote/test/driver")
