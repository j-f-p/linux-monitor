.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: bldg # Build with the GCC 10 C and C++ compilers
.ONESHELL:
bldg:
	mkdir -p build
	cd build
	cmake -D CMAKE_C_COMPILER=gcc-10 -D CMAKE_CXX_COMPILER=g++-10 ..
	make

.PHONY: bldc # Build with the LLVM 10 C and C++ compilers
.ONESHELL:
bldc:
	mkdir -p build
	cd build
	cmake -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ ..
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build
