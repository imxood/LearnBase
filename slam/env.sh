#!/bin/bash
# coding: utf-8

### 安装pcl库
sudo yum search flann-devel.x86_64
git clone https://github.com/PointCloudLibrary/pcl.git
cd pcl
mkdir out && cd out && cmake ..
make -j4 && sudo make install