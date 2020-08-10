#!/bin/bash

mkdir -p img
mkdir -p bcfs

make clear
make
make clearsrc
bash exec-debian.sh