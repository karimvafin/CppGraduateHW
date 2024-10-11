# Cpp Graduate Homeworks

## 1. Cache
Building project:
```
git submodule init
git submodule update
cd cache
cmake -S . -B build
cd build
make
```
Running tests:
```
./tests/run_nameOfTest
```
Avalable tests:

***test_cacheIdeal*** -- test for ideal cache

***test_cacheLRU*** -- test for LRU cache

***test_cacheARC*** -- test for ARC cache

Tests take a sequence of integers as an input. The first number -- cache size, the second number -- number of pages, and then ids of pages. Example:
```
$ ./tests/run_test_cacheARC
4 5 1 2 3 1 2
```
