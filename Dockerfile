FROM debian:stable-slim as builder

RUN apt-get update && apt-get install -y python build-essential zlib1g-dev xz-utils clang

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

WORKDIR /mcidle-cpp

# Build libssl1.1.1b statically
ADD https://www.openssl.org/source/old/1.1.1/openssl-1.1.1b.tar.gz openssl-1.1.1b.tar.gz
RUN tar -xvf openssl-1.1.1b.tar.gz && cd openssl-1.1.1b && ./config -static --prefix=/usr/local/ssl \
&& make && make install && cd ..

# Build libcurl7.61.0 statically
ADD https://curl.se/download/curl-7.61.0.tar.gz curl-7.61.0.tar.gz 
RUN tar -xvf curl-7.61.0.tar.gz && cd curl-7.61.0 && ./configure \
--disable-shared --enable-static --prefix=/usr/curl --disable-sspi --disable-ftp --disable-file --disable-dict \ 
--disable-telnet --disable-tftp --disable-rtsp --disable-pop3 --disable-imap --disable-smtp --disable-gopher \
--disable-smb --with-ssl && make && make install && cd ..

COPY get-ip.py /mcidle-cpp-/get-ip.py
COPY tests /mcidle-cpp/tests
COPY json /mcidle-cpp/json
COPY zlib /mcidle-cpp/zlib
COPY mcidle /mcidle-cpp/mcidle
COPY CMakeLists.txt /mcidle-cpp/

# Generate makefiles and build
#RUN ldd curl-7.70.0/lib/.libs/libcurl.a
RUN cmake -D OPENSSL_ROOT_DIR=/usr/local/ssl . && make

# Check how many dynamically linked libs there are
RUN (ldd /mcidle-cpp/bin/mcidle) || true

FROM busybox:glibc

COPY --from=builder /lib/x86_64-linux-gnu/libdl.so.2 /lib/libdl.so.2
COPY --from=builder /mcidle-cpp/bin/mcidle /mcidle
COPY --from=builder /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /lib/libstdc++.so.6
COPY --from=builder /lib/x86_64-linux-gnu/libgcc_s.so.1 /lib/libgcc_s.so.1

CMD ["/mcidle"]
