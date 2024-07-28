all: build

build:
	./build.sh

test:
	./qemu.sh

clean:
	./clean.sh

headers:
	./headers.sh