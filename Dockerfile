FROM ghcr.io/autamus/dyninst:master
# uncomment to use dyninst latest release
#FROM ghcr.io/autamus/dyninst
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y wget g++ libssl-dev libtbb-dev python3-dev build-essential \
    python3-jinja2 python3-pygments ghostscript doxygen python3-yaml python3-pip clang-format git
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2.tar.gz && \
    tar -xzvf cmake-3.20.2.tar.gz && \
    cd cmake-3.20.2/ && \
    ./bootstrap && \
    make && \
    make install
RUN cd /tmp && \
    git clone https://github.com/cheshirekow/cmake_format && \
    cd cmake_format && \
    pip3 install .
WORKDIR /code
ADD . /code
RUN make
ENV PATH /code/build/standalone:$PATH
