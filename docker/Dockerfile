FROM debian:stretch
LABEL maintainer="Billiecoin Developers <jsidhu@blockchainfoundry.co>"
LABEL description="Dockerised BilliecoinCore, built from Travis"

RUN apt-get update && apt-get -y upgrade && apt-get clean && rm -fr /var/cache/apt/*

COPY bin/* /usr/bin/
