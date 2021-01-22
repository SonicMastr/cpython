echo -e "\033[1;32mCPython 2.7.18 Vita Port\033[0m"
./configure --build=armv7 --host=arm-dolce-eabi --prefix="$DOLCESDK/arm-dolce-eabi" --enable-optimizations --disable-shared --with-threads --disable-ipv6 --without-signal-module CONFIG_SITE="config.site" CFLAGS="-Wl,-q,-z,nocopyreloc" CPPFLAGS="-Wl,-q,-z,nocopyreloc"
make clean
make -j$(nproc) install

export PYTHON_BUILD=python27-vita-build
mkdir -p $PYTHON_BUILD
# Clean the Python Installation for a Smaller Lib file
rm $PYTHON_BUILD/python27.zip
cp -as $DOLCESDK/arm-dolce-eabi/lib/python2.7 $PYTHON_BUILD
cd $PYTHON_BUILD/python2.7
rm -rf test
rm -rf lib2to3/tests
find . -type l -not -name \*.py -delete
find . -type d -empty -delete
python2 -OO -m compileall .
find . -type l -not -name \*.pyc -delete
7z a -l ../python27.zip .
cp ../python27.zip $DOLCESDK/arm-dolce-eabi/lib
echo "Finished Installation"