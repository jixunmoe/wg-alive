#!/bin/sh

set -ex

rm -rf bin

mkdir -p bin obj

# build <arch> <cc>
build()
{
  export ARCH="$1"
  export CC="$2"

  command -v "$CC" || return

  make clean
  make all
  cp "wg-alive" "bin/wg-alive.$1"
}

build "$(uname -m)" gcc
build "arm" arm-linux-gnueabi-gcc
build "aarch64" aarch64-linux-gnu-gcc
