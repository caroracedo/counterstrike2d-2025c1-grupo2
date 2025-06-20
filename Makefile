.PHONY: all test clean editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

run-tests:
	./build/taller_server_tests &
	./build/taller_client_tests

all: clean compile-debug run-tests

clean:
	rm -Rf build/
