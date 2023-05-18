#include "Lexer.h"
#include "utils.h"
#include "Parser.h"

#include "SymbolTable.h"

#define TABLE_SIZE_VARS 65536 // 2^20 ~ 
#define TABLE_SIZE_FUNS 65536 // 2^20 ~  

Parser::Parser(vector<Token*> * t) {
	tokens = *t;
	nodePointer = 0;
}


/*

struct Node {
	cval type = 0;
	char dType = 0;
	cval value = 0;
	vector<cval>* vValue = 0;
	vector<Node*>* children = 0;
};


*/



void Parser::makeNodes() {
	for (int i = 0; i < tokens.size(); i++) {
		Node* node = new Node;
		node->type = strToCVAL(tokens[i]->type);
		node->value = strToCVAL(tokens[i]->value);
		if (tokens[i]->type == "IDENT") {
			node->varName = strToCVAL(tokens[i]->value);
			//node->value = 0;
		}
		else if (tokens[i]->type == "OPERATOR") {
			node->BinaryOperator = strToCVAL(tokens[i]->value);
			//node->value = 0;
		}
		node->dType = tokens[i]->dType;

		nodes.push_back(node);
	}
}

void Parser::printFunction(Scope* fn) {
	cout << "---------------------------------------------------\n";

	cout << "name:" << fn->name << endl;
	cout << "\nStart: " << fn->bodyStart << " End: " << fn->scopeEnd << " Return to: " << fn->returnAddress << endl;
	cout << "args: " << endl;
	for (int i = 0; i < fn->args.size(); i++) {
		cout << fn->args[i]->name << " ";

		if (fn->args[i]->value) {
			cout << ":" << fn->args[i]->value << " ";
		}
	}
	cout << "\n\nBody: \n" << endl;
	printNodes(&fn->nodes);



}

Scope* Parser::makeFunction(Token* t) {
	Scope* fn = new Scope{ strToCVAL(t->value) };
	Variable* arg = new Variable;

	fn->scopeStart = nodePointer + 1;
	while (tokens[nodePointer]->value != "LBRACE") {
		nodePointer++;
		if (tokens[nodePointer]->type == "IDENT") {
			arg = new Variable{ strToCVAL(nodes[nodePointer]->value),nodes[nodePointer]->dType,"0" };
			fn->args.push_back(arg);
		}
	}
	fn->bodyStart = nodePointer + 1;

	int scopes = 1;
	while (!(scopes == 0)) {

		if (nodePointer < nodes.size()) {
			nodePointer++;
		}
		else {
			// error
			exit(1);
		}
			
		if (tokens[nodePointer]->value == "LBRACE") scopes++;
		if (tokens[nodePointer]->value == "RBRACE") scopes--;


		fn->nodes.push_back(nodes[nodePointer]);
	}
	fn->nodes.pop_back();

	fn->bodyEnd = nodePointer-1;
	fn->scopeEnd = nodePointer-1;

	return fn;
}

void Parser::printNodes(vector<Node*> * c) {
	if (c) {
		for (int i = 0; i < (*c).size(); i++) {
			Token* token = new Token{ (*c)[i]->type,(*c)[i]->value,(*c)[i]->dType };
			int desiredLength = 15;

			int typeLength = (token->type).size();
			int valueLength = (token->value).size();
			string typeString = std::to_string(i);
			typeString += "    ";
			for (int i = 0; i < 4 - typeString.size(); i++) {
				typeString.pop_back();
			}
			cout << typeString << ": ";

			typeString = token->type;
			for (int i = 0; i < desiredLength - typeLength; i++) {
				typeString += ' ';
			}
			cout << typeString << ": ";
			typeString = token->dType + token->value;
			for (int i = 0; i < desiredLength - valueLength; i++) {
				typeString += ' ';
			}
			cout << typeString << endl;

		}
	}
	else {
		for (int i = 0; i < nodes.size(); i++) {
			Token* token = new Token{ nodes[i]->type,nodes[i]->value,nodes[i]->dType };
			int desiredLength = 15;

			int typeLength = (token->type).size();
			int valueLength = (token->value).size();
			string typeString = std::to_string(i);
			typeString += "    ";
			for (int i = 0; i < 4 - typeString.size(); i++) {
				typeString.pop_back();
			}
			cout << typeString << ": ";

			typeString = token->type;
			for (int i = 0; i < desiredLength - typeLength; i++) {
				typeString += ' ';
			}
			cout << typeString << ": ";
			typeString = token->dType + token->value;
			for (int i = 0; i < desiredLength - valueLength; i++) {
				typeString += ' ';
			}
			cout << typeString << endl;

		}
	}
}



int Scope::runScope() {
	int pos = 0;
	
	Node* node = new Node;
	while (pos < nodes.size()) {
		peek();
		cout << "scope: " << name << " ::" << node->type << "-" << node->value << endl;

		int startPos = pos;
		int prevPos = pos;
		
		if (checkCurrValue("NEWVAR") && checkNextType("IDENT")) {
			
		}

		pos++;
	}

	return 0;
}



void Parser::parse() {

	makeNodes();


	while (nodePointer < tokens.size()) {
		
		if (tokens[nodePointer]->value == "NEWFUNC") {
			scopes.push_back( makeFunction(tokens[nodePointer +1]) );
		}
		

		nodePointer++;
	}

	for (int i = 0; i < scopes.size(); i++) {
		printFunction(scopes[i]);
	}

	for (int i = 0; i < scopes.size(); i++) {
		if (cmp(scopes[i]->name, "main")) {
			scopes[i]->runScope();
		}
	}
}



void Scope::peek() {
	node = nodes[pos];
}

bool Scope::checkNextType(const char* c) {
	if (pos + 1 < nodes.size()) {
		pos++;
		peek();
		return cmp(node->type, c);
	}
	return 0;
}

bool Scope::checkNextValue(const char* c) {
	if (pos + 1 < nodes.size()) {
		pos++;
		peek();
		return cmp(node->value, c);
	}
	return 0;
}
bool Scope::checkCurrType(const char* c) {
	peek();
	return cmp(node->type, c);

}

void Scope::advanceBack(int length) {
	if (pos - length >= 0) {
		pos -= length;
		peek();
	}
	else {
		cout << "advanced to much back" << pos << endl;
		exit(0);
	}
}

bool Scope::checkCurrValue(const char* c) {
	peek();
	return cmp(node->value, c);
}

void Scope::advance(int length) {
	if (pos + length < nodes.size()) {
		pos += length;
		peek();
	}
	else {
		cout << "advanced to much forward" << pos << endl;
		exit(0);
	}
}