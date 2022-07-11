rm -rf build
mkdir build
clang++ -std=c++11 `cat build_index.txt` -o build/nouse