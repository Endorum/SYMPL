#include <iostream>
#include <string>
#include <vector>

#include "LEXER.h"
#include "utils.h"


using std::endl;
using std::stoi;
using std::stof;
using std::to_string;
using std::cout;
using std::vector;
using std::string;








Lexer::Lexer(std::string t) {
	text = t;
}


void Lexer::printToken(Token* token,int i) {

	

	int desiredLength = 15;

	int typeLength = (token->type).size();
	int valueLength = (token->value).size();
	string typeString = to_string(i);
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
	typeString = token->value;
	for (int i = 0; i < desiredLength - valueLength; i++) {
		typeString += ' ';
	}
	cout << typeString << endl;
	
}


bool Lexer::isWhitespace(char c) {
	if (c != 0) {
		if (c == '\n') currLine++;
		return isspace(c);
	}
	else {
		if (CHAR == '\n') currLine++;
		return isspace(CHAR);

	}
	return false;
}

bool Lexer::isKeyword(std::string s) {
	if (s == "<-")return true;
	else if (s == "->")return true;
	else if (s == "<|")return true;
	else if (s == ">|")return true;
	else if (s == "<<")return true;
	else if (s == ">>")return true;
	else if (s == "[]")return true;

	return false;
}


Token* Lexer::makeNumber() {
	std::string num_str;
	int dotCount = 0;
	while (isdigit(text[pos]) || text[pos] == '.') {
		num_str += text[pos];
		if (text[pos] == '.') dotCount++;
		pos++;
	}
	pos--;
	if (dotCount == 0) return new Token{ "CONST",num_str,'i'};
	else if (dotCount == 1) return new Token{ "CONST",num_str,'f'};
	else {
		std::cout << "Lexical Error: \n\t Invalid Float Format Error in line: " << currLine << " can't convert '" << num_str << "' to float" << '\n';
		exit(1);
	}
	return 0;
}
Token* Lexer::makeIdentifier() {
	std::string str;
	while (isalnum(text[pos]) || text[pos] == '_') {
		str += text[pos];
		pos++;
	}
	pos--;

	if (isKeyword(str)) {
		std::cout << "Lexical Error: \n\t Reserved Word Error in line: " << currLine << " the identifier '" << str << "' is already a keyword" << '\n';
		exit(1);
	}

	return new Token{ "IDENT", str };
}

Token* Lexer::makeString() {
	pos++;
	int start = pos;
	while ((text[pos] != '"') ) {
		if (pos <  text.size()) {
			pos++;
		}
		else {
			std::cout << "Lexical Error: \n\t Missing Closing Quotation Mark Error in line: " << currLine << '\n';
			exit(1);
		}
		
	}
	
	int end = pos;
	string str = text.substr(start, end - start);
	return new Token{ "CONST",str,'s' };

}

Token* Lexer::makeChar() {
	if (pos + 2 < text.size()) {
		if (text[pos] == '\'' && text[pos + 2] == '\'') {
			return new Token{ "CONST",charToString(text[pos + 1]),'c' };
		}
	}
	exit(0);
	return 0;
}

Token* Lexer::makeOperator() {


	if (CHAR == '+') return new Token{ "OPERATOR","PLUS" };
	else if (CHAR == '-') {
		if (pos + 1 < text.size()) {
			if (text[pos + 1] == '>') {
				pos++;
				return new Token{ "KEYWORD","CALL" };
			}
			else {
				return new Token{ "OPERATOR","MINUS" };
			}
		}
	}

	else if (CHAR == '*') return new Token{ "OPERATOR","MUL" };
	else if (CHAR == '/') return new Token{ "OPERATOR","DIV" };
	else if (CHAR == '%') return new Token{ "OPERATOR","MOD" };
	else if (CHAR == '&') return new Token{ "OPERATOR","AND" };
	else if (CHAR == '|') return new Token{ "OPERATOR","OR" };
	else if (CHAR == '^') return new Token{ "OPERATOR","XOR" };

	else if (CHAR == '>') {
		if (pos + 1 < text.size()) {
			if (text[pos + 1] == '|') {
				pos++;
				return new Token{ "KEYWORD","PUSH" };
			}
			else if (text[pos + 1] == '>') {
				pos++;
				return new Token{ "KEYWORD","DGREATER" };
			}
			else {
				return new Token{ "OPERATOR","GREATER" };
			}
		}
	}
	else if (CHAR == '<') {
		if (pos + 1 < text.size()) {
			if (text[pos + 1] == '-') {
				pos++;
				return new Token{ "KEYWORD","RETURN" };
			}
			else if (text[pos + 1] == '|') {
				pos++;
				return new Token{ "KEYWORD","POP" };
			}
			else if (text[pos + 1] == '<') {
				pos++;
				return new Token{ "KEYWORD","DLESS" };
			}
			else {
				return new Token{ "OPERATOR","LESS" };
			}
		}
	}
	else if (CHAR == '=') return new Token{ "OPERATOR","EQUAL" };
	else return 0;
}
Token* Lexer::makeSeperator() {
	if (CHAR == '(') return new Token{ "SEPERATOR","LPAREN" };
	else if (CHAR == ')') return new Token{ "SEPERATOR","RPAREN" };
	else if (CHAR == '[') return new Token{ "SEPERATOR","LBRACKET" };
	else if (CHAR == ']') return new Token{ "SEPERATOR","RBRACKET" };
	else if (CHAR == '{') return new Token{ "SEPERATOR","LBRACE" };
	else if (CHAR == '}') return new Token{ "SEPERATOR","RBRACE" };
	else if (CHAR == ',') return new Token{ "SEPERATOR","COMMA" };
	else if (CHAR == ':') return new Token{ "SEPERATOR","COLON" };
	else if (CHAR == ';') return new Token{ "SEPERATOR","SEMICOLON" };
	else return 0;
}

Token* Lexer::makeSpecial() {
	if (CHAR == '#') {
		if (pos + 2 < text.size()) {
			if (text[pos + 1] == '<' && text[pos + 2] == '<') {
				pos += 2;
				return new Token{ "SPECIAL" , "INCLUDE" };
			}
			else {
				return new Token{ "SPECIAL" , "NEWVAR" };
			}
		}
	}
	else if (CHAR == '@') return new Token{ "SPECIAL" , "LOOP" };
	else if (CHAR == '?') return new Token{ "SPECIAL" , "IF" };
	else if (CHAR == '~') return new Token{ "SPECIAL" , "NEWFUNC" };
	else return 0;
}

Token* Lexer::makeKeyword(string s) {
	pos++;
	if (s == "<-") return new Token{ "KEYWORD","RETURN" };
	if (s == "->") return new Token{ "KEYWORD","CALL" };
	if (s == "<|") return new Token{ "KEYWORD","POP" };
	if (s == ">|") return new Token{ "KEYWORD","PUSH" };
	if (s == "<<") return new Token{ "KEYWORD","OUTPUT" };
	if (s == ">>") return new Token{ "KEYWORD","INPUT" };
	if (s == "[]") return new Token{ "KEYWORD","ARRAY" };
}

Token* Lexer::getNextToken() {
	CHAR = text[pos];
	string word;
	word += text[pos];
	

	if (pos + 1 < text.size()) {
		word += text[pos + 1];
	}
	if (isKeyword(word)) {
		makeKeyword(word);
	}
	else if (isdigit(CHAR)) {
		return makeNumber();
	}
	else if (isalpha(CHAR) || CHAR == '_') {
		return makeIdentifier();
	}
	else if (CHAR == '"') {
		return makeString();
	}
	else if (containsStr("+-*/%<>=&|^",CHAR)) {
		return makeOperator();
	}
	else if (containsStr("(){}[],:;", CHAR)) {
		return makeSeperator();
	}
	else if (containsStr("#@?~", CHAR)) {
		return makeSpecial();
	}

	else if (pos + 2 <= text.size()) {
		if (text[pos] == '\'' && text[pos + 2] == '\'') {
			pos += 2;
			return new Token{ "CONST",charToString(text[pos - 1]),'c' };
		}
	}

	else {
		std::cout << "Lexical Error: \n\t Unrecognized Character Error in line: " << currLine << " the char '" << text[pos] << "' is not definend" << '\n';
		exit(1);
	}
}

//void Lexer::makeSymbolTable() {
//	const char* var =  {0};
//	for (int i = 0; i < tokens.size(); i++) {
//		if (tokens[i]->value == "NEWVAR") {
//			if (i + 1 < tokens.size()) {
//				if (tokens[i + 1]->type == "IDENT") {
//					var = tokens[i + 1]->value.c_str();
//					add(var);
//				}
//			}
//		}
//		else if (tokens[i]->value == "fn") {
//			if (i + 1 < tokens.size()) {
//				if (tokens[i + 1]->type == "IDENT") {
//					var = tokens[i + 1]->value.c_str();
//					add(var);
//				}
//			}
//		}
//	}
//}


void Lexer::printTokens() {
	for (int i = 0; i < tokens.size(); i++) {
		printToken(tokens[i],i);
	}
}

void Lexer::lex() {
	while (pos < text.size()) {
		CHAR = text[pos];
		if (isspace(CHAR)) {
			if (CHAR == '\n') currLine++;
			pos++;
			continue;
		}
		if (CHAR == '/') {
			if (pos + 1 < text.size()) {
				if (text[pos + 1] == '*') {

					while (!(text[pos] == '*' && text[pos+1] == '/')) {
						pos++;
					}
					continue;
				}
			}
		}


		tokens.push_back(getNextToken());
		//exit(0);
		pos++;
	}
	//makeSymbolTable();
}