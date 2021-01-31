echo "\033[1;32mCPython 3.8.7 Vita Port\033[0m"
./configure --build=armv7 --host=arm-dolce-eabi --prefix="$DOLCESDK/arm-dolce-eabi" --with-pydebug --enable-optimizations --without-ensurepip --disable-shared --disable-ipv6 CONFIG_SITE="config.site" CFLAGS="-Wl,-q,-z,nocopyreloc" CPPFLAGS="-Wl,-q,-z,nocopyreloc"
make clean
make -j$(nproc) install

export PYTHON_BUILD=python38-vita-build
mkdir -p $PYTHON_BUILD
# Clean the Python Installation for a Smaller Lib file
rm $PYTHON_BUILD/python38.zip
cp -as $DOLCESDK/arm-dolce-eabi/lib/python3.8 $PYTHON_BUILD
cd $PYTHON_BUILD/python3.8
rm -rf test
rm -rf lib2to3/tests
find . -type l -not -name \*.py -delete
find . -type d -empty -delete
python3 -OO -m compileall .
7z a -l ../python38.zip .
cp ../python38.zip $DOLCESDK/arm-dolce-eabi/lib
echo "Finished Installation"