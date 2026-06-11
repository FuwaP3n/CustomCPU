#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//WARNINGS
bool UNKNOWN_WARNING = true;
bool LONGER_BINARY_WARNING = true;
bool LONGER_HEX_WARNING = true;
bool LONGER_DEC_WARNING = true;
bool GREATER_SMALLER_DEC_WARNING = true;
bool UNKNOWN_INSTRUCTION_WARNING = true;

bool ABORT = false;

char HEX[16][1] = {{'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}};

struct CODES {
	char CMD[5];
	char bin[5];
	int size;
};
struct CODES CMD[32] = {
{"NUL", "00000", 3},
{"ADD", "00001", 3},
{"SUB", "00010", 3},
{"MUL", "00011", 3},
{"OR",  "00100", 2},
{"NOR", "00101", 3},
{"AND", "00110", 3},
{"NAND","00111", 4},
{"XOR", "01000", 3},
{"XNOR","01001", 4},
{"NEG", "01010", 3},
{"<<",  "01011", 2},
{">>",  "01100", 2},
{"",    "01101", 0},
{">",   "01110", 1},
{"<",   "01111", 1},
{"",    "10000", 0},
{"==",  "10001", 2},
{">=",  "10010", 2},
{"<=",  "10011", 2},
{"!=",  "10100", 2},
{"",    "10101", 0},
{"COPY","10110", 4},
{"NUM", "10111", 3},
{"JMP", "11000", 3},
{"JMPN","11001", 4},
{"POP", "11010", 3},
{"PUSH","11011", 4},
{"PUSHN","11100",5},
{"HALT","11101", 4},
{"",    "11110", 0},
{"",    "11111", 0}
};

struct CODES A[11] = {
{"NUL", "0000", 3},
{"IO",  "0001", 2},
{"RA",  "0010", 2},
{"RB",  "0011", 2},
{"RC",  "0100", 2},
{"RD",  "0101", 2},
{"RE",  "0110", 2},
{"MEM", "0111", 3},
{"RP",  "1000", 2},
{"IP",  "1001", 2},
{"CFLAG","1010",5}
};
void throw_error(int error, int line, char * carg, int * iarg){
	ABORT = true;
	switch(error){
		case 0:
			printf("ERROR: Unexpected character \"%c\" on position (%d, %d)\n", *carg, *iarg+1, line);
			break;
		default:
			printf("ERROR: Unknown error while reading line %d\n", line);
			break;
	}
}


void throw_warning(int warning, int line, char * function){
	switch(warning){
		case 0:
			if(!LONGER_BINARY_WARNING){ break; }
			printf("WARNING: binary number on line %d is longer than 8bits. Readed only last 8bits of a number.\n", line);
			break;
		case 1:
			if(!LONGER_HEX_WARNING){ break; }
			printf("WARNING: hex number on line %d is longer than 2 characters. Readed only last 2 characters.\n", line);
			break;
		case 2:
			if(!LONGER_DEC_WARNING){ break; }
			printf("WARNING: decimal number on line %d is longer than 3 characters. Readed only last 3 characters.\n", line);
			break;
		case 3:
			if(!GREATER_SMALLER_DEC_WARNING){ break; }
			printf("WARNING: decimal number on line %d is greater or less than 255. Number translated as 255 or -255.\n", line);
			break;
		case 4:
			if(!UNKNOWN_INSTRUCTION_WARNING){ break; }
			printf("WARNING: Unknown instruction occured on line %d. Translated to NULL instruction.\n", line);
		default:
			if(!UNKNOWN_WARNING){ break; }
			printf("WARNING: Unknown warning error_num: %d, line: %d, from function: %s\n", warning, line, function);
			break;
	}
}



int binary(char * str, int line){
	int size = strlen(str);
	int res=0;
	int power=0;
	for(int i=size-1; i>1; i--){
		if(power>7){ throw_warning(0, line, "\"int binary(char * str, int line\"");  break; }
		int num = (int)(str[i]-'0');
		if(num != 0 && num != 1){ throw_error(0, line, &str[i], &i); return 0; }
		res=res+num*pow(2, power);
		power++;

	}
	return res;
}
int hex(char * str, int line){
	int size = strlen(str);
	int res = 0;
	int power=0;
	for(int i=size-1; i>1; i--){
		if(power>1){ throw_warning(1, line, "\"int hex(char * str, int line)\""); break; }
		for(int x=0; x<16; x++){
			if((int)str[i]==HEX[x][0] || (int)str[i]==HEX[x][0]-32){ res=res+x*pow(16, power); break; }
			throw_error(0, line, &str[i], &i);	return 0;
		}
		power++;
	}
	return res;
}
int dec(char * str, int line){
	int size = strlen(str);
	int res = 0;
	int power=0;
	int is_negative = 0;
	if(str[0]=='-'){ is_negative = 1; }
	for(int i=size-1; i>=is_negative; i--){
		if(power>2){ throw_warning(2, line, "\"int dec(char * str, int line\""); break; }
		int num = (int)(str[i]-'0');
		if(num > 9 || num < 0){ throw_error(0, line, &str[i], &i); return 0; }
		res=res+num*pow(10, power);
		power++;
	}
	if(res>255){ res=255; throw_warning(3, line, "\"int dec(char * str, int line\""); }
	if(is_negative){ res = -res; }
	return res;
}

int translate(char * word, int line){
	if(word[0] == ' '){ return 0;}
	int size = strlen(word);

	if((word[0]>47 && word[0]<58) || word[0]=='-'){
		if(size>2){ 
			if(word[1]=='b'){ return binary(word, line); }
			else if(word[1]=='x'){ return hex(word, line); }
		}
		return dec(word, line);
	}

	for(int i=0; i<32; i++){
		if(size==CMD[i].size){
			for(int x=0; x<=size; x++){
				if(x==size){ return i;}
				if(((int)word[x]-(int)CMD[i].CMD[x])%32==0){ continue; }
				break;
			}
		}
	}
	for(int i=0; i<11; i++){
		if(size==A[i].size){
			for(int x=0; x<=size; x++){
				if(x==size){ return i; }
				if(((int)word[x]-(int)A[i].CMD[x])%32==0){ continue; }
				break;
			}
		}
	}
	throw_warning(4, line, NULL);
	return 0;
}

bool next_word(FILE * fptr, char * output, int * line_num){
	if(fptr==NULL){ ABORT = true; printf("ERROR: File not found!\n"); return false; }
	char c;
	int i=0;
	while(true){
		c = fgetc(fptr);
		if(c==EOF){output[i]='\0'; return true; }
		if(c==' '){ output[i]='\0'; return false; }
		if(c=='\n'){ output[i]='\0'; *line_num = *line_num + 1; return false; }
		output[i]=c;	
		i++;
	}
	printf("ERROR: Unknown error on line %d in function bool next_word.\nYou aren't suppose to it tho.\n", line_num);
	ABORT = true;
	return false;
}


int main(int argc, char * argv){
	char * filename = "testcase.txt";	
	FILE * file = fopen(filename, "r");
	int CurrentLine = 0;
	if(file == NULL){ printf("ERROR: File not found!\n"); return 1; }
	char word[255];
	while(true){
		if(ABORT){ break; }
		int EndOfFile = next_word(file, word, &CurrentLine);
		printf("%s %d\n", word, translate(word, CurrentLine));
		if(EndOfFile==true){ break; }
	}
	fclose(file);
	return 0;
}
