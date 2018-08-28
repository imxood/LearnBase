#!/usr/bin/python
# coding: utf-8

import os, shutil, errno

# webrtc的源码目录
webrtc_source = "/home/mx/develop/sources/webrtc-checkout/src"

# 编译输出目录
webrtc_out = webrtc_source + "/out/Debug1"

# audio_processing的源码目录
audio_processing_sources = webrtc_source + "/modules/audio_processing"

# 安装webrtc_audio_processing库
install_prefix="/usr/local"

include_dir = install_prefix + "/include/webrtc"
lib_dir = install_prefix + "/lib"
webrtc_pkg_file = install_prefix + "/lib/pkgconfig/webrtc.pc"
audio_process_pkg_file = install_prefix + "/lib/pkgconfig/audio_processing.pc"
audio_processing_unittest_pkg_file = install_prefix + "/lib/pkgconfig/audio_processing_unittest.pc"
rtc_base_generic_pkg_file = install_prefix + "/lib/pkgconfig/rtc_base_generic.pc"

# 复制头文件
for root,dirs,files in os.walk(webrtc_source): # 完全路径, 目录名, 文件列表
    for name in files:
        #print name
        if name.endswith(".h"):
            # print root,dirs,name
            src = root + "/" + name
            dest = include_dir + "/" + os.path.relpath(root, start = webrtc_source)
            if os.path.exists(dest + "/" + name):
                continue
            print "src:%s,dest:%s:" % (src,dest)
            if not os.path.isdir(dest):
                os.makedirs(dest)
            shutil.copy(src, dest)


pkg_config_dir='/usr/local/lib/pkgconfig'
if not os.path.exists(pkg_config_dir):
    os.makedirs(pkg_config_dir)

# 复制webrtc库文件
webrtc_lib_file = webrtc_out + "/obj/libwebrtc.a"
shutil.copy(webrtc_lib_file, install_prefix + "/lib")
print "src:%s,dest:%s:" % (webrtc_lib_file, install_prefix + "/lib")

# 生成pkgconfig/webrtc.pc文件
content = ""
content += "libdir=/usr/local/lib\n"
content += "includedir=/usr/local/include\n"
content += "\n"
content += "Name: audio_processing\n"
content += "Description: Webrtc audio process module\n"
content += "Version: 0.1\n"
content += "Libs: -L${libdir} -lwebrtc\n"
content += "Cflags: -I${includedir}/webrtc -DWEBRTC_POSIX\n"

f = open(webrtc_pkg_file, "w")
f.write(content)
f.close()

# 复制audio_process库文件
audio_processing_lib_file = webrtc_out + "/obj/modules/audio_processing/libaudio_processing.a"
shutil.copy(audio_processing_lib_file, install_prefix + "/lib")
print "src:%s,dest:%s:" % (audio_processing_lib_file, install_prefix + "/lib")

# 生成pkgconfig/audio_process.pc文件
content = ""
content += "libdir=/usr/local/lib\n"
content += "includedir=/usr/local/include\n"
content += "\n"
content += "Name: audio_processing\n"
content += "Description: Webrtc audio process module\n"
content += "Version: 0.1\n"
content += "Libs: -L${libdir} -laudio_processing\n"
content += "Cflags: -I${includedir}/webrtc -DWEBRTC_POSIX\n"

f = open(audio_process_pkg_file, "w")
f.write(content)
f.close()

# 复制audio_processing_unittest库文件
audio_processing_unittest_lib_file = webrtc_out + "/obj/modules/audio_processing/libaudioproc_unittest_proto.a"
shutil.copy(audio_processing_unittest_lib_file, install_prefix + "/lib")
print "src:%s,dest:%s:" % (audio_processing_unittest_lib_file, install_prefix + "/lib")

# 生成pkgconfig/audio_processing_unittest.pc文件
content = ""
content += "libdir=/usr/local/lib\n"
content += "includedir=/usr/local/include\n"
content += "\n"
content += "Name: audio_processing_unittest\n"
content += "Description: Webrtc audio process module\n"
content += "Version: 0.1\n"
content += "Libs: -L${libdir} -laudioproc_unittest_proto\n"
content += "Cflags: -I${includedir}/webrtc -DWEBRTC_POSIX\n"

f = open(audio_processing_unittest_pkg_file, "w")
f.write(content)
f.close()


# 复制rtc_base_generic库文件
rtc_base_generic_lib_file = webrtc_out + "/obj/rtc_base/librtc_base_generic.a"
shutil.copy(rtc_base_generic_lib_file, install_prefix + "/lib")
print "src:%s,dest:%s:" % (rtc_base_generic_lib_file, install_prefix + "/lib")

# 生成pkgconfig/rtc_base_generic.pc文件
content = ""
content += "libdir=/usr/local/lib\n"
content += "includedir=/usr/local/include\n"
content += "\n"
content += "Name: rtc_base_generic\n"
content += "Description: Webrtc audio process module\n"
content += "Version: 0.1\n"
content += "Libs: -L${libdir} -lrtc_base_generic\n"
content += "Cflags: -I${includedir}/webrtc -DWEBRTC_POSIX\n"

f = open(rtc_base_generic_pkg_file, "w")
f.write(content)
f.close()

print "Execute successfully!"