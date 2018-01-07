
.PHONY: all run clean pack unpack

all:
	cmake -B"cmake-build" -H.
	make -C "cmake-build"

run:
	./simulation

clean:
	make clean -C "cmake-build"
	rm -rf "cmake-build" *.out simulation

pack:
	tar -zcvf 01_xkozel18_xaugus09.tar.gz src experiments Makefile CMakeLists.txt ploting.m doc.pdf
