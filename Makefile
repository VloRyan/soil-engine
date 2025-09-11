.PHONY: build test

build:
	conan create . --build=missing -o skip_test=False -o build_samples=False

test:
	conan build . --build=missing -o skip_test=False -o build_samples=False
