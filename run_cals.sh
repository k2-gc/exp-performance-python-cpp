# !/bin/bash

echo "Compiling and running C++ code..."
g++ calc_mc.cpp -O3 -march=native -std=c++17 -o calc_mc

echo "Running C++ code..."
echo "================================"
./calc_mc
echo "================================"

echo "Running Python code..."
echo "Running 01_for_loop.py..."
echo "================================"
python 01_for_loop.py
echo "================================"
echo "Running 02_list_comprehension.py..."
echo "================================"
python 02_list_comprehension.py
echo "================================"
echo "Running 03_generator.py..."
echo "================================"
python 03_generator.py
echo "================================"
echo "Running 04_numpy.py..."
echo "================================"
python 04_numpy.py
echo "================================"
echo "Running 05_numba.py..."
echo "================================"
python 05_numba.py
echo "================================"