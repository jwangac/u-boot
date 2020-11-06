Introduction
============

This repository contains the U-Boot source code for several routers. Pre-built
binaries are provided too.

The source code were taken and modified from the GPL archives released by
Netgear and Ubiquiti Networks.

Devices
=======
| Model | Booting mainstream OpenWrt | Recovery using serial  | Recovery using reset button |
|:--- | :--- | :--- | :--- |
| [HiWiFi HC5661A](HiWiFi-HC5661A/HiWiFi-HC5661A.bin?raw=true) | Yes | Yes | No |
| [MikroTik RB750Gr3](MikroTik-RB750Gr3/MikroTik-RB750Gr3.bin?raw=true) | No<sup>1</sup> | Yes | Yes |
| [Netgear WNDR3800](Netgear-WNDR3800/Netgear-WNDR3800.bin?raw=true) | Yes | Yes | Yes |
| [Phicomm K2P](Phicomm-K2P/Phicomm-K2P.bin?raw=true) | Yes<sup>2</sup> | Yes | Yes |
| [Xzwifi CreativeBox](Xzwifi-CreativeBox/Xzwifi-CreativeBox.bin?raw=true) | Yes | Yes, but untested | Yes |
| [YouHua WR1200JS](YouHua-WR1200JS/YouHua-WR1200JS.bin?raw=true) | Yes | Yes, but untested | Yes |
| [ZBT WE1326](ZBT-WE1326/ZBT-WE1326.bin?raw=true) | Yes | Yes | No |
| [ZTE Q7](ZTE-Q7/ZTE-Q7.bin?raw=true) | Yes | Yes | No |

1. u-boot for MikroTik RB750Gr3 will not work after [this commit](https://github.com/openwrt/openwrt/commit/52f2d7d2a9dc3e142bb957462a30806eb5cebaf4). Please use the official RouterBoot.
2. u-boot for Phicomm K2P works with the current stable release (OpenWrt 19.07.4) but not snapshot builds. It is because snapshot builds uses a large LZMA dictionary size which is not supported by this u-boot.
One workaround is to change "KERNEL_DTB += -d21" to "KERNEL_DTB += -d10" in "target/linux/ramips/image/mt7621.mk" when building OpenWrt.
