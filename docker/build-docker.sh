#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..

DOCKER_IMAGE=${DOCKER_IMAGE:-billiecoin/billiecoind-develop}
DOCKER_TAG=${DOCKER_TAG:-latest}

BUILD_DIR=${BUILD_DIR:-.}

rm docker/bin/*
mkdir docker/bin
cp $BUILD_DIR/src/billiecoind docker/bin/
cp $BUILD_DIR/src/billiecoin-cli docker/bin/
cp $BUILD_DIR/src/billiecoin-tx docker/bin/
strip docker/bin/billiecoind
strip docker/bin/billiecoin-cli
strip docker/bin/billiecoin-tx

docker build --pull -t $DOCKER_IMAGE:$DOCKER_TAG -f docker/Dockerfile docker
