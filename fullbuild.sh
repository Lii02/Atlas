#!/bin/bash

mkdir -p img
mkdir -p bcfs

make clear -s
make -s
make clearsrc -s
bash exec.sh