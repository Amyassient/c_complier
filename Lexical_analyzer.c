#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define BUFLEN  20
int datalen = 0; //data length in buffer
int readpos = -1; //read postion
char buffer[BUFLEN]; //buffer

//Stores recognized strings
char temp_word[20] = {0}; 
int temp_word_num = 0;
char ch;

//Count the number of words in each category 
int  ident_num = 0;
int  number_num = 0;
int  keyword_num = 0;
int  delimitter_num = 0;

//Count the number of words types in each category
int  ident_type_num = 0;
int  number_type_num = 0;
int  keyword_type_num = 0;
int  delimitter_type_num = 0;

//Keywords list
char keywords[15][10] = {"int","char","void","extern","if",
						"else","switch","case","default","while",
						"do","for","break","continue","return"};

//An array describing words
char number[50][10] = {0};
char identifie[50][10] = {0};
char key[50][10] = {0};
char delimitter[50][3] = {0};
 
//An array describing words
char number_types[50][10] = {0};
char identifie_types[50][10] = {0};
char key_types[50][10] = {0};
char delimitter_types[50][10] = {0};

//Function declaration
void is_delimiters(char ch ,FILE *fp);
void is_identifie_or_keys(char ch, FILE *fp);
void is_number(char ch, FILE *fp);
void is_invalid(char ch, FILE *fp);
char *str2upper(char *str); 
char Scanner(FILE *fp);
void written_file(FILE *f1);

char Tags[50][20] = {
	"ERR", //error
	"ID",  //identifie
	"NUM","CHAR","STR", //constantINCINC
	"KW_EXTERN", //extern
	"KW_INT","KW_CHAR","KW_VOID", //data type
	"NOT","LEA","REA", //Monocular operation ! & |
	"ADD","SUB","MUL","DIV","MOD", //Arithmetic operation + - * / %
	"INC","DEC", //++ --
	"GT","GE","LT","LE","EQU","NEQU", //compare opt > >= < <= == !=
	"AND","OR", //logic opt && ||
	"LPAREN","RPAREN", //()
	"LBRACK","RBRACK", //[]
	"LBRACE","RBRACE", //{}
	"COMMA","COLON","SEMICON", //, : ;
	"ASSIGN", // =
	"KW_IF","KW_ELSE", //if_else
	"KW_SWITCH","KW_CASE","KW_DEFAULT", //switch_case_default
	"KW_WHILE","KW_DO","KW_FOR", //loop
	"KW_BREAK","KW_CONTINUE","KW_RETURN", //break,continue,return
};
void written_file(FILE *f1){
	for(int i=0; i<20; i++){
		fprintf(f1, "%s\t%s\n",number[i],number_types[i]);
		fprintf(f1, "%s\t%s\n",identifie[i],identifie_types[i]);
		fprintf(f1, "%s\t%s\n",key[i],key_types[i]);
		fprintf(f1, "%s\t%s\n",delimitter[i],delimitter_types[i]);	
	}
	fclose(f1);
	return;
}
char* str2upper(char *str)  
{  
    if(str == NULL)
  return NULL;
    char *p = str;  
    while(*str){  
        *str = toupper(*str);  
        str++;  
    }  
    return p;  
}

void is_invalid(char ch, FILE *fp){
	if(ch == ' ' || ch == '\n'){
		ch = Scanner(fp);
	}
	return;
}
void is_delimiters(char ch ,FILE *fp){
	switch(ch){
		case '+': 
			ch = Scanner(fp);
			if(ch == '+'){
				strcpy(delimitter[delimitter_num++],"++");
				strcpy(delimitter_types[delimitter_type_num++],"INC");
			}
			else{
				strcpy(delimitter[delimitter_num++],"+");
				strcpy(delimitter_types[delimitter_type_num++],"ADD");
			}
			break;
		case '-':	
			ch = Scanner(fp);
			if(ch == '-'){ 
				strcpy(delimitter[delimitter_num++],"--");
				strcpy(delimitter_types[delimitter_type_num++],"DEC");
			}
			else{
				strcpy(delimitter[delimitter_num++],"-");
				strcpy(delimitter_types[delimitter_type_num++],"SUB");
			} 
			break;
		case '*':  
			strcpy(delimitter[delimitter_num++],"*");
			strcpy(delimitter_types[delimitter_type_num++],"MUL");
		    	break;
		case '/':  
			strcpy(delimitter[delimitter_num++],"/");
			strcpy(delimitter_types[delimitter_type_num++],"DIV");
		    	break;
		case '%': 
			strcpy(delimitter[delimitter_num++],"%");
			strcpy(delimitter_types[delimitter_type_num++],"MOD");
		    	break; 
		case '>': 
			ch = Scanner(fp);
			if(ch == '='){
				strcpy(delimitter[delimitter_num++],">=");
				strcpy(delimitter_types[delimitter_type_num++],"GE");
			}
			else{
				strcpy(delimitter[delimitter_num++],">");
				strcpy(delimitter_types[delimitter_type_num++],"GT");
			}
		    	break;
		case '<':
			ch = Scanner(fp);
			if(ch == '='){ 
				strcpy(delimitter[delimitter_num++],"<=");
				strcpy(delimitter_types[delimitter_type_num++],"LE");
			}
			else{
				strcpy(delimitter[delimitter_num++],"<");
				strcpy(delimitter_types[delimitter_type_num++],"LT");
			}
		    	break;
		case '=':
			ch = Scanner(fp);
			if(ch == '='){
				strcpy(delimitter[delimitter_num++],"==");
				strcpy(delimitter_types[delimitter_type_num++],"EQU");
			}
			else{	 
				strcpy(delimitter[delimitter_num++],"=");
				strcpy(delimitter_types[delimitter_type_num++],"ASSIGN");
		    }
				break;
		case '&': 
			ch = Scanner(fp);
			if(ch == '&'){
				strcpy(delimitter[delimitter_num++],"&&");
				strcpy(delimitter_types[delimitter_type_num++],"AND");
			}
			else{
				strcpy(delimitter[delimitter_num++],"&");
				strcpy(delimitter_types[delimitter_type_num++],"LEA");
			}
		    	break;
		case '|':
			ch = Scanner(fp);
			if(ch == '|'){
				strcpy(delimitter[delimitter_num++],"||");
				strcpy(delimitter_types[delimitter_type_num++],"OR");
			}
			else{ 
				strcpy(delimitter[delimitter_num++],"|");
				strcpy(delimitter_types[delimitter_type_num++],"REA");
			}
		    	break;
		case '!':
			ch = Scanner(fp);
			if(ch == '='){
				strcpy(delimitter[delimitter_num++],"!=");
				strcpy(delimitter_types[delimitter_type_num++],"NEQU");
			} 
				strcpy(delimitter[delimitter_num++],"!");
				strcpy(delimitter_types[delimitter_type_num++],"MUL");
			break;
		case ',': 
			strcpy(delimitter[delimitter_num++],",");
			strcpy(delimitter_types[delimitter_type_num++],"COMMA");
		    	break;
		case ':': 
			strcpy(delimitter[delimitter_num++],":");
			strcpy(delimitter_types[delimitter_type_num++],"COLON");
		    	break;
		case ';': 
			strcpy(delimitter[delimitter_num++],";");
			strcpy(delimitter_types[delimitter_type_num++],"SEMICON");
		    	break;
		case '(': 
			strcpy(delimitter[delimitter_num++],"(");
			strcpy(delimitter_types[delimitter_type_num++],"LPAREN");
		    	break;
		case ')': 
			strcpy(delimitter[delimitter_num++],")");
			strcpy(delimitter_types[delimitter_type_num++],"RPAREN");
		    	break;
		case '[': 
			strcpy(delimitter[delimitter_num++],"[");
			strcpy(delimitter_types[delimitter_type_num++],"LBRACK");
		    	break;
		case ']': 
			strcpy(delimitter[delimitter_num++],"]");
			strcpy(delimitter_types[delimitter_type_num++],"RBRACK");
		    	break;
		case '{': 
			strcpy(delimitter[delimitter_num++],"{");
			strcpy(delimitter_types[delimitter_type_num++],"LBRACE");
		    	break;
		case '}': 
			strcpy(delimitter[delimitter_num++],"}");
			strcpy(delimitter_types[delimitter_type_num++],"RBRACE");
		    	break;
		default:  break;
	}
	return;
}
void is_identifie_or_keys(char ch, FILE *fp){
	if(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch == '_'){
		do{
			
			temp_word[temp_word_num++] = ch; 
			ch = Scanner(fp);
        }
		while(ch == '_' || ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9');
		//keywords
		for(int i=0; i<15; i++){
			if(strcmp(keywords[i],temp_word) == 0){
				strcpy(key[keyword_num++],temp_word);
				char temp[20] = "KW_"; 
				strcat(temp,str2upper(temp_word));
				strcpy(key_types[keyword_type_num++],temp);
				temp_word_num = 0;
				memset(temp_word,0,sizeof(temp_word));
				return;
			}				
		}
		//identifie
		strcpy(identifie[ident_num++],temp_word);
		strcpy(identifie_types[ident_type_num++],"ID");
		temp_word_num = 0;
		memset(temp_word,0,sizeof(temp_word));
	}
	return;
}

void is_number(char ch, FILE *fp){
	if(ch>='0' && ch <= '9'){
		do{
			temp_word[temp_word_num++] = ch;
			ch = Scanner(fp);
		}
		while(ch >= '0' && ch<='9' );
		strcpy(number[number_num++],temp_word);
		strcpy(number_types[number_type_num++],"NUM");
		temp_word_num = 0;
		memset(temp_word,0,sizeof(temp_word));
	}
	return;
}
char Scanner(FILE *fp){
	if(!fp)
		return -1;
	if(readpos == datalen-1){
		datalen = fread(buffer,1,BUFLEN,fp);
		if(datalen == 0){
			datalen = 1;
			buffer[0] = -1;
		}
		readpos = -1;
	}
	readpos++;
	char ch = buffer[readpos];
	if(ch == -1){
		fclose(fp);
	}
	return ch;	
}

int main(){
	FILE *fp = fopen("test.txt","r");
	FILE *f1 = fopen("token.txt","w");
	//ch = Scanner(fp);
	do{ 
		ch = Scanner(fp);
		is_invalid(ch, fp);
		is_identifie_or_keys(ch, fp);
		is_number(ch,fp);
		is_delimiters(ch, fp);
		if(ch == '\0') break;
	}
	while(1);	
	written_file(f1);	
	return 0;
}



