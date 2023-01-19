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
		"->",
		"=>",
	};

	m_keywords = {
		// attributes
		"DECIMAL",
		"FIXED",
		"FLOAT",
		"REAL",

		// оператор описаний
		"DECLARE",

		// ввод-вывод
		"READ",
		"WRITE",
		"GET",
		"PUT",

		// утверждение
		"DISPLAY",
		"REPLY",

		// операторы структуры программы
		"BEGIN",
		"END",
		"ENTRY",
		"PROCEDURE",

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
	};
}

void Lexer::Run(std::ifstream& input)
{
	std::string line;
	while (std::getline(input, line))
	{
		auto lineSize = line.size();
		switch (m_state)
		{
		case Lexer::State::COMMON:
			if (m_char == ' ' || m_char == '\t' || m_char == '\0')
			{
				GetChar(line, ++m_linePosition);
			}
			else if (std::isalpha(m_char))
			{
				ClearBuffer();
				AppendBuffer(m_char);
				m_state = State::IDENTIFIER;
				GetChar(line, ++m_linePosition);
			}
			else if (std::isdigit(m_char))
			{
				m_number = m_char - '0';
				m_state = State::NUMBER;
				GetChar(line, ++m_linePosition);
			}
			else if (m_char == '/')
			{
				m_state = State::COMMENT;
				GetChar(line, ++m_linePosition);
			}
			else
			{
				m_state = State::DELIMITER;
			}
			break;
		case Lexer::State::ATTRIBUTE:
			break;
		case Lexer::State::DELIMITER:
			break;
		case Lexer::State::IDENTIFIER:
			if (std::isalnum(m_char))
			{
				AppendBuffer(m_char);
				GetChar(line, ++m_linePosition);
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
				m_state = State::COMMON;
			}
			break;
		case Lexer::State::NUMBER:
			if (std::isdigit(m_char))
			{
				m_number = m_number * 10 + (m_char - '0');
				GetChar(line, ++m_linePosition);
			}
			else if (m_char == '.')
			{
				m_state = State::DECIMAL;
				GetChar(line, ++m_linePosition);
			}
			else if (m_char == 'h')
			{
				// TODO: переделать определение типа числа в его начале, а не конце
			}
			break;
		case Lexer::State::COMMENT:
			break;
		case Lexer::State::FINISH:
			break;
		default:
			break;
		}
	}

}

std::vector<Token> Lexer::GetTokens() const
{
	return m_tokens;
}

void Lexer::AppendToken(TokenTypename tokenTypename, const Lexeme& lexeme, size_t line, size_t pos)
{
	m_tokens.push_back(Token(tokenTypename, lexeme, line, pos));
}

char Lexer::GetChar(const std::string& string, size_t pos)
{
	return string.at(pos);
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
