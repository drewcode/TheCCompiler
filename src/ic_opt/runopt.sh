
g++ optimiser.cc -o opt -std=c++11
lines=$(wc ../../test.icg | awk '{print $1;}')
./opt $lines