# Lexical Analysis
The main task of Lexical Analyser is to generate tokens. 
Lexical Analyzer uses getNextToken( ) to extract each lexeme from the given source file and generate 
corresponding token one at a time. For each identified token an entry is made in the symbol table. 
If entry is already found in the table, then it returns the pointer to the symbol table entry for that token.

# Recursive Descent Parsers
Recursive descent is a top-down parsing technique that constructs the parse tree from the top and the input is read from left to right. 
It uses procedures for every terminal and non-terminal entity. This parsing technique recursively parses the input to make a parse tree, 
which may or may not require back-tracking. But the grammar associated with it (if not left factored) cannot avoid back-tracking. 
A form of recursive-descent parsing that does not require any back-tracking is known as predictive parsing.

# Problem Statement
A simple ‘C’ language grammar is given below. RD parser is created for the grammar and integrated it with lexical analyzer. 
Before parsing the input file, removal of ambiguity and left recursion is mandatory, if present and also left factoring is performed on grammar. 
Include the functions first(X) and follow(X).Lexical analyzer code should be included as header file in parser code. 
Parser program should make a function call getNextToken() of lexical analyze which generates a token. Parser parses it according to given grammar. 
The parser should report syntax errors if any (for e.g.: Misspelling an identifier or keyword, Undeclared or multiply declared identifier,Arithmetic or Relational 
Expressions with unbalanced parentheses and Expression syntax error etc.) with appropriate line-no.

# Lexical-Analyser-implemented-RD-Parser
Recursive Descent Parser implemented for the following grammar:

Program --> main() { declarations statement_list } <br />
identifier_list --> id idlistPrime <br />
idlistPrime --> ε | ,identifier_list | [number] idArray <br />
idArray --> ε | ,identifier_list <br />
statement_list --> statement statement_list | ε <br />
statement --> assign_stat; <br />
assign_stat --> id = expn <br />
expn --> simple_expn eprime <br />
eprime --> relop simple_expn | ε <br />
simple_expn --> term seprime <br />
seprime --> addop term seprime | ε <br />
term --> factor tprime <br />
tprime --> mulop factor seprime | ε <br />
factor --> id | num <br />
relop --> == | != | <= | >= | > | < <br />
addop --> + | - <br />
mulop --> * | / | % <br />
statement --> assign_stat; | decision_stat | looping_stat <br />
decision_stat --> if (expn) { statement_list } dprime <br />
dprime --> else { statement_list } | ε <br />
looping_stat --> while (expn) { statement_list } | for (assign_stat; expn; assign_stat) { statement_list } 

# Insights
Initially it takes a file as an input and replaces blank spaces and tabs with single space and writes the output to a temp file and 
then this temp file is serves as input to remove the preprocessor directives and is documented in another file temp2.
The temp2 file is then taken as the input file and is tokenized and stored in a dest.txt file ,between angular brackets containing the row 
number and column number for each token.



