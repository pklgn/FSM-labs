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

void Token::PrintToken(std::ostream& output) const
{
	PrintTokenTypename(output);
	output << " `" << m_lexeme
		   << "` " << m_line << ':' << m_position;
}

void Token::PrintTokenTypename(std::ostream& output) const
{
	switch (m_typename)
	{
	case TokenTypename::IDENTIFIER:
		output << "IDENTIFIER";
		break;
	case TokenTypename::KEYWORD:
		output << "KEYWORD";
		break;
	case TokenTypename::NUMBER:
		output << "NUMBER";
		break;
	case TokenTypename::OCTAL:
		output << "OCTAL";
		break;
	case TokenTypename::DECIMAL:
		output << "DECIMAL";
		break;
	case TokenTypename::HEXADECIMAL:
		output << "HEXADECIMAL";
		break;
	case TokenTypename::ADD_OPERATOR:
		output << "ADD_OPERATOR";
		break;
	case TokenTypename::SUB_OPERATOR:
		output << "SUB_OPERATOR";
		break;
	case TokenTypename::MUL_OPERATOR:
		output << "MUL_OPERATOR";
		break;
	case TokenTypename::DIV_OPERATOR:
		output << "DIV_OPERATOR";
		break;
	case TokenTypename::EXP_OPERATOR:
		output << "EXP_OPERATOR";
		break;
	case TokenTypename::ASG_OPERATOR:
		output << "ASG_OPERATOR";
		break;
	case TokenTypename::EQV_OPERATOR:
		output << "EQUAL_TO";
		break;
	case TokenTypename::NEQV_OPERATOR:
		output << "NOT_EQUAL_TO";
		break;
	case TokenTypename::LES_OPERATOR:
		output << "LESS_THAN";
		break;
	case TokenTypename::GRT_OPERATOR:
		output << "GREATER_THAN";
		break;
	case TokenTypename::LES_OR_EQV_OPERATOR:
		output << "LESS_THEN_OR_EQUAL_TO_OPERATOR";
		break;
	case TokenTypename::GRT_OR_EQV_OPERATOR:
		output << "GREATER_THEN_OR_EQUAL_TO_OPERATOR";
		break;
	case TokenTypename::AND_OPERATOR_LOGIC:
		output << "AND_OPERATOR_LOGIC";
		break;
	case TokenTypename::OR_OPERATOR_LOGIC:
		output << "OR_OPERATOR_LOGIC";
		break;
	case TokenTypename::AND_OPERATOR_BITWISE:
		output << "AND_OPERATOR_BITWISE";
		break;
	case TokenTypename::OR_OPERATOR_BITWISE:
		output << "OR_OPERATOR_BITWISE";
		break;
	case TokenTypename::STRING:
		output << "STRING";
		break;
	case TokenTypename::DELIMITER:
		output << "DELIMITER";
		break;
	case TokenTypename::COMMENTARY:
		output << "COMMENTARY";
		break;
	case TokenTypename::ERROR:
		output << "ERROR";
		break;
	default:
		break;
	}
}
