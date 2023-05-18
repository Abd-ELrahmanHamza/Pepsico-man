# makefile for linux
# if you get an error like this:
# /usr/bin/ld: cannot find -lX11
# then install the libx11-dev lib using something like:
# sudo apt-get install libx11-dev
# and for the 32 bit version, use
# sudo apt-get install libx11-dev:i386

CPP = g++
OPTS =  -I"../../include" -I"3rdparty" -L"/usr/lib" -L"3rdparty" -DLINUX -lfltklinux -lX11 ../../bin/linux-gcc/libIrrKlang.so -pthread

all:
	$(CPP) main.cpp window.cxx -m32 -o player $(OPTS)

clean:
	rm player
