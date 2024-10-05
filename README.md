# Cpp Graduate Homeworks

## 1. Cache
Building project:
```
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
***test_end2end*** -- the main end to end test that compares LRU, ARC and Ideal caches
***test_cacheARC*** -- test for ARC cache with data visualizing

Tests take a sequence of integers as an input. The first number -- cache size, the second number -- number of pages, and then ids of pages. Example:
```
$ ./tests/run_test_end2end
4 5 1 2 3 1 2
```
