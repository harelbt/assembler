assembler:	assembler.o	assembler_data_types.o assembler_data_types.o errors.o	first_pass.o	in_out_tools.o	line_analyzer.o second_pass.o	symbol_table.o translator.o
	gcc	-g -Wall -ansi -pedantic	assembler.o	assembler_data_types.o errors.o	first_pass.o	in_out_tools.o	line_analyzer.o second_pass.o	symbol_table.o translator.o -lm -o assembler
assembler.o:	assembler.c
	gcc	-c -Wall -ansi -pedantic assembler.c -o assembler.o
assembler_data_types.o:	assembler_data_types.c	
	gcc	-c -Wall -ansi -pedantic assembler_data_types.c -o assembler_data_types.o
errors.o:	errors.c
	gcc -c -Wall -ansi -pedantic	errors.c -o errors.o
first_pass.o:	first_pass.c
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o
in_out_tools.o:	in_out_tools.c
	gcc -c -Wall -ansi -pedantic in_out_tools.c  -o in_out_tools.o
line_analyzer.o: line_analyzer.c
	gcc -c -Wall -ansi -pedantic line_analyzer.c  -o line_analyzer.o
second_pass.o:	second_pass.c		
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o
symbolTable.o	: symbolTable.c
	gcc -c -Wall -ansi -pedantic	 symbolTable.c -o symbolTable.o
translator.o:	translator.c
	gcc -c -Wall -ansi -pedantic	translator.c -o translator.o
