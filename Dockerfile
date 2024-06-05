FROM alpine

# Define working directory.
WORKDIR /usr/local/

RUN apk add alpine-sdk linux-headers cmake lsb-release-minimal

RUN wget https://github.com/google/or-tools/releases/download/v9.10/or-tools_amd64_alpine-edge_cpp_v9.10.4067.tar.gz && \
    tar zxvf or-tools_amd64_alpine-edge_cpp_v9.10.4067.tar.gz && rm or-tools_amd64_alpine-edge_cpp_v9.10.4067.tar.gz && \
    mv or-tools_x86_64_alpine-edge_cpp_v9.10.4067 or-tools && \
    cp -pr /usr/local/or-tools/include /usr/local/include && \
    cp -pr /usr/local/or-tools/lib/libprotobuf* /usr/local/lib/ && \
    cp -pr /usr/local/or-tools/lib/libortools.so* /usr/local/lib/ && \
    rm -r /usr/local/or-tools


RUN wget https://github.com/google/glog/archive/refs/heads/master.zip && \
    unzip master.zip && rm master.zip && \
    cd glog-master && \
    cmake -S . -B build -G "Unix Makefiles" && \
    cmake --build build && \
    cmake --build build --target install && \
    cd /usr/local && rm -r /usr/local/glog-master

RUN wget https://github.com/nlohmann/json/archive/refs/heads/develop.zip && \
    unzip develop.zip && rm develop.zip && \
    cp -pr /usr/local/json-develop/single_include/nlohmann /usr/local/include && \
    cd /usr/local && rm -r /usr/local/json-develop


CMD ["sh"]