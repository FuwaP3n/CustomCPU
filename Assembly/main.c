#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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


int binary(char * str, int line){
	int size = strlen(str);
	int res=0;
	int power=0;
	for(int i=size; i>1; --i){
		if(power>7){ printf("Warning: binary number on line %d, contains more than 8bit. Readed only 8 last bits.", line); break; }
		res=res+((int)str[i]-48)*pow(2, power);
		power++;

	}
	return res;
}
int hex(char * str){}
int dec(char * str){}

int translate(char * word, int line){

	int size = strlen(word);

	if(word[0]>47 && word[0]<58){
		if(size>2){ 
			if(word[1]=='b'){ return binary(word, line); }
			else if(word[1]=='x'){ return hex(word); }
		}
		return dec(word);
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

	return 0;
}

int next_word(FILE *fptr, char * output, int * line_num){ // return codes:	0 - fine
	if(fptr==NULL){ return 1; }				// 					1 - error
	char c;											//						-1 - EOF
	int i=0;
	while(true){
		c = fgetc(fptr);
		if(c==EOF){output[i]='\0'; return -1; }
		if(c==' '){ output[i]='\0'; return 0; }
		if(c=='\n'){ output[i]='\0'; *line_num = *line_num + 1; return 0; }
		output[i]=c;	
		i++;
	}
	return 1;
}


int main(int argc, char * argv){
	char * filename = "testcase.txt";	
	FILE * file = fopen(filename, "r");
	int CurrentLine = 0;
	if(file == NULL){ printf("File not found!\n"); return 1; }
	// CODE FOR NUMBER DETECTION FOR NUM AND *N COMMANDS!!!!!!!
	char word[5];

	while(true){
		int exit_code = next_word(file, word, &CurrentLine);
		printf("%s %d\n", word, translate(word, CurrentLine));
		if(exit_code==-1){ break; }
		if(exit_code==1) { printf("something wrong!\n"); break;}
		
	}
	fclose(file);
	return 0;
}
