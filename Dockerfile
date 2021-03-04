FROM ubuntu:18.04 as builder

RUN apt-get update && apt-get install -y python build-essential zlib1g-dev xz-utils clang curl wget

# Install latest cmake
ADD https://github.com/Kitware/CMake/releases/download/v3.18.0-rc3/cmake-3.18.0-rc3-Linux-x86_64.sh /cmake-3.18.0-rc3-Linux-x86_64.sh 
RUN mkdir /opt/cmake
RUN sh /cmake-3.18.0-rc3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN cmake --version

# Install boost 1.72.0
ADD https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz boost_1_72_0.tar.gz
RUN tar xfz boost_1_72_0.tar.gz     && rm boost_1_72_0.tar.gz     && cd boost_1_72_0     && ./bootstrap.sh     && ./b2 --without-python --prefix=/usr -j 4 link=static runtime-link=static install     && cd .. && rm -rf boost_1_72_0/ && ldconfig

RUN mkdir -p /mcidle-cpp/mcidle

COPY get-ip.py /mcidle-cpp-/get-ip.py
COPY tests /mcidle-cpp/tests
COPY json /mcidle-cpp/json
COPY zlib /mcidle-cpp/zlib
COPY mcidle /mcidle-cpp/mcidle
COPY CMakeLists.txt /mcidle-cpp/

WORKDIR /mcidle-cpp

# Build libssl1.1.1j statically
ADD https://ftp.openssl.org/source/openssl-1.1.1j.tar.gz openssl-1.1.1j.tar.gz
RUN tar -xvf openssl-1.1.1j.tar.gz && cd openssl-1.1.1j && ./config -static && make && make install && cd .. 

# Build libcurl7.47.0 statically
RUN openssl version
ADD https://curl.se/download/curl-7.47.0.tar.gz curl-7.47.0.tar.gz
RUN tar -xvf curl-7.47.0.tar.gz && cd curl-7.47.0 && ./configure --disable-shared --enable-static --prefix=/tmp/curl --disable-sspi --disable-ftp --disable-file --disable-dict --disable-telnet --disable-tftp --disable-rtsp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-smb && make && make install && cd ..

# Generate makefiles and build
RUN cmake . && make

FROM ubuntu:18.04

WORKDIR /

COPY --from=builder /mcidle-cpp/bin/mcidle /mcidle

CMD ["/mcidle"]
