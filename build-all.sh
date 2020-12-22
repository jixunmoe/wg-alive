mkdir -p bin

make clean
CC=gcc make all
mv wg-alive bin/wg-alive.amd64

make clean
CC=arm-linux-gnueabi-gcc make all
mv wg-alive bin/wg-alive.armhf
