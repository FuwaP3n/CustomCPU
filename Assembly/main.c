#include <stdio.h>
#include <stdlib.h>

#define BufSize 50

struct CODES {
	char CMD[5];
	char bin[5];
};
int translate(char * word){
struct CODES Commands[32] = {
{"NUL", "00000"},
{"ADD", "00001"},
{"SUB", "00010"},
{"MUL", "00011"},
{"OR",  "00100"},
{"NOR", "00101"},
{"AND", "00110"},
{"NAND","00111"},
{"XOR", "01000"},
{"XNOR","01001"},
{"NEG", "01010"},
{"<<",  "01011"},
{">>",  "01100"},
{"",    "01101"},
{">",   "01110"},
{"<",   "01111"},
{"",    "10000"},
{"==",  "10001"},
{">=",  "10010"},
{"<=",  "10011"},
{"!=",  "10100"},
{"",    "10101"},
{"COPY","10110"},
{"NUM", "10111"},
{"JMP", "11000"},
{"JMPN","11001"},
{"POP", "11010"},
{"PUSH","11011"},
{"PUSHN","11100"},
{"HALT","11101"},
{"",    "11110"},
{"",    "11111"}
};

struct CODES A[11] = {
{"NUL", "0000"},
{"IO",  "0001"},
{"RA",  "0010"},
{"RB",  "0011"},
{"RC",  "0100"},
{"RD",  "0101"},
{"RE",  "0110"},
{"MEM", "0111"},
{"RP",  "1000"},
{"IP",  "1001"},
{"CFLAG","1010"}
};

struct CODES B[8] = {
{"NUL", "000"},
{"IO",  "001"},
{"RA",  "010"},
{"RB",  "011"},
{"RC",  "100"},
{"RD",  "101"},
{"RE",  "110"},
{"MEM", "111"}};

struct CODES OUT[11] = {
{"NUL", "0000"},
{"IO",  "0001"},
{"RA",  "0010"},
{"RB",  "0011"},
{"RC",  "0100"},
{"RD",  "0101"},
{"RE",  "0110"},
{"MEM", "0111"},
{"RP",  "1000"},
{"IP",  "1001"},
{"CFLAG","1010"}
};
	
	


	return 0;
}




int main(int argc, char * argv){
	char * filename = "testcase.txt";	
	FILE * file = fopen(filename, "r");
	if(file == NULL){ printf("File not found!\n"); return 1; }
	
	char buff[BufSize];
	fgets(buff, BufSize, file); 
	printf("%s", buff);

	return 0;
}
