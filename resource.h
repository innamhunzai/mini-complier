#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<string.h>
#include<iomanip>

enum TYPE{
	       IF,THEN,ENDIF,ELSE,ENDELSE,SWITCH,ENDSWITCH,CASSES,VALUE,BREAK,REPEAT,UNTIL,FOR,ENDFOR,WHILE,ENDWHILE,
		   DECLAR,
	       CONST,INT,STRING,CHAR,INPUT,OUTPUT,
		   START,END,MODE,
		   IDENTIFIER,NUMCONST,CHARCONST,STRINGCONST,
		   PLUS,INC,MINUS,DEC,MULTIPLICATION,DIVISION,EQUAL,
		   LT,GT,LTE,GTE,NE,EE,
		   COMMENT,COLON,
		   OPENPARANTHESIS,CLOSEPARANTHESIS,
		   TERMINATOR,ENDOFFILE
};
enum MOD{
	BINARY,
	OCTAL,
	DECIMAL,
	HEXADECIMAL
};
struct TOKEN{
    TYPE t;
	char lexeme[20];
};
          
int TT[26][22]={ 
	          //{ S, L, D, ", ', {, }, !, >, <, =, +, -, *, /, %, ;,ot, (, ),Ac,Bu}
	            { 0, 1, 3, 5, 7,10,-1,14,16,16,16,18,20,22,22,22,23,-1,24,25,50,50},
				{ 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,50,50},
				{ 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,51},
				{ 3,-1, 3,-1,-1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,50,50},
				{ 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,51},
				{ 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,50,50},
				{ 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{ 7, 8, 8, 8,-1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,50,50},
				{ 8,-1,-1,-1, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,50,50},
				{ 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,50,50},
				{11,11,11,11,11,11,11,11,11,11,11,11,11,12,11,11,11,11,11,11,50,50},
				{12,11,11,11,11,11,13,11,11,11,11,11,11,12,11,11,11,11,11,11,50,50},
				{13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{14,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,50,50},
				{15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{16,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,19,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{19,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,21,-1,-1,-1,-1,-1,-1,-1,51,50},
				{21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{24,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50},
				{25,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,50}
               };

const char* RWords[] = {
	                  "if","then","endif","else","endelse","switch","casses","value","break",
					  "repeat","until","for","endfor","while","endwhile","const","int","string","char",
					  "input","output","start","end","declaration","mode"
                    };
//-------------------------------------- Variables and classes for Error Handling -------------------------                   
const char* ERRORS[] = {
	"100: Parser Error! Unidentified Character!",//0
	"200: Syntax Error! Expected ')' symbol",//1
	"201: Syntax Error! Expected '(' symbol",//2
	"202: Syntax Error! Unresolved expression",//3
	"203: Syntax Error! Unidedentify token",//4
	"204: Syntax Error! Expected 'THEN' after 'IF'",//5
	"205: Syntax Error! Expected ENDIF",//6
	"206: Syntax Error! Nested IF is not allowed",//7
	"207: Syntax Error! Can not resolve the expression",//8
	"208: Syntax Error! Expected '=' in assignment statment",//9
	"209: Syntax Error! expected 'endif' not found",//10
	"210: Syntax Error! expected 'endwhile' not found",//11
	"211: Syntax Error! expected 'until' not found for repeat statment",//12
	"300: Fatal Error! No starting point is found",//13
	"212: Syntax Error! undefined datatype",//14
	"301: Redefinition of identifier",//15
	"302: Undeclared Variable",//16
	"212: Expected EOF at the end of file", //17
	"303: Unexpected Token before declaration or main function", //18
	"001: Please follow syntax for selecting mode '#mode 2|8|10|16'",  //19
	"213: Expected Identifier after comma", //20
	"214: Unexpected Operator", //21
	"215: Ambigious Statment"   //22
	};
int Err[100]; 
int Err_count = 0;
int token_count = 0;

class entry{
	public:
	TYPE t;
	char value[50];
	char name[50];
	entry* next;
	entry()
	{
		 strcpy(name,"\0");
		 strcpy(value,"\0");
	  	 next = NULL;
	}
};
////////////////////////////////////////////SYMBOL TABLE/////////////////////////////////////////////////
class SymbolTable{
	entry* head;
	public:
		SymbolTable()
		{
			head = NULL;
		}
		void insert(TYPE temp_type, char *temp_name, char *temp_value)
		{			
			if(IsInSymbolTable(temp_name))
			{
				Err[Err_count++] = 15;
			}
			else
			{
			 entry* temp = new entry;
			 strcpy(temp->name, temp_name);
			 strcpy(temp->value, temp_value);
			 temp->t = temp_type;
			 if(head == NULL)
			 {
				head = temp; 
			 }
			 else{
				temp->next = head;
				head = temp;
			 }
		    }	
		}
		bool IsInSymbolTable(char temp_name[])
		{
			bool flag = false;
			if(head == NULL)
			  return false;
			else{
				entry* temp = head;
				while(temp != NULL)
				{
					if(strcmp(temp_name,temp->name)==0)
					{
						flag = true;
						break;
					}
					temp = temp->next;
				}	
				return flag;
			}
		}
		void GenrateDataSection(FILE* file)
		{
			
			fputs("; CODE IS GENRATED BY MINI COMPILER.\n",file);
			fputs("; Developed By: Innam Hunzai.\n",file);
			fputs("; 19 May, 2013.\n",file);
			fputs("; COMSATS University of Science and technolgy Islamabad.\n",file);
			fputs(";------------------------------------------------------------------\n\n",file);
			fputs(".MODEL SMALL\n",file);
			fputs("extrn INDEC:far,OUTDEC:far,INBIN:far,OUTBIN:far,INOCT:far,OUTOCT:far,INHEX:far,OUTHEX:far,NEWLINE:far,MESSAGE:far\n",file);
            fputs(".STACK 250H\n",file);
            fputs(".DATA\n",file);
            
            if(head == NULL);
			else{
				entry* temp = head;
				while(temp!=NULL)
				{
					if(temp->t==STRING)
					{
						fputs("   ",file);
						fputs(temp->name,file);
						fputs("  db  ",file);
						fputs(temp->value,file);
						fputs("\n",file);
					}
					else
					{
					    fputs("   ",file);
					    fputs(temp->name,file);
					    fputs("  dw  ?\n",file);
				    }
					temp = temp->next;
				}
			}

		}
		void display()
		{
			std::cout<<"\n\nSYMBOL TABLE ENTRIES\n";
			if(head == NULL)
			 std::cout<<"\nSYMBOL TABLE IS EMPTY";
			else{
				entry* temp = head;
				while(temp!=NULL)
				{
					std::cout<<temp->name<<"\n";
					temp = temp->next;
				}
			}
		}
};

//-------- POST FIX CONVERSION REQUIRED VARIABLES AND CLASSES
TOKEN*  postFix[50];
int postFixIndex = 0;
class Stack{
	TOKEN* arr[50];
	int top;
	
	public:
		Stack()
		{
			top=-1;
		}
		void Push(TOKEN* temp)
		{
			if(top >= 49)
			{
				std::cout<<"\n\nCompiler Error: Stack Overflow";
				getch();
			}
			else{
			    top++;
			    arr[top] = temp;	
			}			
		}
		TOKEN* Pop()
		{
			if(top < 0)
			{
				return "\0";
			}
			else
			  return arr[top--];
		}
		void Trash()
		{
			top = -1;
		}
};

void Parser();
TOKEN* NextToken();
int Table(int state, char lookUp);
TOKEN* CreateToken(char* buffer,int state);
bool FinalState(int state);
bool TableBackUp(int state);
bool IsRword(char* word);
int length(char*);
bool beg_func();
TYPE RWordType(char *word);

/*      -------------------------------- PARSER FUNCTIONS ------------------------------------ */
void Declar();
void DeclStmt();
void StatmentPart();
void Stmt(TYPE t);
void if_Stmt();
void assign_Stmt();
void input_Stmt();
void output_Stmt();
void switch_Stmt();
void for_Stmt();
void repeat_Stmt();
void while_Stmt();
void exp();
void Simple_exp();
void matching_item();
void factor();
void term();
