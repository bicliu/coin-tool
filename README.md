Coins tool
==================

UlordChain
-------------
### Building

```bash
git clone https://github.com/UlordChain/UlordChain.git
cd UlordChain
./autogen.sh
./configure
make

git clone https://github.com/bicliu/coin-tool.git
cd coin-tool
mkdir build
cd build
cmake -DCHAIN_TYPE=ULORD -DCHAIN_SRC_ROOT=/root/UlordChain -DJOBS=24 ..
make
```

### Configure

```bash
cd
mkdir .cointools
```

Add a configure file "tool.conf", and add "testnet=0" for main net or "testnet=1" for test net

```bash
sudo apt-get install libboost-all-dev
```
