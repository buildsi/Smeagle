FROM ghcr.io/autamus/dyninst:master
# uncomment to use dyninst latest release
#FROM ghcr.io/autamus/dyninst
ENV DEBIAN_FRONTEND=noninteractive
ENV CMAKE=3.20.2
RUN apt-get update && \
    apt-get install -y wget g++ libssl-dev libtbb-dev python3-dev build-essential curl \
    python3-jinja2 python3-pygments ghostscript doxygen python3-yaml python3-pip clang-format git
RUN curl -s -L https://github.com/Kitware/CMake/releases/download/v$CMAKE/cmake-$CMAKE-linux-x86_64.sh > cmake.sh && \
    sh cmake.sh --prefix=/usr/local --skip-license && \
    rm cmake.sh
RUN cd /tmp && \
    git clone https://github.com/cheshirekow/cmake_format && \
    cd cmake_format && \
    pip3 install .
WORKDIR /code
ADD . /code
RUN make
ENV PATH /code/build/standalone:$PATH
ENTRYPOINT ["/code/build/standalone/Smeagle"]
