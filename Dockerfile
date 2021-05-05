FROM ghcr.io/autamus/dyninst
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y wget g++ libssl-dev git libtbb-dev python3-dev build-essential \
    python3-jinja2 python3-pygments ghostscript doxygen
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2.tar.gz && \
    tar -xzvf cmake-3.20.2.tar.gz && \
    cd cmake-3.20.2/ && \
    ./bootstrap && \
    make && \
    make install

WORKDIR /code
ADD . /code
