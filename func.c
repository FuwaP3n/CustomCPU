#include <string.h>

typedef struct {
	char CMD[6];
	char bin[6];
	int size;
} CODES;

static CODES CMD[32] = {
{"NUL",  "00000", 3},
{"ADD",  "00001", 3},
{"SUB",  "00010", 3},
{"MUL",  "00011", 3},
{"OR",   "00100", 2},
{"NOR",  "00101", 3},
{"AND",  "00110", 3},
{"NAND", "00111", 4},
{"XOR",  "01000", 3},
{"XNOR", "01001", 4},
{"NEG",  "01010", 3},
{"<<",   "01011", 2},
{">>",   "01100", 2},
{"INC",  "01101", 3},
{">",    "01110", 1},
{"<",    "01111", 1},
{"",     "10000", 0},
{"==",   "10001", 2},
{">=",   "10010", 2},
{"<=",   "10011", 2},
{"!=",   "10100", 2},
{"DEC",  "10101", 3},
{"COPY", "10110", 4},
{"NUM",  "10111", 3},
{"JMP",  "11000", 3},
{"JMPN", "11001", 4},
{"POP",  "11010", 3},
{"PUSH", "11011", 4},
{"PUSHN","11100", 5},
{"HALT", "11101", 4},
{"",     "11110", 0},
{"",     "11111", 0}
};

CODES A[11] = {
{"NUL",  "0000", 3},
{"IO",   "0001", 2},
{"RA",   "0010", 2},
{"RB",   "0011", 2},
{"RC",   "0100", 2},
{"RD",   "0101", 2},
{"RE",   "0110", 2},
{"MEM",  "0111", 3},
{"RP",   "1000", 2},
{"IP",   "1001", 2},
{"CFLAG","1010", 5}
};

CODES get_CODES(){
	return CMD;
}

CODES get_INPUT_CODES(){
	return A;
}

char HEX[16][1] = {{'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}};

void dec_to_bin(int num, char * output){
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

int bin_to_dec(char * str){
	int size = strlen(str);
	int res=0;
	int power=0;
	for(int i=size-1; i>=0; i--){
		int num = (int)(str[i]-'0');
		res=res+(int)(num*pow(2, power));
		power++;
	}
	return res;
}

int hex_to_dec(char * str){
	int size = strlen(str);
	int res = 0;
	int power=0;
	for(int i=size-1; i>1; i--){
		//if(power>1){ break; }
		for(int x=0; x<16; x++){
			if((int)str[i]==HEX[x][0] || (int)str[i]==HEX[x][0]-32){ res=res+x*pow(16, power); break; }
			return 0;
		}
		power++;
	}
	return res;
}

int str_to_dec(char * str){
	int size = strlen(str);
	int res = 0;
	int power=0;
	int is_negative = 0;
	if(str[0]=='-'){ is_negative = 1; }
	for(int i=size-1; i>=is_negative; i--){
		//if(power>2){ break; }
		int num = (int)(str[i]-'0');
		if(num > 9 || num < 0){ return 0; }
		res=res+num*pow(10, power);
		power++;
	}
	//if(res>255){ res=255; }
	res = is_negative ? -res : res;
	//if(res < -128){ res=-128; }
	return res;
}
