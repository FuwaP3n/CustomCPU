#ifndef FUNC_H
#define FUNC_H

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


typedef struct CODES;
CODES get_CODES();
CODES get_INPUT_CODES();

void dec_to_bin(int num, char * output);
int bin_to_dec(char * str);
int hex_to_dec(char * str);
int str_to_dec(char * str);

#endif
