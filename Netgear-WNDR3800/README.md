Toolchain
=========
Please use [OpenWrt Toolchain 10.03.1](https://downloads.openwrt.org/backfire/10.03.1/ar71xx/).

How to compile
==============
```shell
export BUILD_TOPDIR=$(pwd)
export STAGING_DIR=$BUILD_TOPDIR/tmp

make distclean
make hw29763654p16p128_config
make CROSS_COMPILE=mips-openwrt-linux-

head -c 327680 /dev/zero | tr '\000' '\377' > `pwd | xargs basename`.bin
dd conv=notrunc if=u-boot.bin of=`pwd | xargs basename`.bin
chmod +x `pwd | xargs basename`.bin
```
