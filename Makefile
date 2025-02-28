WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c11 -g $(WARNING) $(ERROR) 

SRCS = main.c stringm.c
OBJS = $(SRCS:%.c=%.o)

stringm: $(OBJS) 
	$(GCC) $(OBJS) -o stringm

.c.o: 
	$(GCC) -c $*.c 

testall: stringm
	./stringm -1

test1: stringm
	./stringm 1

test2: stringm
	./stringm 2

test3: stringm
	./stringm 3

test4: stringm
	./stringm 4

test5: stringm
	./stringm 5

test6: stringm
	./stringm 6

test7: stringm
	./stringm 7

test8: stringm
	./stringm 8

test9: stringm
	./stringm 9

test10: stringm
	./stringm 10

test11: stringm
	./stringm 11

test12: stringm
	./stringm 12

test13: stringm
	./stringm 13

test14: stringm
	./stringm 14

test15: stringm
	./stringm 15

test16: stringm
	./stringm 16

test17: stringm
	./stringm 17

test18: stringm
	./stringm 18

test19: stringm
	./stringm 19

test20: stringm
	./stringm 20

test21: stringm
	./stringm 21

test22: stringm
	./stringm 22

test23: stringm
	./stringm 23

test24: stringm
	./stringm 24

# TODO: put the ones that have memory allocation here
leak: stringm
	valgrind ./stringm -1

clean: # remove all machine generated files
	rm -f stringm *.o
	
