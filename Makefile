
monkeys.out: main.c
	clang -g -Wall -o monkeys.out main.c


.PHONY:clean
clean:
	rm -f *~
	rm -f *.out
	rm -rf *.dSYM/
