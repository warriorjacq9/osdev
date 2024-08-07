all: iso

build:
	./build.sh

iso:
	./iso.sh

test:
	./qemu.sh

clean:
	./clean.sh

headers:
	./headers.sh