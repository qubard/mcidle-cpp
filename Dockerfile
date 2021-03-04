FROM ubuntu:18.04
ENTRYPOINT /mcidle-cpp/bin/mcidle

RUN apt-get update && apt-get install -y build-essential libssl-dev libcurl4-openssl-dev zlib1g-dev xz-utils clang curl

# Install latest cmake
ADD https://github.com/Kitware/CMake/releases/download/v3.18.0-rc3/cmake-3.18.0-rc3-Linux-x86_64.sh /cmake-3.18.0-rc3-Linux-x86_64.sh 
RUN mkdir /opt/cmake
RUN sh /cmake-3.18.0-rc3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN cmake --version

# Install boost
ADD https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz boost_1_72_0.tar.gz
RUN tar xfz boost_1_72_0.tar.gz     && rm boost_1_72_0.tar.gz     && cd boost_1_72_0     && ./bootstrap.sh     && ./b2 --without-python --prefix=/usr -j 4 link=shared runtime-link=shared install     && cd .. && rm -rf boost_1_72_0/ && ldconfig

RUN mkdir -p /mcidle-cpp/mcidle

COPY tests /mcidle-cpp/tests
COPY json /mcidle-cpp/json
COPY zlib /mcidle-cpp/zlib
COPY mcidle /mcidle-cpp/mcidle
COPY CMakeLists.txt /mcidle-cpp/

WORKDIR /mcidle-cpp

# Generate makefiles and build
RUN cmake . && make

CMD ["exec /mcidle-cpp/bin/mcidle"]
