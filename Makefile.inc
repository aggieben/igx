#-*-Mode: makefile-*-
TOP=/home/bc/academic/thesis/igx

AS=/usr/bin/nasm
CC=/usr/bin/gcc
INCLUDE=-I${TOP}/include
SRC=${TOP}/src
OPT=-O0 -g
CXXDISABLE=-fno-rtti -fno-exceptions

CFLAGS=${INCLUDE} ${OPT} \
	-nostdlib -fno-builtin
CXXFLAGS=${CFLAGS} ${CXXDISABLE}
LDFLAGS=-d
