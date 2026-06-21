#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>



#define NUL 0
#define IO 1
#define RA 2
#define RB 3
#define RC 4
#define RD 5
#define RE 6
#define MEM 7
#define RP 8
#define IP 9
#define CFLAG 10

typedef struct {
	char * old_string;
	char * string;
	int offset;
} btr_string;


#define U_CODE_SIZE = 10
typedef struct {
	char name[10];
	int size;
} user_codes;

user_codes u_cmd[U_CODE_SIZE] = {
{"step",    4},
{"s",       1},
{"continue",8},
{"c",       1},
{"register",8},
{"r",       1},
{"break",   5},
{"b",       1},
{"put",     3},
{"p",       1}
};

void HELP(){
	printf("emu filename\n");
}

char HEX[16][1] = {{'0'}, {'1'}, {'2'}, {'3'}, {'4'}, {'5'}, {'6'}, {'7'}, {'8'}, {'9'}, {'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}};


struct CODES {
	char CMD[6];
	char bin[6];
	int size;
};


struct CODES CMD[32] = {
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

struct CODES A[11] = {
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


uint8_t binary(char * str){
	int size = strlen(str);
	uint8_t res=0;
	int power=0;
	for(int i=size-1; i>=0; i--){
		int num = (int)(str[i]-'0');
		res=res+(uint8_t)(num*pow(2, power));
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

bool step(uint8_t * reg, uint8_t * ram, uint8_t * u_out, uint8_t * code){
	uint8_t *a = reg+*(code+1);
	uint8_t *b = reg+*(code+2);
	uint8_t *c = reg+*(code+3);
	uint8_t rp = *(reg+RP);
	uint8_t mem = *(reg+MEM);
	uint8_t io = *(reg+IO);

	switch(*code){
		case 1:
			*c = *a + *b;
			break;
		case 2:
			*c = *a - *b;
			break;
		case 3: 
			uint8_t arg1 = *a;
			uint8_t arg2 = *b;
			for(int i = 128; i>8; i=i/2){
				if(arg1 >= i){ arg1 = arg1 - i; }
				if(arg2 >= i){ arg2 = arg2 - i; }
			}
			*c = arg1 * arg2;
			break;
		case 4:
			*c = *a | *b;
			break;
		case 5:
			*c = *a | *b;
			*c = ~*c;
			break;
		case 6:
			*c = *a & *b;
			break;
		case 7:
			*c = *a & *b;
			*c = ~*c;
			break;
		case 8:
			*c = *a ^ *b;
			break;
		case 9:
			*c = *a ^ *b;
			*c = ~*c;
			break;
		case 10:
			*c = ~*a;
			break;
		case 11:
			*c = *a << *b;
			break;
		case 12:
			*c = *a >> *b;
			break;
		case 13:
			*c = *a + 1;
			break;
		case 14:
			*(reg+CFLAG) = (*a > *b) ? 128 : 0;
			break;
		case 15:
			*(reg+CFLAG) = (*a < *b) ? 128 : 0;
			break;
		case 16:
			break;
		case 17:
			*(reg+CFLAG) = (*a == *b) ? 128 : 0;
			break;
		case 18:
			*(reg+CFLAG) = (*a >= *b) ? 128 : 0;
			break;
		case 19:
			*(reg+CFLAG) = (*a <= *b) ? 128 : 0;
			break;
		case 20:
			*(reg+CFLAG) = (*a != *b) ? 128 : 0;
			break;
		case 21:
			*c = *a - 1;
			break;
		case 22:
			*c = *a;
			break;
		case 23:
			*c = *(code+1);
			break;
		case 24:
			*(reg+IP) = (*(reg+CFLAG) == 128) ? *a : *(reg+IP);
			break;
		case 25:
			*(reg+IP) = (*(reg+CFLAG) == 128) ? *(code+1) : *(reg+IP);
			break;
		case 26:
			*(reg+RP) = *(reg+RP)-1;
			*c = *(ram+*(reg+RP));
		//	*(reg+MEM) = *c;
			break;
		case 27:
			*(ram+*(reg+RP)) = *a;
			*(reg+RP) = *(reg+RP)+1;
		//	*(reg+MEM) = *(ram+*(reg+RP));
			break;
		case 28:
			*(ram+*(reg+RP)) = *(code+1);
			*(reg+RP) = *(reg+RP)+1;
		//	*(reg+MEM) = *(ram+*(reg+RP));
			break;
		case 29: //HALT
			return true;
			break;
		default:
			break;
	}
	// If MEM or RAM had been changed
	if(rp!=*(reg+RP)){ *(reg+MEM) = *(ram+*(reg+RP)); }
	else if(mem != *(reg+MEM)){ *(ram+rp) = *(reg+MEM); }
	// If IO had been changed by CPU
	if(io!=*(reg+IO)){ *u_out = *(reg+IO); *(reg+IO)=io; }

	return false;
}

void deconstruct(char * str, uint8_t * output){
	char buff[strlen(str)+1];
	strcpy(buff, str);
	buff[5] = '\0';
	*output = binary(buff);
	strcpy(buff, &str[5]);

	if(*output==23 || *output==25 || *output==28){
		buff[8]='\0';
		*(output+1) = binary(buff);
		strcpy(buff, &str[13]);
		*(output+3) = binary(buff);
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

void reconstruct(uint8_t * str, char * out){
	strcpy(out, CMD[*str].CMD);
	out = out+strlen(out);
	out[0] = ' ';
	if(*str==23 || *str==25 || *str==28){
		char * x = malloc(5);
		sprintf(x, "%d", *(str+1));
		strcpy(&out[1], x);
		out = out+strlen(out);
		out[0] = ' ';
		strcpy(&out[1], A[*(str+3)].CMD);
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

void UI_construct(char * out, char * line, uint8_t * reg){
	sprintf(out, "%s", line);
	for(int i = 0; i < 11; i++){
		int len = strlen(out);
		out[len] = ' ';
		sprintf(&out[len+1], "%s: %d", A[i].CMD, *(reg+i));
	}
}
/*
btr_string to_btr_string(char * str){
	int size = strlen(str);
	char string1[size];
	char string2[size];
	btr_string string = {string1, string2, 0};
	strcpy(string.old_string, str);	
	strcpy(string.string, str);
	return string;
}

void user_translate(char * str, int * args){
	btr_string string = to_btr_string(str);
	for(int i=0; string.old_string[i] != '\0'; i++){
		if(string.string[i] = ' '){
			string.string[i] = '\0';
			
		}
	}
}
*/

void activate_cmd(int cmd){
	if(cmd<2){ //step
	} else if(cmd < 4){ //continue
	} else if(cmd < 6){ //register
	} else if(cmd < 8){ //break
	} else if(cmd < 10){//put
	} else { //default
	}

}

void debug_cmd(char *str){
	char * word = strtok(str, " ");
	for(int i = 0; i<U_CODE_SIZE){
		if(strlen(word)!=u_cmd[i].size){ continue; }
		if(strcmp(word, u_cmd[i].name)==0){
			activate_cmd(i);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc<2){ HELP(); return 0; }
	char filename[255];
	strcpy(filename, argv[1]);
	FILE * CODE = fopen(filename, "r");
	if(CODE==NULL){ 
		printf("File not found!\n"); 
		return 0; 
	}

	uint8_t * REGISTERS = malloc(sizeof(uint8_t)*11);
	uint8_t * RAM = malloc(sizeof(uint8_t)*64);
	*(REGISTERS+IP) = 0;
	*(REGISTERS+CFLAG) = 0;

	uint8_t * code_line = malloc(sizeof(uint8_t)*4);
	uint8_t user_output = 0;
	char * assembly_line = malloc(255);

	

	char line[25];
	char UI[256];
	bool is_halt = false;
	uint8_t ip = 0;
	char debug[255];
	while(true){
		if(is_halt){ 
			is_halt=false;  
			scanf("%s", debug);
			//user_translate();
			//user_act();
		}
		
		fgets(line, sizeof(line), CODE); //holy shit, this sequence worked
		if(feof(CODE)){ break; }
		
		line[16]='\0';
		deconstruct(line, code_line);
		reconstruct(code_line, assembly_line);
		is_halt = step(REGISTERS, RAM, &user_output, code_line);
		UI_construct(UI, assembly_line, REGISTERS);
		printf("%s\n", UI);
		if(ip!=*(REGISTERS+IP)){ 
			ip = *(REGISTERS+IP); 
			fseek(CODE, 16*ip, SEEK_SET); 
		}
		else {*(REGISTERS+IP) = *(REGISTERS+IP) + 1; ip++; }
	}
	printf("\n---Programm finished!---\n");
	free(REGISTERS);
	free(RAM);
	free(code_line);
	free(assembly_line);
	fclose(CODE);
	return 0;
}
