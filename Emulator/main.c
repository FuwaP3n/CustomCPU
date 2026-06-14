#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
{"INC", "01101", 3},
{">",   "01110", 1},
{"<",   "01111", 1},
{"",    "10000", 0},
{"==",  "10001", 2},
{">=",  "10010", 2},
{"<=",  "10011", 2},
{"!=",  "10100", 2},
{"DEC", "10101", 3},
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

void to_bin(int num, char * output){
	strcpy(output, "00000000");
	if(num<0) { output[0] = '1'; num = -(num+1); }
	else if(num>127){output[0]= '1'; num = num - 128; }
	if(num>63 ){ output[1] = '1'; num = num -  64; }
	if(num>31 ){ output[2] = '1'; num = num -  32; }
	if(num>15 ){ output[3] = '1'; num = num -  16; }
	if(num>7  ){ output[4] = '1'; num = num -   8; }
	if(num>3  ){ output[5] = '1'; num = num -   4; }
	if(num>1  ){ output[6] = '1'; num = num -   2; }
	if(num>0  ){ output[7] = '1'; }
	output[8] = '\0';
}


int binary(char * str){
	int size = strlen(str);
	int res=0;
	int power=0;
	for(int i=size-1; i>=0; i--){
		int num = (int)(str[i]-'0');
		res=res+num*pow(2, power);
		power++;
	}
	return res;
}/*
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
	if(res<-128){ res=-128; throw_warning(5, line, "\"int dec(char * str, int line\""); }
	return res;
}*/


void deconstruct(char * str, int * output){
	char buff[strlen(str)+1];
	strcpy(buff, str);
	buff[5] = '\0';
	*output = binary(buff);
	strcpy(buff, &str[5]);

	if(*output==23 || *output==25 || *output==28){
		buff[8]='\0';
		*(output+1) = binary(buff);
		strcpy(buff, &str[13]);
		*(output+2) = binary(buff);
	} else {
		buff[4]='\0';
		*(output+1) = binary(buff);
		strcpy(buff, &str[9]);
		buff[3] = '\0';
		*(output+2) = binary(buff);
		strcpy(buff, &str[12]);
		*(output+3) = binary(buff);
	}
}

void reconstruct(int * str, char * out){
	strcpy(out, CMD[*str].CMD);
	out = out+strlen(out);
	out[0] = ' ';
	if(*str==23 || *str==25 || *str==28){
		char * x = malloc(5);
		sprintf(x, "%d", *(str+1));
		strcpy(&out[1], x);
		out = out+strlen(out);
		out[0] = ' ';
		strcpy(&out[1], A[*(str+2)].CMD);
		free(x);
	} else {
		strcpy(&out[1], A[*(str+1)].CMD);
		out = out+strlen(out);
		out[0] = ' ';
		strcpy(&out[1], A[*(str+2)].CMD);
		out = out+strlen(out);
		out[0] = ' ';
		strcpy(&out[1], A[*(str+3)].CMD);
	}
}


int main(void){
	FILE * CODE = fopen("code.bin", "r");
	char line[17];
	int * code_line = malloc(sizeof(int)*4);
	char * assembly_line = malloc(255);
	fgets(line, 17, CODE);
	deconstruct(line, code_line);
	reconstruct(code_line, assembly_line);
	printf("%s\n", assembly_line);
	free(code_line);
	free(assembly_line);
	fclose(CODE);
	return 0;
}
