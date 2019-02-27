Sashi Tiny Http
=================

A tiny, fast, platform-independent static HTTP server library implemented using C++ and Boost ASIO.
Inspired heavily from [Simple-Web-Server](https://gitlab.com/eidheim/Simple-Web-Server) and Boost ASIO documentation.

### Features

* Asynchronous request handling
* Platform independent
* HTTP persistent connection (for HTTP/1.1 keep-alive)
* ExpireMap-based cache control for static resources

### Usage

See [main.cpp](main.cpp) for example usage. 

### Dependencies

* Boost ASIO
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

### TODO

* HTTPS
* HTTP/2
* QUIC
* POST support
