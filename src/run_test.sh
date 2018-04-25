#!/bin/bash
### use: ./run_examples <program.c> <test_n>

TESTDIR="../tests"

gcc *.c *.h -o $TESTDIR/proj1
./$TESTDIR/proj1 < $TESTDIR/ex$2.in > $TESTDIR/ex$2.mine.out; diff -q $TESTDIR/ex$2.out $TESTDIR/ex$2.mine.out
