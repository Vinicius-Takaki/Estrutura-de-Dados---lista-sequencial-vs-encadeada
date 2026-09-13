#!/usr/bin/env bash
# Compiles and runs both benchmarks, writes results.csv, then plots the chart.
set -e

cd "$(dirname "$0")"

echo "Compiling benchmarks..."
g++ -O2 -o /tmp/bench_seq benchmark_sequential.cpp
g++ -O2 -o /tmp/bench_link benchmark_linked.cpp

echo "Running benchmarks..."
echo "structure,operation,n,comparisons,movements" > results.csv
/tmp/bench_seq >> results.csv
/tmp/bench_link >> results.csv

echo "Plotting results..."
python3 plot_results.py

echo "Done. See results.csv and ../assets/comparison_chart.png"
