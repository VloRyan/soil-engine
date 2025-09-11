.PHONY: build build-quick build-all

build:
	conan create . --build=missing -o skip_test=False -o build_samples=False

build-quick:
	conan build . --build=missing -o skip_test=True -o build_samples=False

build-all:
	conan create . --build=missing -o skip_test=False -o build_samples=True
