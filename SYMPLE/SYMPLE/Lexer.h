#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

#define MAX_NAME_SIZE 256


using std::endl;
using std::stoi;
using std::stof;
using std::cout;
using std::vector;
using std::string;



struct Token {
	string type;
	string value;
	char dType = 0;
};



class Lexer {

public:

	std::string text;

	std::vector<Token*> tokens;

	//const char* symbols[TABLE_SIZE] = { 0 };

	int pos = 0;
	int currLine = 1;

	char CHAR = 0;

	Lexer(std::string t);

	void printToken(Token* token, int i = 0);

	/*unsigned int hash(const char* v);

	void add(const char* v);

	const char* get(const char* v);

	void printSymbolTable();*/

	void printTokens();

	bool isWhitespace(char c = 0);

	bool isKeyword(std::string s);

	bool isOperator(char c = 0);

	bool isSeperator(char c = 0);

	bool isSpecial(char c = 0);

	Token* makeNumber();
	Token* makeIdentifier();

	Token* makeString();

	Token* makeChar();

	Token* makeOperator();
	Token* makeSeperator();

	Token* makeSpecial();

	Token* makeKeyword(string s);

	void makeSymbolTable();

	Token* getNextToken();


	void lex();
};