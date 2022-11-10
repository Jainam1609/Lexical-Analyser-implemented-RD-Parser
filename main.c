/* statement --> assign_stat; | decision_stat | looping_stat
   decision_stat --> if (expn) { statement_list } dprime
   dprime --> else { statement_list } | ε
   looping_stat --> while (expn) { statement_list } | for (assign_stat; expn; assign_stat) { statement_list }
*/
#include "lex_analyser.h"
void statement(token t)
{	if (strcmp(t.token_name,"if")==0) decision_stat();
	else if (strcmp(t.token_name,"while")==0 || strcmp(t.token_name,"for")==0) looping_stat(t);
	else
	{	assign_stat(t); token tk=getToken();
		if (strcmp(tk.token_name,";")!=0) { printf("Missing semi-colon after assignment (%d:%d)\n",t.row,t.col); exit(0); }
	}
}
void decision_stat()
{ token t=getToken();
	if (strcmp(t.token_name,"(")==0)
	{	expn(); t=getToken();
		if (strcmp(t.token_name,")")==0)
		{	t=getToken();
			if (strcmp(t.token_name,"{")==0)
			{	t=getToken(); statement_list(t); t=getToken();
				if (strcmp(t.token_name,"}")==0) dprime();
				else { printf("\'if\' body ended improperly (%d:%d)\n",t.row,t.col); exit(0); }
			}
			else { printf("Improper \'if\' body (%d:%d)\n",t.row,t.col); exit(0); }
		}
		else { printf("Missing \')\' after if condition (%d:%d)\n",t.row,t.col); exit(0); }
	}
	else { printf("Missing \'(\' after \'if\' (%d:%d)\n",t.row,t.col); exit(0); }
}
void dprime()
{	token t=getToken();
	if (strcmp(t.token_name,"else")==0)
	{	t=getToken();
		if (strcmp(t.token_name,"{")==0)
		{	t=getToken(); statement_list(t); t=getToken();
			if (strcmp(t.token_name,"}")!=0) { printf("\'else\' body ended improperly (%d:%d)\n",t.row,t.col); exit(0); }
		}
		else { printf("Improper \'else\' body (%d:%d)\n",t.row,t.col); exit(0); }
	}
	else { pos-=tlen(t); return; }
}
void looping_stat(token t)
{	if (strcmp(t.token_name,"while")==0)
	{	t=getToken();
		if (strcmp(t.token_name,"(")==0)
		{	expn(); t=getToken();
			if (strcmp(t.token_name,")")==0)
			{	t=getToken();
				if (strcmp(t.token_name,"{")==0)
				{	statement_list(getToken()); t=getToken();
					if (strcmp(t.token_name,"}")!=0) { printf("\'while\' body ended improperly (%d:%d)\n",t.row,t.col); exit(0); }
				}
				else { printf("Improper \'while\' body (%d:%d)\n",t.row,t.col); exit(0); }
			}
			else { printf("Missing \')\' after while condition (%d:%d)\n",t.row,t.col); exit(0); }
		}
		else { printf("Missing \'(\' after \'while\' (%d:%d)\n",t.row,t.col); exit(0); }
	}
	else if (strcmp(t.token_name,"for")==0)
	{	t=getToken();
		if (strcmp(t.token_name,"(")==0)
		{	assign_stat(getToken()); t=getToken();
			if (strcmp(t.token_name,";")==0)
			{	expn(); t=getToken();
				if (strcmp(t.token_name,";")==0)
				{	assign_stat(getToken()); t=getToken();
					if (strcmp(t.token_name,")")==0)
					{	t=getToken();
						if (strcmp(t.token_name,"{")==0)
						{	statement_list(getToken()); t=getToken();
							if (strcmp(t.token_name,"}")!=0) { printf("\'for\' body ended improperly (%d:%d)\n",t.row,t.col); exit(0); }
						}
						else { printf("Improper \'for\' body (%d:%d)\n",t.row,t.col); exit(0); }
					}
					else { printf("Missing \')\' after updating iteration variable (%d:%d)\n",t.row,t.col); exit(0); }
				}
				else { printf("Missing semi-colon after \'if\' condition (%d:%d)\n",t.row,t.col); exit(0); }
			}
			else { printf("Missing semi-colon after assigning iteration variable (%d:%d)\n",t.row,t.col); exit(0); }
		}
		else { printf("Missing \'(\' after \'for\' (%d:%d)\n",t.row,t.col); exit(0); }
	}
}
void main() { createSymTab(); init(); Program(); }
