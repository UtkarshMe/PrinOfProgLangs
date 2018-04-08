
# Make all files
all: symbol_table

symbol_table:
	gcc -g -o symbol_table.o symbol_table.c


.PHONY: clean
clean:
	rm -f *.o symbol_table.o
