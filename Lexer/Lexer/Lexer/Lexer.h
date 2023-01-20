#pragma once
#include <vector>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "../Token/Token.h"

using Tokens = std::vector<Token>;

class Lexer
{
public:
	enum class State
	{
		COMMON,
		ATTRIBUTE,
		DELIMITER,
		IDENTIFIER,
		NUMBER,
		DECIMAL,
		OCTAL,
		HEXADECIMAL,
		DIVISION,
		MULTIPLICATION,
		COMPARISON,
		COMMENT,
		FINISH,
		ERROR,
	};

	Lexer();

	void Run(std::ifstream&);

	Tokens GetTokens() const;


private:
	void AppendToken(TokenTypename, const Lexeme&, size_t line, size_t pos);

	void GetChar(const std::string&);
	void ClearBuffer();
	void AppendBuffer(char);

	using LexemeIter = std::unordered_set<Lexeme>::const_iterator;
	LexemeIter FindLexeme(const std::unordered_set<Lexeme>& lexemes) const;

	State m_state = State::COMMON;
	std::string m_buffer;
	char m_char = ' ';
	double m_number;

	Tokens m_tokens;

	using Lexemes = std::unordered_set<Lexeme>;
	Lexemes m_keywords;
	Lexemes m_delimeters;

	size_t m_lineNumber = 0;
	size_t m_linePosition = 0;

};