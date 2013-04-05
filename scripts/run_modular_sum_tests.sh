#!/bin/sh

run_test()
{
    name="$1"
    echo "$name..."

    echo rm -f ./modular_sum_overhead_small
    rm -f ./modular_sum_overhead_small
    g++ --version | head -n 1
    echo g++ -DCOUNT="$COUNT" -D$name -I./ -std=c++11 -O3 ./benchmarks/modular_sum_overhead_small.cpp -o ./benchmarks/modular_sum_overhead_small
    g++ -DCOUNT="$COUNT" -D$name -I./ -std=c++11 -O3 ./benchmarks/modular_sum_overhead_small.cpp -o ./benchmarks/modular_sum_overhead_small
    
    ./benchmarks/modular_sum_overhead_small
    echo ""
    
    echo rm -f ./modular_sum_overhead_small
    rm -f ./modular_sum_overhead_small
    clang++ --version | head -n 1
    echo clang++ -DCOUNT=$COUNT -D$name -I./ -std=c++11 -O3 ./benchmarks/modular_sum_overhead_small.cpp -o ./benchmarks/modular_sum_overhead_small
    clang++ -DCOUNT=$COUNT -D$name -I./ -std=c++11 -O3 ./benchmarks/modular_sum_overhead_small.cpp -o ./benchmarks/modular_sum_overhead_small
    
    ./benchmarks/modular_sum_overhead_small
    echo ""
}

if [ -z "${COUNT+xxx}" ]
then
    COUNT=1000000000
fi

echo "System information:"
#sudo lshw -C system -C memory -C processor
echo ""

run_test "SUM_CONTROL_1"
run_test "SUM_CONTROL_2"
run_test "SUM_CONTROL_3"
run_test "SUM_TEST"
