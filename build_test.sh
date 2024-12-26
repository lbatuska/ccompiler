#!/bin/bash

BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
  echo "$BUILD_DIR exists."
else
  echo "$BUILD_DIR does not exist. Creating it"
  mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR" || exit

if [ -z "$(ls -A ./)" ]; then
  echo "$BUILD_DIR is empty, configuring build..."
  cmake ..
  make
else
  echo "$BUILD_DIR is not empty, running make..."
  make
fi

if [ -f "./ccompiler" ]; then
  "./ccompiler" ../test.test
fi
