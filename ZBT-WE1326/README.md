Toolchain
=========
Please use [OpenWrt Toolchain 12.09](https://downloads.openwrt.org/attitude_adjustment/12.09/ramips/rt288x/).

How to compile
==============
```shell
export BUILD_TOPDIR=$(pwd)
export STAGING_DIR=$BUILD_TOPDIR/tmp

make CROSS_COMPILE=mipsel-openwrt-linux-

head -c 196608 /dev/zero | tr '\000' '\377' > `pwd | xargs basename`.bin
dd conv=notrunc if=uboot.bin of=`pwd | xargs basename`.bin
chmod +x `pwd | xargs basename`.bin
```
