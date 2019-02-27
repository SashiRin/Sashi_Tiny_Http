Sashi Tiny Http
=================

A tiny, fast, platform-independent static HTTP server library implemented using C++ and Boost ASIO.

### Features

* Asynchronous request handling
* Platform independent
* HTTP persistent connection (for HTTP/1.1 keep-alive)
* ExpireMap-based cache control for static resources

### Usage

See main.cpp for example usage. 

### Dependencies

* Boost.Asio
* Boost is required to compile the examples

### Compile and run

Compile with a C++11 compliant compiler:
```sh
mkdir build
cd build
cmake ..
make
cd ..
```
