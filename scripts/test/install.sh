#!/bin/bash

rm -rf ./tests/*
mkdir -p ./tests/bunch-1/
cp -R ./src/images/ ./tests/bunch-1/images-1
cp -R ./src/images/ ./tests/bunch-1/images-2
cp -R ./src/images/ ./tests/bunch-1/images-3
cp -R ./src/images/ ./tests/bunch-1/images-4
cp -R ./src/images/ ./tests/bunch-1/images-5
cp -R ./tests/bunch-1/ ./tests/bunch-2/
cp -R ./tests/bunch-1/ ./tests/bunch-3/
cp -R ./tests/bunch-1/ ./tests/bunch-4/
cp -R ./tests/bunch-1/ ./tests/bunch-5/