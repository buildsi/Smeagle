FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y cmake \
      libtbb-dev \
      g++ \
      git \
      libelf-dev \
      python3-dev \
      clang-format \
      texlive-full
WORKDIR /code
ADD . /code
