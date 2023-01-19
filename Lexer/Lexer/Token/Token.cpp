#include "../pch.h"
#include "Token.h"

Token::Token(TokenTypename tokenTypename, const Lexeme& lexeme, size_t line, size_t pos)
	: m_typename(tokenTypename)
	, m_lexeme(lexeme)
	, m_line(line)
	, m_position(pos)
{
}

TokenTypename Token::GetTypename() const
{
	return m_typename;
}

Lexeme Token::GetLexeme() const
{
	return m_lexeme;
}

size_t Token::GetLine() const
{
	return m_line;
}

size_t Token::GetPosition() const
{
	return m_position;
}
