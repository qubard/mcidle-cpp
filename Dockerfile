FROM lballabio/boost:1.72.0-cosmic
ENTRYPOINT /mcidle-cpp/bin/mcidle

# The pre-built boost lib we use runs debian so use ubuntu packages
RUN sed -i -e 's/archive.ubuntu.com\|security.ubuntu.com/old-releases.ubuntu.com/g' /etc/apt/sources.list

RUN apt-get update && apt-get upgrade && apt-get install -y libssl-dev libcurl4-openssl-dev zlib1g-dev

# Install latest cmake
ADD https://github.com/Kitware/CMake/releases/download/v3.18.0-rc3/cmake-3.18.0-rc3-Linux-x86_64.sh /cmake-3.18.0-rc3-Linux-x86_64.sh 
RUN mkdir /opt/cmake
RUN sh /cmake-3.18.0-rc3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN cmake --version

RUN mkdir -p /mcidle-cpp/mcidle

COPY json /mcidle-cpp/json
COPY zlib /mcidle-cpp/zlib
COPY mcidle /mcidle-cpp/mcidle
COPY CMakeLists.txt /mcidle-cpp/

WORKDIR /mcidle-cpp

# Generate makefiles and build
# for some reason clang breaks here
RUN cmake -DCMAKE_CXX_COMPILER=g++ . && make
