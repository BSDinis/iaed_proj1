#!/bin/bash
### use: ./run_loud_examples <program.c>

#doesnt supress diff output

gcc *.c *.h -o proj1
./proj1 < ex0.in > ex0.mine.out; diff ex0.out ex0.mine.out
./proj1 < ex1.in > ex1.mine.out; diff ex1.out ex1.mine.out
./proj1 < ex2.in > ex2.mine.out; diff ex2.out ex2.mine.out
./proj1 mat.in < ex3.in > ex3.mine.out; diff ex3.out ex3.mine.out
./proj1 mat.in < ex4.in > ex4.mine.out; diff ex4.out ex4.mine.out
