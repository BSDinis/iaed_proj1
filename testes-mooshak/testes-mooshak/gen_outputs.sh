#!/bin/bash
### use: ./gen_outputs <program.c>

gcc -Wall -ansi $1 -o proj1
./proj1 < test01.in > test01.out
./proj1 < test02.in > test02.out
./proj1 < test03.in > test03.out
./proj1 < test04.in > test04.out
./proj1 < test05.in > test05.out
./proj1 < test06.in > test06.out
./proj1 < test07.in > test07.out
./proj1 < test08.in > test08.out
./proj1 < test09.in > test09.out
./proj1 < test10.in > test10.out
./proj1 mooshak.mine.out < test11.in > test11.out
./proj1 < test12.in > test12.out
./proj1 < test13.in > test13.out
./proj1 < test14.in > test14.out
./proj1 < test15.in > test15.out
./proj1 < test16.in > test16.out