#!bin/sh
echo -e "\033[1;32mCompiling Python Modules\033[0m"
if [ -z "$1" ]
    then
        export PREFIX=$VITASDK/arm-vita-eabi
    else
        export PREFIX=$1
fi
export PYTHON_BUILD=python27-vita-build
mkdir -p $PYTHON_BUILD
# Clean the Python Installation for a Smaller Lib file
rm $PYTHON_BUILD/python27.zip
cp -as $PREFIX/lib/python2.7 $PYTHON_BUILD
cd $PYTHON_BUILD/python2.7
rm -rf test
rm -rf lib2to3/tests
find . -type l -not -name \*.py -delete
find . -type d -empty -delete
python2 -OO -m compileall .
find . -type l -not -name \*.pyc -delete
7z a -l ../python27.zip .
cp ../python27.zip $PREFIX/lib