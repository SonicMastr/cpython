#!/bin/sh
echo -e "\033[1;32mCPython 2.7.18 Vita Port\033[0m"
if [ -z "$1" ]
    then
        export PREFIX=$VITASDK/arm-vita-eabi
    else
        export PREFIX=$1/$VITASDK/arm-vita-eabi
fi

./configure --build=armv7 --host=arm-vita-eabi --prefix="$PREFIX" --enable-optimizations --disable-shared --with-threads --disable-ipv6 --without-signal-module CONFIG_SITE="config.site" CFLAGS="-Wl,-q,-z,nocopyreloc" CPPFLAGS="-Wl,-q,-z,nocopyreloc" LDFLAGS="-Wl,--allow-multiple-definition"
make clean
make -j$(nproc) install