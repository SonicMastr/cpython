#!/bin/sh
echo -e "\033[1;32mDownloading Additional Modules into $1\033[0m"
if [ -z "$1" ]
    then
        export PREFIX=$VITASDK/arm-vita-eabi
    else
        export PREFIX=$1/$VITASDK/arm-vita-eabi
fi

export INSTALLDIR=$PREFIX/lib/python2.7

get_install_latest_release() {
    echo -e "\033[1;32mInstalling $1/$2\033[0m" 
    wget -q -O tmp.zip $(curl -s https://api.github.com/repos/$1/$2/releases/latest | grep 'browser_' | cut -d\" -f4) && unzip -o tmp.zip -d $INSTALLDIR && rm tmp.zip
}

get_install_latest_release SonicMastr youtube-dl
