#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
typedef struct { unsigned int row,col; char token_name[100]; } token;
typedef struct { char lexeme[100],DType[10],RType[10],size[2]; } entry;
typedef struct { entry e,local[50]; } global;
entry SymTab[100]; global globe[20]; int r=1,c=1,count=0,num=0; //No. of symbol table entries & preprocessor directives
void delPreproc(FILE *fptr1,char src[],FILE *fptr2)
{ int p=0,c=0; char ch=fgetc(fptr1);
	while(ch!=EOF)
	{ p++; ch=fgetc(fptr1);
		if (ch=='m')
		{ ch=fgetc(fptr1);
			if (ch=='a')
			{ ch=fgetc(fptr1);
				if (ch=='i')
				{ ch=fgetc(fptr1);
					if (ch=='n') break;
					else continue;
				}
				else continue;
			}
			else continue;
		}
		else continue;
	}
	fclose(fptr1); fptr1=fopen(src,"r"); ch=fgetc(fptr1);
	while (c<p)
	{ c++;
		if (ch=='#') { num++; while (ch!='\n') ch=fgetc(fptr1); }
		fputc(ch,fptr2); ch=fgetc(fptr1);
	}
	while (ch!=EOF) { fputc(ch,fptr2); ch=fgetc(fptr1); }
}
void delwsComments(FILE *fptr1,FILE *fptr2)
{ char ch=fgetc(fptr1),ch2;
	for (int i=0;i<num;i++) ch=fgetc(fptr1); //To remove blank lines left after removing preprocessor directives
	while(ch!=EOF)
	{ if (ch==' ') //Space
		{ fputc(ch,fptr2);
			do
			{ ch=fgetc(fptr1); } while(ch=='	' || ch==' ');
			continue;
		}
		else if (ch=='	') //Tab space
		{ fputc(' ',fptr2);
			do
			{ ch=fgetc(fptr1); } while(ch=='	' || ch==' ');
			continue;
		}
		else if (ch=='/')
		{ ch2=fgetc(fptr1);
			if (ch2=='/') //Single line comments
			{ while(ch!='\n') ch=fgetc(fptr1);
				fputc('\n',fptr2);
			}
			else if (ch2=='*') //Multi line comments
			{ do
				{ while (ch!='*') ch=fgetc(fptr1);
					ch=fgetc(fptr1);
				} while (ch!='/');
				ch=fgetc(fptr1);
			}
			else { fputc(ch,fptr2); fputc(ch2,fptr2); }
		}
		else fputc(ch,fptr2);
		ch=fgetc(fptr1);
	}
}
void printToken(token t,FILE *fptr)
{ char buf[128];
	sprintf(buf,"<%s,%d,%d>",t.token_name,t.row,t.col);
	fputs(buf,fptr);
}
char keywords[32][10]={"auto","double","int","struct","break","else","long","switch","case","enum","register",
"typedef","char","extern","return","union","const","float","short","unsigned","continue","for","signed","void",
"default","goto","sizeof","volatile","do","if","static","while"};
char symbols[]={'=','<','>','!','+','-','*','/','%','(',')','{','}','[',']',',',';'};
char ipop[10][9]={"printf","scanf","gets","puts","getline","putline","fgetc","fputc","fgets","fputs"};
int compKey(char buf[])
{	for(int i=0;i<32;i++) if(strcmp(buf,keywords[i])==0) return 1;
  return 0;
}
int compSym(char ch)
{	for(int i=0;i<17;i++) if(ch==symbols[i]) return 1;
  return 0;
}
int compIPOP(char buf[])
{	for(int i=0;i<10;i++) if(strcmp(buf,ipop[i])==0) return 1;
  return 0;
}
void getNextToken(FILE *fptr1,FILE *fptr2)
{ int i; char ch=fgetc(fptr1),buf[128],ch2; token t,t1,t2;
	while (ch!=EOF)
	{ i=0; buf[0]='\0'; c++;
		if (ch=='\n') { r++; c=1; ch=fgetc(fptr1); continue; } //To modify row and col
		else if (ch==' ') { ch=fgetc(fptr1); continue; }
		else if (compSym(ch)==1) //Relational and arithmetic operators, and brackets, comma, and semi-colon
		{ buf[i++]=ch; ch2=fgetc(fptr1);
			if (ch2=='='||(ch=='+'&&ch2=='+')||(ch=='-'&&ch2=='-')) buf[i++]=ch2;
			buf[i]='\0'; t.row=r; t.col=(--c); strcpy(t.token_name,buf);
			printToken(t,fptr2); c+=strlen(buf); ch=ch2;
			if (strcmp(buf,"!=")==0) ch=fgetc(fptr1);
			continue;
		}
		else if (ch=='|' || ch=='&') //Logical operators
		{ buf[i++]=ch; ch=fgetc(fptr1);
			if (ch=='|' || ch=='&') buf[i++]=ch;
		}
		else if (ch=='\"') //String literals
		{ do { buf[i++]=ch; ch=fgetc(fptr1); } while (ch!='\"');
			buf[i++]=ch;
		}
		else if (isdigit(ch)) //Numeric constants
		{ buf[i++]=ch; ch2=fgetc(fptr1);
			while (isdigit(ch2)) { buf[i++]=ch2; ch2=fgetc(fptr1); }
			if (ch2=='.') do { buf[i++]=ch2; ch2=fgetc(fptr1); } while (isdigit(ch2));
			if (ch2=='E' || ch2=='e')
			{ buf[i++]=ch2; ch2=fgetc(fptr1);
				if (ch2=='+' || ch2=='-') { buf[i++]=ch2; ch2=fgetc(fptr1); }
				while (isdigit(ch2)) { buf[i++]=ch2; ch2=fgetc(fptr1); }
			}
			buf[i]='\0'; t.row=r; t.col=(--c); strcpy(t.token_name,buf);
			printToken(t,fptr2); c+=strlen(buf); ch=ch2; continue;
		}
		else if (isalpha(ch) || ch=='_') //Identifiers and Keywords
		{ buf[i++]=ch; ch2=fgetc(fptr1);
			while (isalnum(ch2) || ch2=='_') { buf[i++]=ch2; ch2=fgetc(fptr1); }
			buf[i]='\0';
			if (compKey(buf)==1) //Keyword
			{ t1.row=r; t1.col=(--c); strcpy(t1.token_name,buf); printToken(t1,fptr2); }
			else //Identifier
			{ t2.row=r; t2.col=(--c); strcpy(t2.token_name,buf); printToken(t2,fptr2);
				strcpy(SymTab[count].lexeme,buf); char str[2];
				if (compIPOP(buf)==1) { c+=strlen(buf); ch=ch2; continue; }
				else if (ch2=='(') //Function
				{ strcpy(SymTab[count].DType,"Func"); strcpy(SymTab[count].RType,t1.token_name);
					strcpy(SymTab[count].size,"-");
				}
				else //Variable
				{ if (strcmp(t1.token_name,"double")==0) { sprintf(str,"%ld",sizeof(double)); strcpy(SymTab[count].size,str); }
					else if (strcmp(t1.token_name,"int")==0) { sprintf(str,"%ld",sizeof(int)); strcpy(SymTab[count].size,str); }
					else if (strcmp(t1.token_name,"long")==0) { sprintf(str,"%ld",sizeof(long)); strcpy(SymTab[count].size,str); }
					else if (strcmp(t1.token_name,"char")==0) { sprintf(str,"%ld",sizeof(char)); strcpy(SymTab[count].size,str); }
					else if (strcmp(t1.token_name,"float")==0) { sprintf(str,"%ld",sizeof(float)); strcpy(SymTab[count].size,str); }
					else if (strcmp(t1.token_name,"short")==0) { sprintf(str,"%ld",sizeof(short)); strcpy(SymTab[count].size,str); }
					else { c+=strlen(buf); ch=ch2; continue; } //to avoid cases like "return sum;" being added to SymTab
					strcpy(SymTab[count].DType,t1.token_name); strcpy(SymTab[count].RType,"-");
				}
				count++;
			}
			c+=strlen(buf); ch=ch2; continue;
		}
		else buf[i++]=ch; //Special characters
		buf[i]='\0'; t.row=r; t.col=(--c); strcpy(t.token_name,buf);
		printToken(t,fptr2); c+=strlen(buf); ch=fgetc(fptr1);
	}
}
void copy(entry *dest,entry *src)
{ strcpy(dest->lexeme,src->lexeme); strcpy(dest->size,src->size);
	strcpy(dest->DType,src->DType); strcpy(dest->RType,src->RType);
}
void display(entry ent) { printf("%s\t%s\t%s\t%s\n",ent.lexeme,ent.DType,ent.size,ent.RType); }
void createSymTab()
{ FILE *fptr1,*fptr2; char file[100]; int i,j,k,l,f[20],ct=0,hf,flag;
	printf("Enter source code file name: "); scanf("%s",file);
	fptr1=fopen(file,"r"); fptr2=fopen("temp.txt","w");
	if (fptr1==NULL) { printf("File doesn\'t exist\n"); exit(0); }
	delPreproc(fptr1,file,fptr2); fclose(fptr1); fclose(fptr2);
	fptr1=fopen("temp.txt","r"); fptr2=fopen("temp2.txt","w");
	delwsComments(fptr1,fptr2); fclose(fptr1); fclose(fptr2);
	fptr1=fopen("temp2.txt","r"); fptr2=fopen("dest.txt","w");
	getNextToken(fptr1,fptr2); fclose(fptr1); fclose(fptr2);
	printf("Tokens generated in dest.txt\n");
	/*for (i=0;i<=count-1;i++) //Remove function calls
	{ for (j=i+1;j<=count;j++)
		{ if (strcmp(SymTab[i].DType,"Func")==0 && strcmp(SymTab[j].DType,"Func")==0)
			{ if (strcmp(SymTab[i].lexeme,SymTab[j].lexeme)==0)
				{ for (k=j;k<count-1;k++) copy(&SymTab[k],&SymTab[k+1]);
					count--;
				} } } }
	for (i=0;i<count;i++) { if (strcmp(SymTab[i].DType,"Func")==0) { f[ct++]=i; }}//To get no. and posns. of funcs
	for (k=0;k<ct;k++) //Remove duplicates within a function
	{ if ((k+1)==ct) f[k+1]=count;
		for (i=f[k];i<f[k+1]-1;i++)
		{ for (j=i+1;j<f[k+1];j++) //Check within an individual function
			{ if (strcmp(SymTab[i].RType,"-")==0 && strcmp(SymTab[j].RType,"-")==0)
				{ if (strcmp(SymTab[i].lexeme,SymTab[j].lexeme)==0)
					{ for (l=j;l<count-1;l++) copy(&SymTab[l],&SymTab[l+1]);
						count--;
					} } } } }
	ct=0;
	for (i=0;i<count;i++) if (strcmp(SymTab[i].DType,"Func")==0) f[ct++]=i; //To refresh no. and posns. of funcs.
	f[ct]=count;
	for (i=0;i<ct;i++)
	{ copy(&globe[i].e,&SymTab[f[i]]); //Creating global symbol table
		for (j=0;j<50;j++) strcpy(globe[i].local[j].lexeme,"-"); //Initializing all local entries first
		for (j=f[i]+1;j<f[i+1];j++) //Creating respective local symbol tables
		{	hf=0; flag=0;
			for (k=0;k<strlen(SymTab[j].lexeme);k++) hf+=(SymTab[j].lexeme[k]-'0');
			hf%=50;
			if (strcmp(globe[i].local[hf].lexeme,"-")==0) { copy(&globe[i].local[hf],&SymTab[j]); continue; }
			else
			{ for (k=hf+1;k<50;k++)
				{	if (strcmp(globe[i].local[k].lexeme,"-")==0)
					{	copy(&globe[i].local[k],&SymTab[j]); flag=1; break;
					} } }
			if (flag==0)
			{ for (k=hf-1;k>=0;k--)
				{	if (strcmp(globe[i].local[k].lexeme,"-")==0)
					{	copy(&globe[i].local[k],&SymTab[j]); flag=1; break;
					} } }
			if (flag==0) printf("%s couldn\'t be inserted\n",SymTab[j].lexeme);
		}
	}
	printf("\nGlobal Symbol Table:\nLexeme\tDType\tSize\tRType\n");
	for (i=0;i<ct;i++) display(globe[i].e);
	for (i=0;i<ct;i++)
	{ printf("\nLocal Symbol Table for %s():\nLexeme\tDType\tSize\tRType\n",globe[i].e.lexeme);
		for (j=0;j<50;j++)
		{ if (strcmp(globe[i].local[j].lexeme,"-")==0) continue;
			else display(globe[i].local[j]);
		}
	}*/
}
void Program(),declarations(),data_type(token t),expn(),eprime(),simple_expn(),seprime(),term(),tprime(),factor(),decision_stat(),dprime();
void statement_list(token t),statement(token t),assign_stat(token t),relop(token t),addop(token t),mulop(token t),looping_stat(token t);
token identifier_list(),idlistPrime(),idArray();
int size,pos=0; //to get position for getToken()
void init()
{ FILE *fptr; fptr=fopen("dest.txt","r"); fseek(fptr,0,SEEK_END);
	size=ftell(fptr); fclose(fptr);
}
char dt[10][10]={"double","int","struct","long","char","const","float","short","unsigned","signed"};
int compDT(char buf[100])
{ for (int i=0;i<10;i++) if (strcmp(buf,dt[i])==0) return 1;
	return 0;
}
int compNum(char buf[100])
{ int i=0;
	if (isdigit(buf[i]))
	{ i++;
	 	while (isdigit(buf[i])) i++;
		if (buf[i]=='.')
		{ i++;
			if (!isdigit(buf[i])) return 0;
			else while (isdigit(buf[i])) i++;
		}
		if (buf[i]=='E' || buf[i]=='e')
		{ i++;
			if (!isdigit(buf[i]) && buf[i]!='+' && buf[i]!='-') return 0;
			else i++;
		}
		while (isdigit(buf[i])) i++;
		return 1;
	}
	return 0;
}
int compID(char buf[100])
{ int i=0;
	if (isalpha(buf[i]) || buf[i]=='_')
	{ i++;
		while (i<strlen(buf))
		{ if (isalnum(buf[i]) || buf[i]=='_') i++;
			else return 0;
		}
		return (!compKey(buf));
	}
	return 0;
}
token getToken()
{	if (pos==size-2) { printf("Missing closing braces\n"); exit(0); } //size-1 for Ubuntu
	token t; char ch,buf[100]; int i=0; FILE *fptr;
	fptr=fopen("dest.txt","r"); fseek(fptr,pos,SEEK_SET); ch=fgetc(fptr); ch=fgetc(fptr);
	if (ch==',') { buf[i++]=ch; ch=fgetc(fptr); }
	else while (ch!=',') { buf[i++]=ch; ch=fgetc(fptr); }
	buf[i]='\0'; pos+=strlen(buf); strcpy(t.token_name,buf); i=0; buf[i]='\0';
	while ((ch=fgetc(fptr))!=',') buf[i++]=ch;
	buf[i]='\0'; pos+=strlen(buf); t.row=atoi(buf); i=0; buf[i]='\0';
	while ((ch=fgetc(fptr))!='>') buf[i++]=ch;
	buf[i]='\0'; pos+=strlen(buf); t.col=atoi(buf); pos+=4; //for the 2 commas and 2 angular brackets skipped
  return t;
}
int tlen(token t)
{	int len=strlen(t.token_name),rw=t.row,cl=t.col;
	while (rw>0) { len++; rw/=10; }
	while (cl>0) { len++; cl/=10; }
	return (len+4); //for the 2 commas and 2 angular brackets
}
void Program()
{ token t=getToken(); t=getToken(); //to get to the 'main' after 'void'
	if (strcmp(t.token_name,"main")==0)
	{ t=getToken();
		if (strcmp(t.token_name,"(")==0)
		{ t=getToken();
			if (strcmp(t.token_name,")")==0)
			{ t=getToken();
				if (strcmp(t.token_name,"{")==0)
				{ declarations(); t=getToken(); statement_list(t); t=getToken();
					if (strcmp(t.token_name,"}")==0) printf("Valid program\n");
					else printf("Illegal symbol encountered (%d:%d)\n",t.row,t.col);
					exit(0);
				}
				else { printf("Missing opening braces (%d:%d)\n",t.row,t.col); exit(0); }
			}
			else { printf("Missing \')\' after main (%d:%d)\n",t.row,t.col); exit(0); }
		}
		else { printf("Missing \'(\' after main (%d:%d)\n",t.row,t.col); exit(0); }
	}
	else { printf("No main() declared (%d:%d)\n",t.row,t.col); exit(0); }
}
void declarations()
{ token t=getToken();
	if (!compDT(t.token_name)) { pos-=tlen(t); return; } //if identifier, move on to assignment
	data_type(t); t=identifier_list();
	if (strcmp(t.token_name,";")==0) declarations();
	else { printf("Missing semi-colon after declaration\n"); exit(0); }
}
void data_type(token t) { if (!compDT(t.token_name)) { printf("Incorrect data type (%d:%d)\n",t.row,t.col); exit(0); } }
token identifier_list()
{ token t=getToken();
	if (!compID(t.token_name)) { printf("Invalid identifier in variables list (%d:%d)\n",t.row,t.col); exit(0); }
	else
	{ t=idlistPrime(); //t.row>r || t.col>c for Ubuntu
		if (t.row==0 || t.col==0) { pos-=8; t=idlistPrime(); } //account for data corruption
		return t;
	}
}
char op[11][3]={"==","!=","<=",">=",">","<","+","-","*","/","%"};
char rop[6][3]={"==","!=","<=",">=",">","<"};
char bnl[3][7]={"if","for","while"};
int compOP(char buf[100])
{	for (int i=0;i<11;i++) if (strcmp(buf,op[i])==0) return 1;
	return 0;
}
int compRop(char buf[100])
{	for (int i=0;i<6;i++) if (strcmp(buf,rop[i])==0) return 1;
	return 0;
}
int compBnL(char buf[100])
{	for (int i=0;i<3;i++) if (strcmp(buf,bnl[i])==0) return 1;
	return 0;
}
token idlistPrime()
{ token t=getToken();
	if (strcmp(t.token_name,",")==0) identifier_list();
	else if (strcmp(t.token_name,"[")==0)
	{	t=getToken();
		if (!compNum(t.token_name)) { printf("Invalid array parameter (%d:%d)\n",t.row,t.col); exit(0); }
		else
		{	t=getToken();
			if (strcmp(t.token_name,"]")==0) idArray();
			else { printf("Missing \']\' after array parameter (%d:%d)\n",t.row,t.col); exit(0); }
		}
	}
	else return t;
}
token idArray()
{	token t=getToken();
	if (strcmp(t.token_name,",")==0) identifier_list();
	else return t;
}
void statement_list(token t)
{ statement(t); token tk=getToken();
	if (compID(tk.token_name) || compBnL(tk.token_name)) statement_list(tk);
	else { pos-=tlen(tk); return; }
}
void assign_stat(token t)
{ token tk=getToken();
	if (strcmp(tk.token_name,"=")==0) expn();
	else { printf("Improper assignment (%d:%d)\n",tk.row,tk.col); exit(0); }
}
void expn() { simple_expn(); eprime(); }
void eprime()
{ token t=getToken();
	if (!compOP(t.token_name)) { pos-=tlen(t); return; }
	relop(t); simple_expn();
}
void simple_expn() { term(); seprime(); }
void seprime()
{ token t=getToken();
	if (compRop(t.token_name) || !compOP(t.token_name)) { pos-=tlen(t); return; }
	addop(t); term(); seprime();
}
void term() { factor(); tprime(); }
void tprime()
{	token t=getToken();
	if (compRop(t.token_name) || !compOP(t.token_name)) { pos-=tlen(t); return; }
	mulop(t); factor(); seprime();
}
void factor()
{	token t=getToken();
	if (!compID(t.token_name) && !compNum(t.token_name)) { printf("Invalid value assigned (%d:%d)\n",t.row,t.col); exit(0); }
}
void relop(token t) { if (!compRop(t.token_name)) { printf("Invalid relational operator (%d:%d)\n",t.row,t.col); exit(0); } }
void addop(token t)
{ if (strcmp(t.token_name,"+")!=0 && strcmp(t.token_name,"-")!=0)
	{ printf("Addition/subtraction operator expected (%d:%d)\n",t.row,t.col); exit(0); }
}
void mulop(token t)
{	if (strcmp(t.token_name,"*")!=0 && strcmp(t.token_name,"/")!=0 && strcmp(t.token_name,"%")!=0)
	{ printf("Multiplication/division/modulus operator expected (%d:%d)\n",t.row,t.col); exit(0); }
}
