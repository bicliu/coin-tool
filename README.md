Coins tool
==================
Coins:
        Ulord;


UlordChain:

git clone tools and UlordChain

cd coin-tool

mkdir build

cd build

cmake -DCHAIN_TYPE=ULORD -DCHAIN_SRC_ROOT=/root/UlordChain -DJOBS=24 ..

make
