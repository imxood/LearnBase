#!/usr/env python
# coding: utf-8
import os, sys

# 遍历所有文件
# filelist = os.listdir(filepath)

# 可执行文件与当前路径的相对路径
# print "__file__=%s" % __file__

# 可执行程序的绝对路径
# print "os.path.realpath(__file__)=%s" % os.path.realpath(__file__)

# 可执行程序所在的目录名
# print "os.path.dirname(os.path.realpath(__file__))=%s" % os.path.dirname(os.path.realpath(__file__))

# 可执行程序所在的目录名
# print "os.path.split(os.path.realpath(__file__))=%s" % os.path.split(os.path.realpath(__file__))[0]

# 可执行程序的绝对路径
# print "os.path.abspath(__file__)=%s" % os.path.abspath(__file__)

# 当前所在的工作空间
# print "os.getcwd()=%s" % os.getcwd()

# 脚本环境
# print "sys.path[0]=%s" % sys.path[0]

# 脚本文件
# print "sys.argv[0]=%s" % sys.argv[0]

# 参数
# print "sys.argv[1]=%s" % sys.argv[1]

# print sys.getdefaultencoding()

def mod_auth(file, auth):
    if os.system("chmod %s %s" %(auth, file))==0:
        print "seted %s %s" %(file, auth)

# def dir_deal(path):
#     if os.path.isdir(path):os.listdir
#         filelist = os.listdir(path)
#         for file in filelist:
#             dir_deal(file)
#     else:
#         mod_auth(path, '664')

def dir_deal(path):
    # 目录 下面的所有目录 下面的所有文件(不含目录)
    for root, dirs, files in os.walk(path):
        for name in files:
            mod_auth(os.path.join(root, name),  '664')
        # for name in dirs:
        #     print(os.path.join(root, name))
    
if len(sys.argv) != 2 :
    print "Param error! the program need 2 params!"
    exit(0)

dir_deal(sys.argv[1])