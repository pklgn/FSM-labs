#pragma once
#include <string>
#include "TokenTypename.h"

using Lexeme = std::string;

class Token
{
public:
	Token(TokenTypename, const Lexeme&, size_t line, size_t pos);
	
	TokenTypename GetTypename() const;
	Lexeme GetLexeme() const;
	size_t GetLine() const;
	size_t GetPosition() const;

private:
	TokenTypename m_typename;
	Lexeme m_lexeme;
	size_t m_line;
	size_t m_position;
};