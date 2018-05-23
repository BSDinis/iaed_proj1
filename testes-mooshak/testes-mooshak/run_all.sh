#!/bin/bash
### use: ./run_all <program.c>

gcc -Wall -ansi $1 -o proj1
time ./proj1 < test01.in > test01.mine.out; diff -q test01.out test01.mine.out
time ./proj1 < test02.in > test02.mine.out; diff -q test02.out test02.mine.out
time ./proj1 < test03.in > test03.mine.out; diff -q test03.out test03.mine.out
time ./proj1 < test04.in > test04.mine.out; diff -q test04.out test04.mine.out
time ./proj1 < test05.in > test05.mine.out; diff -q test05.out test05.mine.out
time ./proj1 < test06.in > test06.mine.out; diff -q test06.out test06.mine.out
time ./proj1 < test07.in > test07.mine.out; diff -q test07.out test07.mine.out
time ./proj1 < test08.in > test08.mine.out; diff -q test08.out test08.mine.out
time ./proj1 < test09.in > test09.mine.out; diff -q test09.out test09.mine.out
time ./proj1 < test10.in > test10.mine.out; diff -q test10.out test10.mine.out
time ./proj1 mooshak.mine.out < test11.in > test11.mine.out; diff -q test11.out test11.mine.out
time ./proj1 < test12.in > test12.mine.out; diff -q test12.out test12.mine.out
time ./proj1 < test13.in > test13.mine.out; diff -q test13.out test13.mine.out
time ./proj1 < test14.in > test14.mine.out; diff -q test14.out test14.mine.out
time ./proj1 < test15.in > test15.mine.out; diff -q test15.out test15.mine.out
time ./proj1 < test16.in > test16.mine.out; diff -q test16.out test16.mine.out
