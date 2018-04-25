#!/bin/bash
### use: ./run_examples <program.c>

TESTDIR="../tests"

gcc *.c *.h -o $TESTDIR/proj1
./$TESTDIR/proj1 < $TESTDIR/ex0.in > $TESTDIR/ex0.mine.out; diff -q $TESTDIR/ex0.out $TESTDIR/ex0.mine.out
./$TESTDIR/proj1 < $TESTDIR/ex1.in > $TESTDIR/ex1.mine.out; diff -q $TESTDIR/ex1.out $TESTDIR/ex1.mine.out
./$TESTDIR/proj1 < $TESTDIR/ex2.in > $TESTDIR/ex2.mine.out; diff -q $TESTDIR/ex2.out $TESTDIR/ex2.mine.out
./$TESTDIR/proj1 mat.in < $TESTDIR/ex3.in > $TESTDIR/ex3.mine.out; diff -q $TESTDIR/ex3.out $TESTDIR/ex3.mine.out
./$TESTDIR/proj1 mat.in < $TESTDIR/ex4.in > $TESTDIR/ex4.mine.out; diff -q $TESTDIR/ex4.out $TESTDIR/ex4.mine.out
