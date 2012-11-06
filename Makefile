
monkeys.out: main.c
	clang -g -Wall -o monkeys.out main.c

.PHONY:analyze
analyze:
	clang --analyze main.c

.PHONY:clean
clean:
	rm -f *~
	rm -f *.out
	rm -rf *.dSYM/
	rm -f main.plist
