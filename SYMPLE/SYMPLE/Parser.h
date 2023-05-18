#pragma once
#include "Lexer.h"
#include "utils.h"

#include "SymbolTable.h"

#define TABLE_SIZE_VARS 65536 // 2^20 ~ 
#define TABLE_SIZE_FUNS 65536 // 2^20 ~ 


struct Node {
	cval type = 0;
	Node* left;
	Node* right;

	cval value;
	char dType;
	cval varName;
	cval BinaryOperator;
};

struct Variable {
	cval name = 0;
	char dType = 0;
	cval value = 0;
	vector<cval>* vValue = 0;
};

class Scope {
public:
	cval name = 0;

	int scopeStart = 0;
	int scopeEnd = 0;

	int bodyStart = 0;
	int bodyEnd = 0;

	int returnAddress = 0;

	int pos = 0;

	vector<Variable*> args;
	vector<Variable*> variables;
	vector<Node*> nodes;

	Node* node;

	void peek();

	bool checkNextType(const char* c);

	bool checkNextValue(const char* c);
	bool checkCurrType(const char* c);

	void advanceBack(int length = 1);

	bool checkCurrValue(const char* c);

	void advance(int length = 1);

	int runScope();
};

class Parser {
public:
	vector<Scope*> scopes;
	vector<Token*> tokens;
	vector<Node*> nodes;

	int nodePointer;

	Parser(vector<Token*> *t);

	void advance(int i = 1);
	void advanceBack(int i = 1);

	void makeNodes();
	void printNodes(vector<Node*>* c = 0);

	Scope* makeFunction(Token* t);
	void printFunction(Scope* fn);

	void parse();


};