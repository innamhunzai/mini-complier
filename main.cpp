// LEXICAL ANALYZER BY INNAM HUNZAI

#include"resource.h"

using namespace std;

int col = 0;
int row = 0;
int TotalChar = 0;
FILE *fp;
char lookUp;
TOKEN* Token;
SymbolTable ST;
MOD mode = DECIMAL;
FILE* Target;
char* label = "LB01";
Stack S1;
int main()
{
    char fileName[50];
	cout<<"Please Enter File Name :";
	cin>>fileName;
	char fname[100];
	strcpy(fname,fileName);
	strcat(fname, "_target");
	strcat(fname,".asm");
	strcat(fileName,".txt");
	fp = fopen(fileName,"r");
	if(fp == NULL)
	{
		cout<<"ERROR! CANNOT LOAD FILE "<<fileName<<endl;
		getch();
		return 0;
	} 
	Target = fopen(fname,"w+");
		Parser();
	//ST.display();
	
	fclose(Target);
	fclose(fp);
	getch();
}

//Parser Function
void Parser()
{
    lookUp = getc(fp);
	if(lookUp == '#'){
		  if(beg_func())
				{
					lookUp = getc(fp);
					char* buff = "#mode";
					cout<<"\n"<<token_count<<".   "<<buff<<setw(50-length(buff))<<"  "<<"1";
				}
		  else
		    {Err[Err_count++] = 19;}
	token_count++;
	}
	bool flag = false;
	while(lookUp!=EOF)
	{	
		Token = NextToken();
	    switch(Token->t)
	    {
				case MODE:
				//	mode();
					break;
				case DECLAR:
					  Declar();
					  ST.GenrateDataSection(Target);
					  StatmentPart();
					  flag = true;
					break;
				case START:
					  StatmentPart();
					  flag = true;
					break;
				default:
				{
					Err[Err_count++] = 18;
			    }
					
		}
		if(flag == true)
		  break;
	}
	if(Err_count > 0)
	{
		cout<<"\n\n Compiler Error! Can not build .asm File Please Reomve following Errors";
	    int l = 0;
	    while(l < Err_count)
	    {
		   cout<<endl<<ERRORS[Err[l++]];
	    }
	    fputs("\n\nCODE GENRATION STOPPED THERE WERE ERRORS IN SOURCE FILE",Target);
    }
	else
	{
		cout<<"\n\nSuccessfully compiled without any error\n\n";
		rewind(Target);
	char ch = getc(Target);
	while(ch!=EOF)
	{
		cout<<ch;
		ch = getc(Target);
	}
	}   
}
bool beg_func()
{
	lookUp = getc(fp);
	if(lookUp == 'm')
	 { 
	  lookUp = getc(fp);
	  if(lookUp == 'o')
	  {  
		 lookUp = getc(fp);
   	     if(lookUp == 'd')
   	     {  
			lookUp = getc(fp);
	        if(lookUp == 'e')
	         {  
			    lookUp = getc(fp);
			    if(lookUp == ' '||lookUp == '\t')
 	               while(lookUp == ' '||lookUp == '\t')lookUp=getc(fp);
 	            if(lookUp == '2' || lookUp == '8')
 	            {
					switch(lookUp)
					{
						case '2':
							mode = BINARY;
							break;
						case '8':
						    mode = OCTAL;
					}
						return true;
				}
 	            else if(lookUp == '1')
 	            {
					lookUp=getc(fp);
					if(lookUp == '0' || lookUp == '6')
					{
						switch(lookUp)
					    {
					  	  case '0':
							mode = DECIMAL;
							break;
						  case '6':
						    mode = HEXADECIMAL;
					    }
					    return true;
					}
					else
					  return false;
				}
 	            else
 	               return false;
 	         }else 
 	             return false;
 	       }else
 	           return false;
 	   } 
	   else
 	      return false;
    }
	else
 	   return false;

	
}
//Scanner Functions
TOKEN* NextToken()
{
	int state = 0;
	char buffer[50];
	int k = 0;
	TOKEN* T = NULL;
	if(lookUp == EOF)
	{
		T->t = ENDOFFILE;
		return T;
	}
	if(lookUp == '\n' || lookUp == '\r' || lookUp == ' '||lookUp == '\t'||lookUp == ','||lookUp == ':')
	     while(lookUp == '\n' || lookUp == '\r'|| lookUp == ' '||lookUp == '\t'||lookUp == ','||lookUp == ':')
	     { 
			if(lookUp == '\n')
			  col++;
			lookUp = getc(fp);
		 }
	  do{
		col++;
		buffer[k++] = lookUp;
		buffer[k] = '\0';
		state = Table(state,lookUp);
		if(FinalState(state))
		{
				char lookahead = getc(fp);			
				if((lookahead == '=' && (lookUp=='='||lookUp=='<'||lookUp=='>'))||(lookahead=='+'&& lookUp=='+')||(lookahead=='-'&& lookUp=='-'))
				{
					buffer[k]=lookahead;
					buffer[k+1]='\0';	
					state = Table(state,lookahead);
				}
				else if(lookahead != EOF)
				    fseek ( fp , -1 , SEEK_CUR );
				  
				if(TableBackUp(state))
				{
					buffer[k-1]='\0';
					if(lookUp != EOF)					
					    fseek ( fp , -1 , SEEK_CUR );
				}
						
				T = CreateToken(buffer,state);
		}
		else if(state == -1)
		{
			Err[Err_count++] = 0;
			k=0;
		}
		lookUp = getc(fp);
	  }while(T == NULL);
      
      if(token_count<10)
	       cout<<"\n"<<token_count<<".   "<<T->lexeme<<setw(50-length(T->lexeme))<<"  "<<T->t;
	    else
		   cout<<"\n"<<token_count<<".  "<<T->lexeme<<setw(50-length(T->lexeme))<<"  "<<T->t;
		 token_count++;
      
	  return T;
}

TOKEN* CreateToken(char* buffer,int state)
{ 
	TOKEN* temp = new TOKEN;
	int i=0;
	switch(state)
	{
		case 2:
			strcpy(temp->lexeme,buffer);
			if(IsRword(buffer))
				temp->t = RWordType(buffer);
			else
			    temp->t = IDENTIFIER;
			break;
		case 4:
			temp->t = NUMCONST;
			strcpy(temp->lexeme,buffer);
			break;
		case 6:
			temp->t = STRINGCONST;
			strcpy(temp->lexeme,buffer);
			break;
		case 9:
			temp->t = CHARCONST;
			strcpy(temp->lexeme,buffer);
			break;
		case 13:
			temp->t = COMMENT;
			strcpy(temp->lexeme,buffer);
			break;
		case 15:
			temp->t=NE;
			strcpy(temp->lexeme,buffer);
			break;
		case 16:
			if(strcmp(buffer,"=")==0)
			  temp->t=EQUAL;
			else if(strcmp(buffer,"<")==0)
			  temp->t=LT;
			else if(strcmp(buffer,">")==0)
			  temp->t=GT;
		/*	else
			   cout<<"Comprator operator problem";  */
			strcpy(temp->lexeme,buffer);
			break;
		case 17:
			if(strcmp(buffer,"==")==0)
			  temp->t=EE;
			else if(strcmp(buffer,"<=")==0)
			  temp->t=LTE;
			else if(strcmp(buffer,">=")==0)
			  temp->t=GTE;
		/*	else
			   cout<<"Comprator operator problem";*/
			strcpy(temp->lexeme,buffer);
			break;
		case 18:
			temp->t=PLUS;
			strcpy(temp->lexeme,buffer);
			break;
		case 19:
			temp->t=INC;
			strcpy(temp->lexeme,buffer);
			break;
		case 20:
			temp->t=MINUS;
			strcpy(temp->lexeme,buffer);
			break;
		case 21:
			temp->t=DEC;
			strcpy(temp->lexeme,buffer);
			break;
		case 22:
			if(strcmp(buffer,"/")==0)
			  temp->t=DIVISION;
			else
			   temp->t=MULTIPLICATION;
			   
			strcpy(temp->lexeme,buffer);
			break;
		case 23:
			temp->t=TERMINATOR;
			temp->lexeme[0]=';';
			temp->lexeme[1]='\0';
			break;
		case 24:
			temp->t=OPENPARANTHESIS;
			strcpy(temp->lexeme,buffer);
			break;
		case 25:
			temp->t=CLOSEPARANTHESIS;
			strcpy(temp->lexeme,buffer);
			break;
		default:
		{
			cout<<"\n\nUnexpected Error occured! in creating Token Try again!";
	        getch();
		}
	}
	return temp;
}

int Table(int state, char lookUp)
{
	int col_n = 0;
	if((lookUp >=65 && lookUp<=90) || (lookUp >=97 && lookUp<=122))
		lookUp = 'L';
	else if(lookUp >=48 && lookUp<=57)
		lookUp = 'D';
	switch(lookUp)
	{
	case 'L':
		col_n = 1;
		break;
	case 'D':
		col_n = 2;
		break;
	case '"':
		col_n = 3;
	    break;
	case 39:
		col_n = 4;	
		break;
	case '{':
		col_n = 5;
		break;
	case '}':
		col_n = 6;
		break;
	case '!':
		col_n = 7;
		break;
	case '>':
		col_n = 8;
		break;
	case '<':
		col_n = 9;
		break;
	case '=':
		col_n = 10;
		break;
	case '+':
		col_n = 11;
		break;
	case '-':
		col_n = 12;
		break;
	case '*':
		col_n = 13;
		break;
	case '/':
		col_n = 14;
		break;
	case '%':
		col_n = 15;
		break;
	case ';':
		col_n = 16;
		break;
	case '(':
	    col_n = 18;
	    break;
	case ')':
		col_n = 19;
		break;
	case ',':
	    col_n = 16;
	case ' ':
	    col_n = 17;
	    break;
	default:
		col_n = 17;
	}
	
	return TT[state][col_n];
}

bool FinalState(int state)
{
	if(TT[state][20] == 51)
		return true;
	else
		return false;
}

bool TableBackUp(int state)
{
	if(TT[state][21] == 51)
		return true;
	else
		return false;
}

bool IsRword(char* word)
{
	int k =0;
	bool flag = false;
	while(k<25)
	{
		if(strcmp(word,RWords[k])==0)
		{	flag = true; break; }
		k++;
	}
	return flag;
}
int length(char* word)
{
	int i = 0;
	while(word[i]!='\0')
	 i++;
	return i;
}
TYPE RWordType(char *word)
{
	int k =0;
	bool flag = false;
	while(k<25)
	{
		if(strcmp(word,RWords[k])==0)
		{	flag = true; break;}
		k++;
	}
	switch(k)
	{
		case 0:
			return IF;
			break;
		case 1:
			return THEN;
			break;
		case 2:
			return ENDIF;
			break;
		case 3:
			return ELSE;
			break;
		case 4:
			return ENDELSE;
			break;
		case 5:
			return SWITCH;
			break;
		case 6:
			return CASSES;
			break;
		case 7:
			return VALUE;
			break;
		case 8:
			return BREAK;
			break;
		case 9:
			return REPEAT;
			break;
		case 10:
			return UNTIL;
			break;
		case 11:
			return FOR;
			break;
		case 12:
			return ENDFOR;
			break;
		case 13:
			return WHILE;
			break;
		case 14:
			return ENDWHILE;
			break;
		case 15:
			return CONST;
			break;
		case 16:
			return INT;
			break;
		case 17:
			return STRING;
			break;
		case 18:
			return CHAR;
			break;
		case 19:
			return INPUT;
			break;
		case 20:
			return OUTPUT;
			break;
		case 21:
			return START;
			break;
		case 22:
			return END;
			break;
		case 23:
			return DECLAR;
			break;
		case 24:
			return MODE;
			break;
	}
}


void Declar()
{
	Token = NextToken();
	while(true)
 	{
		if(Token->t == START)
		  break;
		else if(Token->t == ENDOFFILE){
			Err[Err_count++] = 13;
			break;
		}
		DeclStmt();		
	}
}

void DeclStmt()
{
	    TYPE temp;
		if(Token->t == INT || Token->t == CHAR || Token->t == STRING)
		{	
			temp = Token->t;
		    Token = NextToken();
			if(Token->t == IDENTIFIER)
			{	
			  while(Token->t == IDENTIFIER){
				ST.insert(temp,Token->lexeme,"0");
				Token = NextToken();
				if(Token->t == TERMINATOR)
				{
					Token = NextToken();
					if(Token->t == IDENTIFIER);
					else
					 Err[Err_count++] = 20;
				}
				else 
				  break;
		      }
		    }
			else 
			{   
				Err[Err_count++] = 14;
				Token = NextToken();
		    }     
		}
		else
		{
			Err[Err_count++] = 14;
		    Token = NextToken();
	    }
	    
	    
}

void StatmentPart() 
{
	if(Err_count == 0)
	 fputs("\n\n.CODE\n\nSTART:\n\tMOV AX, @DATA\n\tMOV DS, AX\n",Target);	
	Token = NextToken();
	while(true)
 	{
		    if(Token->t == END)
		     break;
		    else if(Token->t == ENDOFFILE)
		    {
				Err[Err_count++] = 17;
				break;
			}
			Stmt(Token->t);
    }
    if(Err_count == 0)
      fputs("\n\tMOV AX, 4C00H\n\tINT 21H\n\nEND START",Target);
    lookUp = getc(fp);
    lookUp = getc(fp);
    lookUp = getc(fp);
}

void Stmt(TYPE t)
{
	switch(t)
	{
		case IF:
			if_Stmt();
			break;
		case WHILE:
			while_Stmt();
			break;
		case FOR:
			for_Stmt();
			break;
		case IDENTIFIER:
			if(!ST.IsInSymbolTable(Token->lexeme))
			{
				Err[Err_count++] = 16;
			}
			assign_Stmt();
			break;
		case SWITCH:
			switch_Stmt();
			break;
		case REPEAT:
		   repeat_Stmt();
		   break;
		case OUTPUT:
			output_Stmt();
			break;
		case INPUT:
			input_Stmt();
			break;
		default:
			Err[Err_count++] = 22;
			Token = NextToken();	
	}
}

void if_Stmt()
{
	Token = NextToken();
	if(Token->t == OPENPARANTHESIS)
	{
		if(Err_count==0)
		 fputs("\n\tMOV AX, ",Target);
	    exp();
		if(Token->t == CLOSEPARANTHESIS)
	    {
			Token = NextToken();
			if(Token->t == THEN )
			{
                bool flag = false;
				Token = NextToken();
				while(true)
				{  
				  switch(Token->t)
	              {
		           case WHILE:
			           while_Stmt();
			           break;
		           case FOR:
			           for_Stmt();
			           break;
	 	           case IDENTIFIER:
			           if(!ST.IsInSymbolTable(Token->lexeme))
				            Err[Err_count++] = 16;
					   assign_Stmt();
			           break;
		           case REPEAT:
		               repeat_Stmt();
		               break;
	 	           case OUTPUT:
			           output_Stmt();
			           break;
	           	   case INPUT:
			           input_Stmt();
    			       break;
				   case IF:
						Err[Err_count++] = 7;
						break;
		           default: 
						Err[Err_count++] = 8;
             	  }
             	  
             	  if(Token->t == ENDIF)
				  { 
					if(Err_count==0){
					  fputs("\n  ",Target);
					  fputs(label,Target);
					  fputs(":",Target);
					}
				    Token = NextToken();
					break;
				   }
				   else if(Token->t == END || Token->t == ENDOFFILE)
				   {
						Err[Err_count++] = 10;
						break;
				   }
             	//Token = NextToken();  
			    }
			}else{
				Err[Err_count++] = 5;
			}
		}else{
			Err[Err_count++] = 1;
		}
	}
	else{
		Err[Err_count++] = 2;	
	}
}
void assign_Stmt()
{
	char* id = Token->lexeme;
	Token = NextToken();
	if(Token->t == EQUAL)
	{	
		fputs("\n\tMOV AX, ",Target);
		exp();
		if(Err_count == 0){
		fputs("\n\tMOV ",Target);
		fputs(id,Target);
		fputs(", AX",Target);
		}
	}
	else{
		Err[Err_count++] = 9;
	}
}

void input_Stmt()
{
	Token = NextToken();
	if(Token->t == IDENTIFIER)
	{
		if(Err_count == 0){
		  switch(mode)
		  {
				case BINARY:
					fputs("\n\tCALL INBIN",Target);
					break;
				case OCTAL:
					fputs("\n\tCALL INOCT",Target);
					break;
				case DECIMAL:
					fputs("\n\tCALL INDEC",Target);
					break;
				case HEXADECIMAL:
					fputs("\n\tCALL INHEX",Target);
					break;
		  }
		  fputs("\n\tMOV ",Target);
		  fputs(Token->lexeme,Target);
		  fputs(", AX",Target);
		}
		Token = NextToken();
	}
	else
	  Err[Err_count++] = 8;
	
}

void output_Stmt()
{
	
	Token = NextToken();
	if(Token->t == STRINGCONST)
	{
		if(Err_count == 0){
		  fputs("\n\tLEA DX, ",Target);
		  fputs(Token->lexeme,Target);
		  fputs("\n\tCALL MESSAGE",Target);
		}
		Token = NextToken();
	}
	else if(Token->t == IDENTIFIER)
	{
		if(Err_count == 0){
		  switch(mode)
		  {
				case BINARY:
					fputs("\n\tCALL OUTBIN",Target);
					break;
				case OCTAL:
					fputs("\n\tCALL OUTOCT",Target);
					break;
				case DECIMAL:
					fputs("\n\tCALL OUTDEC",Target);
					break;
				case HEXADECIMAL:
					fputs("\n\tCALL OUTHEX",Target);
					break;
		  }
		}
		Token = NextToken();
	}
	else
	  Err[Err_count++] = 8;		  
}

void switch_Stmt()
{
	Token = NextToken();
	if(Token->t == OPENPARANTHESIS)
	{
	    Token = NextToken();
	    if(Token->t == IDENTIFIER)
	    {
			Token = NextToken();
	        if(Token->t== CLOSEPARANTHESIS)
	        {
			   Token = NextToken();
			   while(Token->t != ENDSWITCH)
			   {
					if(Token->t == CASSES)
					{
						matching_item();
						Token = NextToken();
						if(Token->t == TERMINATOR )
						{
						   Token = NextToken(); 
						   while(Token->t != BREAK)
						   {  
						  	   Stmt(Token->t);
						       Token = NextToken();
					       }
				     	}
					}
			   }	
		    }
		}
	}
}

void for_Stmt()
{
	Token = NextToken();
	if(Token->t == OPENPARANTHESIS)
	{
		Token = NextToken();
		if(Token->t == IDENTIFIER)
		  {
				assign_Stmt();
				Token = NextToken();
				if(Token->t == TERMINATOR)
				{
					exp();
					Token = NextToken();
					if(Token->t == TERMINATOR)
					{
						exp();
						Token = NextToken();
						if(Token->t == CLOSEPARANTHESIS);
						else
						  cout<<"ERROR";
					}
				}
	      }
	}
}
void exp()
{
	Simple_exp();
	if(Token->t == LT || Token->t == GT || Token->t == LTE || Token->t == GTE||Token->t == NE||Token->t == EE)
	{
		if(Err_count == 0){	
		fputs(S1.Pop(),Target);
		fputs("\n\tCMP AX, ",Target); 
	    }
	    TOKEN *temp = Token;
		Simple_exp();
		if(Err_count == 0){
		  fputs(S1.Pop(),Target);	
		  switch(temp->t)
		  {
		 	 case LT:
		         fputs("\n\tJGE ",Target);
		         break;
			 case GT:
				 fputs("\n\tJLE ",Target);
				 break;
			 case LTE:
		         fputs("\n\tJG ",Target);
		         break;
			 case GTE:
				 fputs("\n\tJB ",Target);
				 break;
			 case NE:
		         fputs("\n\tJE ",Target);
		         break;
			 case EE:
				 fputs("\n\tJNE ",Target);
				 break;		
		  }
		  fputs(label,Target);
	    }
    }
    if(Err_count==0)
       fputs(S1.Pop(),Target);
    S1.Trash();
}

void Simple_exp()
{
	term();
	while(Token->t == PLUS || Token->t == MINUS)
	{
		TOKEN* temp = Token;
		term();
		if(Err_count == 0){
		fputs(S1.Pop(),Target);
		switch(temp->t)
		{
			case PLUS:
		         fputs("\n\tADD AX, ",Target);
		         break;
			case MINUS:
				 fputs("\n\tSUB AX, ",Target);
				 break;		
		}
	    }
	}
}

void term()
{
	factor();
	while(Token->t == MULTIPLICATION || Token->t == DIVISION)
	{
		TOKEN* temp = Token;
		factor();
		if(Err_count == 0){
		fputs(S1.Pop(),Target);
		switch(temp->t)
		{
			case MULTIPLICATION:
		         fputs("\n\tMUL ",Target);
		         break;
			case DIVISION:
				 fputs("\n\tDIV ",Target);
				 break;		
		}
	    }
	}
}

void factor()
{
	Token = NextToken();
	
	if(Token->t == OPENPARANTHESIS)
	{
		//postFix[postFixIndex++] = Token;
		exp();
		if(Token->t == CLOSEPARANTHESIS);
		else
		  	Err[Err_count++] = 1;
    }
	else if(Token->t == NUMCONST)
	{
		postFix[postFixIndex++] = Token;
	}
	else if(Token->t == IDENTIFIER){  
		  if(!ST.IsInSymbolTable(Token->lexeme)){
					Err[Err_count++] = 16;
				    ST.insert(INT , Token->lexeme,"0");
		}
		postFix[postFixIndex++] = Token;
	}
	else
	  	Err[Err_count++] = 8;
	  	
    Token = NextToken();
}

void matching_item()
{
	Token = NextToken();
	if(Token->t == CHARCONST || Token->t == NUMCONST );
	else
	 cout<<"ERROR";
}

void repeat_Stmt()
{
	if(Err_count==0){
		fputs("\n  ",Target);
		fputs(label,Target);
		fputs(":",Target);
	}
	Token = NextToken();
	while(true)
	{
		Stmt(Token->t);
		if(Token->t == UNTIL)
		{
			Token = NextToken();
			if(Token->t == OPENPARANTHESIS)
	        {
				if(Err_count==0)
		          fputs("\n\tMOV AX, ",Target);
		        exp();
	 	        if(Token->t == CLOSEPARANTHESIS)
					{
						if(Err_count==0){
		                    //fputs("\n\tMOV AX, ",Target);
		                }
						Token = NextToken();
						break;
				    }
			    else{
					Err[Err_count++] = 1;
			    }	
			}else{
				Err[Err_count++] = 2;
			}
		}
		else if(Token->t == END || Token->t == ENDOFFILE)
		{
			Err[Err_count++] = 12;
			break;
	    }
	}
}
void while_Stmt()
{
	Token = NextToken();
	if(Token->t == OPENPARANTHESIS)
	{
		if(Err_count==0){
		  fputs("\n  ",Target);
		  fputs(label,Target);
		  fputs(":",Target);
		  fputs("\n\tMOV AX,",Target);
		} 
		exp();
		if(Token->t == CLOSEPARANTHESIS)
		{
			Token = NextToken();
			while(true)
			{
				if(Token->t == ENDWHILE)
				{
					if(Err_count==0)
					{ 
					  fputs("\n\tJMP ",Target);
					  fputs(label,Target);
					  fputs("\n  ",Target);
					  fputs(label,Target);
					  fputs(":",Target);
				    }
					Token = NextToken();
					break;
				}
				else if(Token->t == END || Token->t == ENDOFFILE)
				{
					Err[Err_count++] = 11;
					break;
				}
				Stmt(Token->t);
			}
		}else{
			Err[Err_count++] = 1;
		}
	}else{
		Err[Err_count++] = 2;
	}
}
