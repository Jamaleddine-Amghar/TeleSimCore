# TeleSimCore

![CI](https://github.com/Jamaleddine-Amghar/TeleSimCore/actions/workflows/ci.yml/badge.svg)

Simulateur de chaine de transmission numerique en C++17.
Projet R&D Telecom — DSP, modulation BPSK/QPSK, canal AWGN, Python bridge.

## Stack technique
- C++17 | CMake 3.20+ | Ninja
- Google Test 1.14
- Python 3 + pybind11 + matplotlib
- GitHub Actions CI/CD

## Build

    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    cd build && ctest --output-on-failure

## Structure

    src/core/        <- Bibliotheque C++ metier (DSP, modulation, canal)
    src/app/         <- Executable principal
    tests/unit/      <- Tests unitaires Google Test
    python/          <- Bridge pybind11 + scripts analyse
