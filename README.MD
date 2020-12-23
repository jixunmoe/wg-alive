## WireGuard keep-alive

A cli tool to keep client alive and accessible from other nodes.

## Cross-compile for arm

Install packages

```sh
# as root, or use sudo:
# for armel (compatible on armhf/aarch64 system)
apt install -y gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi

# for armhf
apt install -y gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf

# ... if you want aarch64
apt install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
```

Compile:

```
CC=gcc make all
CC=arm-linux-gnueabi-gcc make all
CC=aarch64-linux-gnu-gcc make all
```
