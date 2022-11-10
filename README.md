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

Program --> main() { declarations statement_list }__
identifier_list --> id idlistPrime
idlistPrime --> ε | ,identifier_list | [number] idArray
idArray --> ε | ,identifier_list
statement_list --> statement statement_list | ε
statement --> assign_stat;
assign_stat --> id = expn
expn --> simple_expn eprime
eprime --> relop simple_expn | ε
simple_expn --> term seprime
seprime --> addop term seprime | ε
term --> factor tprime
tprime --> mulop factor seprime | ε
factor --> id | num
relop --> == | != | <= | >= | > | <
addop --> + | -
mulop --> * | / | %   
statement --> assign_stat; | decision_stat | looping_stat
decision_stat --> if (expn) { statement_list } dprime
dprime --> else { statement_list } | ε
looping_stat --> while (expn) { statement_list } | for (assign_stat; expn; assign_stat) { statement_list }

# Insights



