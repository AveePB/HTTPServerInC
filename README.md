# HTTPServerInC

A low-level HTTP server implementation in C.

![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform: Linux](https://img.shields.io/badge/platform-Linux-lightgrey.svg)
![Language: C](https://img.shields.io/badge/language-C-orange.svg)

## Table of contents

- [Features](#features)
- [Getting started](#getting-started)
- [Usage](#usage)
- [Project structure](#project-structure)
- [License](#license)

## Features

- Raw socket handling, no external dependencies.
- HTTP request parsing.
- HTTP response generation.
- Small, readable codebase that is easy to extend.

## Getting started

### Requirements

- A C compiler (`gcc` or `clang`)
- [CMake](https://cmake.org/)
- Linux

### Build

```text
1. Clone the repository
   git clone https://github.com/<your-username>/HTTPServerInC.git

2. Build with CMake
   cd HTTPServerInC
   cmake -S . -B build
   cmake --build build
```

## Usage

1. Start the server with `./server`.
2. Send it a request, for example `curl -i http://localhost:8080/`.
3. Or open `http://localhost:8080` in a browser.

## Project structure

```text
HTTPServerInC/
├── assets/
│   ├── client.py          # test client
│   ├── get_request.txt    # sample GET request
│   ├── run_40_tests.sh    # test runner script
│   └── test_results.txt   # test output
├── include/
│   ├── http.h
│   └── myqueue.h
├── src/
│   ├── http.c
│   ├── main.c
│   └── myqueue.c
├── static/
│   ├── about_me.html
│   └── home.html
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## License

Distributed under the MIT License. See [`LICENSE`](LICENSE) for details.