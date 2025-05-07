# Soil Engine

## Introduction

Soil is a C++ game engine based on OpenGL and OpenAL with capabilities of creating 2D/3D games.

### Games using Soil engine

* TBD

## Install

### Prerequisite

* [Conan 2](https://docs.conan.io/2/)
* Conan packages from [VloRyan/conan](https://github.com/VloRyan/conan)
    * gl3w
    * openal
* Some xorg libs as dependencies from gl3w
  * libxaw
  * libxres

### Build

```bash
conan create . --build=missing
```
