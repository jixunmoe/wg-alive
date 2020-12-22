## WireGuard keep-alive

A cli tool to keep client alive and accessible from other nodes.

## Cross-compile for arm

Install packages

```sh
# as root, or use sudo:
apt install -y gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi
```

Compile:

```
CC=arm-linux-gnueabi-gcc make all
```
