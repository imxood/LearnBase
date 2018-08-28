##### init set
cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
##### install
apt-get update
apt-get -y upgrade
apt-get install -y gcc tmux vim git python-dev python-pip libxml2-dev libxslt1-dev libmysqld-dev
pip install lxml beautifulsoup4 shadowsocks mysql-python pysocks netifaces
pip install --upgrade requests
##### dirs
cd /root
##### ssh
#    pem
wget http://example.com/skey.txt
mv skey.txt id_rsa_1024_aliyun.pem
chmod 0400 id_rsa_1024_aliyun.pem
#    config
wget http://example.com/sshconfig.txt
cat sshconfig.txt > ~/.ssh/config
rm sshconfig.txt
#    known_hosts
wget http://example.com/knownhost.txt
cat knownhost.txt > ~/.ssh/known_hosts
rm knownhost.txt
##### ss
wget http://example.com/sslocal.txt
mv sslocal.txt sslocal.json
sslocal -c sslocal.json &
##### git
mkdir app
cd app
git clone ssh://aliyun/~/app/crawler
cd crawler
cp dat/cookie_tmpl.dat dat/cookie.dat
##### work
python regHost.py reg
python crawler.py 1
python regHost.py unreg