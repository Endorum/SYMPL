

#include "utils.h"
#include "Lexer.h"
#include "Parser.h"
#include <memory>


#define TABLE_SIZE_VARS 65536 // 
#define TABLE_SIZE_FUNS 65536 //  


 
int main(int argc, const char ** argv) {
	
	Variable* variables = (Variable*)malloc(sizeof(Variable) * TABLE_SIZE_VARS);
	Scope* functions = (Scope*)malloc(sizeof(Scope) * TABLE_SIZE_FUNS);


	string text = readFile();

	Lexer lexer(text);

	lexer.lex();

	//lexer.printTokens();

	Parser parser(&lexer.tokens);

	parser.parse();

	return 0;
}