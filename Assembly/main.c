#include <stdio.h>
#include <stdlib.h>

#define BufSize 50
/*
char Commands[][] = [
["NUL", 0b00000],
["ADD", 0b00001],
["SUB", 0b00010],
["MUL", 0b00011],
["OR",  0b00100],
["NOR", 0b00101],
["AND", 0b00110],
["NAND",0b00111],
["XOR", 0b01000],
["XNOR",0b01001],
["NEG", 0b01010],
["<<",  0b01011],
[">>",  0b01100],
["",    0b01101],
[">",   0b01110],
["<",   0b01111],
["",    0b10000],
["==",  0b10001],
[">=",  0b10010],
["<=",  0b10011],
["!=",  0b10100],
["",    0b10101],
["COPY",0b10110],
["NUM", 0b10111],
["JMP", 0b11000],
["JMPN",0b11001],
["POP", 0b11010],
["PUSH",0b11011],
["PUSHN",0b11100],
["HALT",0b11101],
["",    0b11110],
["",    0b11111]]
*/

struct CODES {
	char CMD[5];
	int bin;
};

struct CODES Commands[32] = {
{"NUL", 0b00000},
{"ADD", 0b00001},
{"SUB", 0b00010},
{"MUL", 0b00011},
{"OR",  0b00100},
{"NOR", 0b00101},
{"AND", 0b00110},
{"NAND",0b00111},
{"XOR", 0b01000},
{"XNOR",0b01001},
{"NEG", 0b01010},
{"<<",  0b01011},
{">>",  0b01100},
{"",    0b01101},
{">",   0b01110},
{"<",   0b01111},
{"",    0b10000},
{"==",  0b10001},
{">=",  0b10010},
{"<=",  0b10011},
{"!=",  0b10100},
{"",    0b10101},
{"COPY",0b10110},
{"NUM", 0b10111},
{"JMP", 0b11000},
{"JMPN",0b11001},
{"POP", 0b11010},
{"PUSH",0b11011},
{"PUSHN",0b11100},
{"HALT",0b11101},
{"",    0b11110},
{"",    0b11111}
};

struct CODES A[11] = {
{"NUL", 0b0000},
{"IO",  0b0001},
{"RA",  0b0010},
{"RB",  0b0011},
{"RC",  0b0100},
{"RD",  0b0101},
{"RE",  0b0110},
{"MEM", 0b0111},
{"RP",  0b1000},
{"IP",  0b1001},
{"CFLAG",0b1010}
};

struct CODES B[8] = {
{"NUL", 0b000},
{"IO",  0b001},
{"RA",  0b010},
{"RB",  0b011},
{"RC",  0b100},
{"RD",  0b101},
{"RE",  0b110},
{"MEM", 0b111}};

struct CODES OUT[11] = {
{"NUL", 0b0000},
{"IO",  0b0001},
{"RA",  0b0010},
{"RB",  0b0011},
{"RC",  0b0100},
{"RD",  0b0101},
{"RE",  0b0110},
{"MEM", 0b0111},
{"RP",  0b1000},
{"IP",  0b1001},
{"CFLAG",0b1010}
};

int main(int argc, char * argv){
	char * filename = "testcase.txt";	
	FILE * file = fopen(filename, "r");
	if(file == NULL){ printf("File not found!\n"); return 1; }
	
	char buff[BufSize];
	fgets(buff, BufSize, file); 
	printf("%s", buff);

	return 0;
}
