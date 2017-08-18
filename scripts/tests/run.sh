#!/bin/bash

echo "Memory tests"

echo "paes --encrypt --memory";time ./bin/paes --encrypt --memory;sleep 5
echo "paes --decrypt --memory";time ./bin/paes --decrypt --memory;sleep 5
echo "paes --encrypt -t 2 --memory";time ./bin/paes --encrypt -t 2 --memory;sleep 5
echo "paes --decrypt -t 2 --memory";time ./bin/paes --decrypt -t 2 --memory;sleep 5
# echo "paes --encrypt -t 3 --memory";time ./bin/paes --encrypt -t 3 --memory;sleep 5
# echo "paes --decrypt -t 3 --memory";time ./bin/paes --decrypt -t 3 --memory;sleep 5
echo "paes --encrypt -t 4 --memory";time ./bin/paes --encrypt -t 4 --memory;sleep 5
echo "paes --decrypt -t 4 --memory";time ./bin/paes --decrypt -t 4 --memory;sleep 5
# echo "paes --encrypt -t 5 --memory";time ./bin/paes --encrypt -t 5 --memory;sleep 5
# echo "paes --decrypt -t 5 --memory";time ./bin/paes --decrypt -t 5 --memory;sleep 5
# echo "paes --encrypt -t 6 --memory";time ./bin/paes --encrypt -t 6 --memory;sleep 5
# echo "paes --decrypt -t 6 --memory";time ./bin/paes --decrypt -t 6 --memory;sleep 5
# echo "paes --encrypt -t 7 --memory";time ./bin/paes --encrypt -t 7 --memory;sleep 5
# echo "paes --decrypt -t 7 --memory";time ./bin/paes --decrypt -t 7 --memory;sleep 5
echo "paes --encrypt -t 8 --memory";time ./bin/paes --encrypt -t 8 --memory;sleep 5
echo "paes --decrypt -t 8 --memory";time ./bin/paes --decrypt -t 8 --memory;sleep 5
echo "paes --encrypt -t 16 --memory";time ./bin/paes --encrypt -t 16 --memory;sleep 5
echo "paes --decrypt -t 16 --memory";time ./bin/paes --decrypt -t 16 --memory;sleep 5

echo "Non-Memory tests"
echo "paes --encrypt";time ./bin/paes --encrypt;sleep 5
echo "paes --decrypt";time ./bin/paes --decrypt;sleep 5
echo "paes --encrypt -t 2";time ./bin/paes --encrypt -t 2;sleep 5
echo "paes --decrypt -t 2";time ./bin/paes --decrypt -t 2;sleep 5
# echo "paes --encrypt -t 3";time ./bin/paes --encrypt -t 3
# echo "paes --decrypt -t 3";time ./bin/paes --decrypt -t 3
echo "paes --encrypt -t 4";time ./bin/paes --encrypt -t 4;sleep 5
echo "paes --decrypt -t 4";time ./bin/paes --decrypt -t 4;sleep 5
# echo "paes --encrypt -t 5";time ./bin/paes --encrypt -t 5
# echo "paes --decrypt -t 5";time ./bin/paes --decrypt -t 5
# echo "paes --encrypt -t 6";time ./bin/paes --encrypt -t 6
# echo "paes --decrypt -t 6";time ./bin/paes --decrypt -t 6
# echo "paes --encrypt -t 7";time ./bin/paes --encrypt -t 7
# echo "paes --decrypt -t 7";time ./bin/paes --decrypt -t 7
echo "paes --encrypt -t 8";time ./bin/paes --encrypt -t 8;sleep 5
echo "paes --decrypt -t 8";time ./bin/paes --decrypt -t 8;sleep 5
echo "paes --encrypt -t 16";time ./bin/paes --encrypt -t 16;sleep 5
echo "paes --decrypt -t 16";time ./bin/paes --decrypt -t 16;sleep 5