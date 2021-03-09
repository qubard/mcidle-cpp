FROM cub949/mcidle-builder:latest as builder

WORKDIR /mcidle-cpp/

COPY get-ip.py /mcidle-cpp-/get-ip.py
COPY tests /mcidle-cpp/tests
COPY json /mcidle-cpp/json
COPY zlib /mcidle-cpp/zlib
COPY mcidle /mcidle-cpp/mcidle
COPY CMakeLists.txt /mcidle-cpp/

# Generate makefiles and build
RUN cmake -D OPENSSL_ROOT_DIR=/usr/local/ssl . && make

# Check how many dynamically linked libs there are
RUN (ldd /mcidle-cpp/bin/mcidle) || true

FROM busybox:glibc

COPY --from=builder /lib/x86_64-linux-gnu/libdl.so.2 /lib/libdl.so.2
COPY --from=builder /mcidle-cpp/bin/mcidle /mcidle
COPY --from=builder /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /lib/libstdc++.so.6
COPY --from=builder /lib/x86_64-linux-gnu/libgcc_s.so.1 /lib/libgcc_s.so.1

CMD ["/mcidle"]
