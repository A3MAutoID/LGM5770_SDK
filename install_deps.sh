#!/bin/bash -eu
sudo apt-get install -y cmake python3 python3-dev libpcre3-dev

if ! command -v swig >/dev/null; then
    echo "Installing swig"
    mkdir -p ~/Downloads
    pushd ~/Downloads > /dev/null
    wget https://downloads.sourceforge.net/project/swig/swig/swig-3.0.12/swig-3.0.12.tar.gz -O swig-3.0.12.tar.gz
    tar -xvf swig-3.0.12.tar.gz
    cd swig-3.0.12
    ./configure
    make
    sudo make install
    popd > /dev/null
fi
