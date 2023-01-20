#include "../pch.h"
#include "Lexer.h"

Lexer::Lexer()
{
	m_delimeters = {
		",",
		".",
		";",
		"=",
		":",
		" ",
		"(",
		")",
		"%",
		"+",
		"-",
		"*",
		"/",
		"\"",
		"'",
	};

	m_keywords = {
		// attributes
		"DECIMAL",
		"FIXED",
		"FLOAT",
		"REAL",
		"CHAR",

		// оператор описаний
		"DECLARE",

		// ввод-вывод
		"READ",
		"WRITE",
		"GET",
		"PUT",
		"INTO",
		"FROM",

		// утверждение
		"DISPLAY",
		"REPLY",

		// операторы структуры программы
		"BEGIN",
		"END",
		"ENTRY",
		"PROCEDURE",
		"INITIALIZE",

		// операторы управления
		"CALL",
		"DO",
		"EXIT",
		"IF",
		"RETURN",
		"STOP",
		"ELSE",
		"THEN",
		"WHILE",
		"LIST",
		"TO",
	};
}

void Lexer::Run(std::ifstream& input)
{
	std::string line;
	m_lineNumber = 0;
	while (std::getline(input, line))
	{
		++m_lineNumber;
		m_char = ' ';
		m_linePosition = 0;
		m_state = State::COMMON;
		auto lineSize = line.size();
		while (m_state != State::FINISH)
		{
			if (lineSize < m_linePosition)
			{
				m_state = State::FINISH;
			}
			switch (m_state)
			{
			case Lexer::State::COMMON: {
				if (m_char == ' ' || m_char == '\t' || m_char == '\0')
				{
					GetChar(line);
				}
				else if (std::isalpha(m_char) || m_char == '_')
				{
					AppendBuffer(m_char);
					m_state = State::IDENTIFIER;
					GetChar(line);
				}
				else if (std::isdigit(m_char))
				{
					m_state = State::NUMBER;
					AppendBuffer(m_char);
					GetChar(line);
				}
				else if (m_char == '/')
				{
					m_state = State::DIVISION;
					AppendBuffer(m_char);
					GetChar(line);
				}
				else if (m_char == '*')
				{
					m_state = State::MULTIPLICATION;
					AppendBuffer(m_char);
					GetChar(line);
				}
				else if (m_char == '+')
				{
					AppendBuffer(m_char);
					AppendToken(TokenTypename::ADD_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					GetChar(line);
					m_state = State::COMMON;
				}
				else if (m_char == '-')
				{
					AppendBuffer(m_char);
					AppendToken(TokenTypename::SUB_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					GetChar(line);
					m_state = State::COMMON;
				}
				else if (m_char == '<')
				{
					AppendBuffer(m_char);
					if (m_linePosition != lineSize)
					{
						GetChar(line);
						if (m_char == '=')
						{
							AppendBuffer(m_char);
							AppendToken(TokenTypename::LES_OR_EQV_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
							ClearBuffer();
							GetChar(line);
							
						}
					}
					else
					{
						AppendToken(TokenTypename::LES_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
						ClearBuffer();
					}
					m_state = State::COMMON;
				}
				else if (m_char == '>')
				{
					AppendBuffer(m_char);
					if (m_linePosition != lineSize)
					{
						GetChar(line);
						if (m_char == '=')
						{
							AppendBuffer(m_char);
							AppendToken(TokenTypename::GRT_OR_EQV_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
							ClearBuffer();
							GetChar(line);
							
						}
					}
					else
					{
						AppendToken(TokenTypename::GRT_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
						ClearBuffer();
					}
					m_state = State::COMMON;
				}
				else if (m_char == '=')
				{
					AppendBuffer(m_char);
					if (m_linePosition != lineSize)
					{
						GetChar(line);
						if (m_char == '=')
						{
							AppendBuffer(m_char);
							AppendToken(TokenTypename::EQV_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
							ClearBuffer();
							GetChar(line);
							
						}
					}
					else
					{
						AppendToken(TokenTypename::ASG_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
						ClearBuffer();
					}
					m_state = State::COMMON;
				}
				else if (m_char == '\"')
				{
					m_state = State::STRING;
					GetChar(line);
				}
				else if (m_char == '&')
				{
					m_state = State::BITWISE_AND;
					GetChar(line);
				}
				else if (m_char == '|')
				{
					m_state = State::BITWISE_OR;
					GetChar(line);
				}
				else
				{
					m_state = State::DELIMITER;
				}
				break;
			}
			case Lexer::State::DIVISION: {
				if (m_char == '/')
				{
					AppendBuffer(m_char);
					m_state = State::COMMENT;
					GetChar(line);
					
				}
				else
				{
					AppendToken(TokenTypename::DIV_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::MULTIPLICATION: {
				if (m_char == '*')
				{
					AppendBuffer(m_char);
					AppendToken(TokenTypename::EXP_OPERATOR, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
					GetChar(line);
					
				}
				else
				{
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::DELIMITER: {
				AppendBuffer(m_char);

				auto it = FindLexeme(m_delimeters);
				if (it != m_delimeters.end())
				{
					AppendToken(TokenTypename::DELIMITER, m_buffer, m_lineNumber, m_linePosition);
					m_state = State::COMMON;
					GetChar(line);
					ClearBuffer();
				}
				else
				{
					ClearBuffer();
					m_state = State::ERROR;
				}

				break;
			}
			case Lexer::State::IDENTIFIER: {
				if (std::isalnum(m_char) || m_char == '_')
				{
					AppendBuffer(m_char);
					GetChar(line);
				}
				else
				{
					auto it = FindLexeme(m_keywords);
					if (it != m_keywords.end())
					{
						AppendToken(TokenTypename::KEYWORD, m_buffer, m_lineNumber, m_linePosition);
					}
					else
					{
						AppendToken(TokenTypename::IDENTIFIER, m_buffer, m_lineNumber, m_linePosition);
					}
					ClearBuffer();
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::NUMBER: {
				if (std::isdigit(m_char))
				{
					AppendBuffer(m_char);
					GetChar(line);
					
				}
				else if (m_char == '.')
				{
					m_state = State::DECIMAL;
					AppendBuffer(m_char);
					GetChar(line);
					
				}
				else if (m_char == 'h')
				{
					if (m_buffer == "0")
					{
						m_state = State::HEXADECIMAL;
						AppendBuffer(m_char);
						GetChar(line);
						
					}
					else
					{
						AppendBuffer(m_char);
						m_state = State::ERROR;
					}
				}
				else if (m_char == 'q')
				{
					if (m_buffer == "0")
					{
						m_state = State::OCTAL;
						AppendBuffer(m_char);
						GetChar(line);
						
					}
					else
					{
						m_state = State::ERROR;
					}
				}
				else
				{
					AppendToken(TokenTypename::NUMBER, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::DECIMAL: {
				if (std::isdigit(m_char))
				{
					AppendBuffer(m_char);
					GetChar(line);
				}
				else
				{
					AppendToken(TokenTypename::DECIMAL, m_buffer, m_lineNumber, m_linePosition);
					m_state = State::COMMON;
					ClearBuffer();
				}
				break;
			}
			case Lexer::State::OCTAL: {
				if (std::isdigit(m_char) && m_char != '9')
				{
					AppendBuffer(m_char);
					GetChar(line);
				}
				else
				{
					AppendToken(TokenTypename::OCTAL, m_buffer, m_lineNumber, m_linePosition);
					m_state = State::COMMON;
					ClearBuffer();
				}
				break;
			}
			case Lexer::State::HEXADECIMAL: {
				auto upperCh = std::toupper(m_char);
				if (std::isdigit(m_char) || upperCh == 'A' || upperCh == 'B' || upperCh == 'C' || upperCh == 'D' || upperCh == 'E' || upperCh == 'F')
				{
					AppendBuffer(m_char);
					GetChar(line);
				}
				else
				{
					AppendToken(TokenTypename::HEXADECIMAL, m_buffer, m_lineNumber, m_linePosition);
					m_state = State::COMMON;
					ClearBuffer();
				}
				break;
			}
			case Lexer::State::STRING: {
				if (m_char == '"')
				{
					AppendToken(TokenTypename::STRING, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
					GetChar(line);
				}
				else
				{
					AppendBuffer(m_char);
					GetChar(line);
				}
				break;
			}
			case Lexer::State::BITWISE_AND: {
				if (m_char == '&')
				{
					AppendToken(TokenTypename::AND_OPERATOR_LOGIC, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
					GetChar(line);
				}
				else
				{
					AppendToken(TokenTypename::AND_OPERATOR_BITWISE, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::BITWISE_OR: {
				if (m_char == '|')
				{
					AppendToken(TokenTypename::OR_OPERATOR_LOGIC, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
					GetChar(line);
				}
				else
				{
					AppendToken(TokenTypename::OR_OPERATOR_BITWISE, m_buffer, m_lineNumber, m_linePosition);
					ClearBuffer();
					m_state = State::COMMON;
				}
				break;
			}
			case Lexer::State::FINISH:
				// TODO: что-то делать
				break;
			case Lexer::State::ERROR: {
				// save buffer
				auto tempBuffer = m_buffer;
				m_buffer = m_char;
				auto it = FindLexeme(m_delimeters);
				if (it != m_delimeters.end())
				{
					AppendToken(TokenTypename::ERROR, tempBuffer, m_lineNumber, m_linePosition);
					m_state = State::COMMON;
					ClearBuffer();
				}
				else
				{
					m_buffer = tempBuffer;
					AppendBuffer(m_char);
					GetChar(line);
				}
				break;
			}
			default:
				throw std::runtime_error("Unexpected lexer state was found");
			}
		}
	}

}

std::vector<Token> Lexer::GetTokens() const
{
	return m_tokens;
}

void Lexer::PrintTokens(std::ostream& output) const
{
	for (auto&& token : m_tokens)
	{
		token.PrintToken(output);
		output << std::endl;
	}
}

void Lexer::AppendToken(TokenTypename tokenTypename, const Lexeme& lexeme, size_t line, size_t pos)
{
	m_tokens.push_back(Token(tokenTypename, lexeme, line, pos - lexeme.size()));
}

void Lexer::GetChar(const std::string& string)
{
	if (m_linePosition < string.size())
	{
		m_char = string.at(m_linePosition);
	}
	++m_linePosition;
}

void Lexer::ClearBuffer()
{
	m_buffer = "";
}

void Lexer::AppendBuffer(char ch)
{
	m_buffer += ch;
}

Lexer::LexemeIter Lexer::FindLexeme(const std::unordered_set<Lexeme>& lexemes) const
{
	return std::find(lexemes.begin(), lexemes.end(), m_buffer);
}
