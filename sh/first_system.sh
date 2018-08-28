# update system
sudo apt update && sudo apt upgrade -y


# install chinese language package
sudo apt install -y language-pack-zh-hans


# set language environment
sudo bash -c 'echo "LC_ALL=zh_CN.UTF-8" >> /etc/default/locale'


# install common software
sudo apt install -y vim-gnome git subversion axel


# install mwget: a install tool
wget http://jaist.dl.sourceforge.net/project/kmphpfm/mwget/0.1/mwget_0.1.0.orig.tar.bz2
tar -xjvf mwget_0.1.0.orig.tar.bz2
cd mwget_0.1.0.orig
sudo apt install -y intltool libssl-dev
./configure
make
sudo make install


# install vscode
# ...


# config anaconda source
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes

# install shadowsocks
# 

# install ros
# http://wiki.ros.org/ROS/Installation/UbuntuMirrors
# 
sudo sh -c '. /etc/lsb-release && echo "deb http://mirrors.ustc.edu.cn/ros/ubuntu/ $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt-get update
# sudo apt-get install ros-kinetic-desktop-full
