# install cmake3.18.0
wget https://github.com/Kitware/CMake/releases/download/v3.18.0-rc3/cmake-3.18.0-rc3-Linux-x86_64.sh
mkdir -p /opt/cmake
chmod +x ./cmake-3.18.0-rc3-Linux-x86_64.sh 
./cmake-3.18.0-rc3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
cmake --version

# install boost 1.72.0
wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz 
tar xfz boost_1_72_0.tar.gz     && rm boost_1_72_0.tar.gz     && cd boost_1_72_0     && ./bootstrap.sh     && ./b2 --without-python --prefix=/usr -j 4 link=static runtime-link=static install 1> /dev/null && cd .. && rm -rf boost_1_72_0/ && ldconfig

# install openssl1.1.1b
wget https://www.openssl.org/source/old/1.1.1/openssl-1.1.1b.tar.gz
tar -xvf openssl-1.1.1b.tar.gz && cd openssl-1.1.1b && ./config -static --prefix=/usr/local/ssl && make && make install 1> /dev/null && cd ..

wget https://curl.se/download/curl-7.61.0.tar.gz
tar -xvf curl-7.61.0.tar.gz && cd curl-7.61.0 && ./configure \
--disable-shared --enable-static --prefix=/usr/curl --disable-sspi --disable-ftp --disable-file --disable-dict \ 
--disable-telnet --disable-tftp --disable-rtsp --disable-pop3 --disable-imap --disable-smtp --disable-gopher \
--disable-smb --with-ssl && make && make install && cd ..

