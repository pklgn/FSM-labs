#define CATCH_CONFIG_MAIN
#include "../Lexer/Lexer/Lexer.h"
#include <format>
#include <sstream>
#include <catch2/catch.hpp>
#undef ERROR // undefine this because catch2 uses the same name with my TokenTypename macro

class IsSameTokenMatcher : public Catch::Matchers::Impl::MatcherBase<Token>
{
public:
	IsSameTokenMatcher(const Token& expected)
		: m_expectedToken(expected)
	{
	}

	bool match(Token const& actual) const override
	{
		return actual.GetLexeme() == m_expectedToken.GetLexeme() &&
			actual.GetLine() == m_expectedToken.GetLine() &&
			actual.GetPosition() == m_expectedToken.GetPosition() &&
			actual.GetTypename() == m_expectedToken.GetTypename();
	}

	std::string describe() const override
	{
		std::ostringstream ss;
		ss << "Must be equal to the token ";
		m_expectedToken.PrintToken(ss);
		return ss.str();
	}

private:
	Token m_expectedToken;
};

IsSameTokenMatcher IsSameToken(const Token& token)
{
	return { token };
}

TEST_CASE("Check numbers")
{
	Lexer lexer;
	std::istringstream iss;

	SECTION("Check integer number")
	{
		auto programText = "1234567";
		iss.str(programText);

		lexer.Run(iss);
		
		auto numberToken = lexer.GetTokens().front();
		REQUIRE_THAT(numberToken, IsSameToken(Token(TokenTypename::NUMBER, "1234567", 1, 1)));
	}

	SECTION("Check integer number with text after")
	{
		auto programText = "1234567text";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		auto numberToken = tokens.front();
		auto textToken = tokens.back();
		REQUIRE(tokens.size() == 2);
		REQUIRE_THAT(numberToken, IsSameToken(Token(TokenTypename::NUMBER, "1234567", 1, 1)));
		REQUIRE_THAT(textToken, IsSameToken(Token(TokenTypename::IDENTIFIER, "text", 1, 8)));
	}

	SECTION("Check decimal number")
	{
		auto programText = "123.987";
		iss.str(programText);

		lexer.Run(iss);

		auto numberToken = lexer.GetTokens().front();
		REQUIRE_THAT(numberToken, IsSameToken(Token(TokenTypename::DECIMAL, "123.987", 1, 1)));
	}

	SECTION("Check decimal number with text after")
	{
		auto programText = "123.987unexpected text";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		auto numberToken = tokens.front();
		auto afterNumberToken = tokens[1];
		REQUIRE(tokens.size() == 3);
		REQUIRE_THAT(numberToken, IsSameToken(Token(TokenTypename::DECIMAL, "123.987", 1, 1)));
		REQUIRE_THAT(afterNumberToken, IsSameToken(Token(TokenTypename::IDENTIFIER, "unexpected", 1, 8)));
	}

	SECTION("Check decimal number with two dots")
	{
		auto programText = "123.987.654";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 3);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::DECIMAL, "123.987", 1, 1)));
		REQUIRE_THAT(tokens[1], IsSameToken(Token(TokenTypename::DELIMITER, ".", 1, 8)));
		REQUIRE_THAT(tokens.back(), IsSameToken(Token(TokenTypename::NUMBER, "654", 1, 9)));
	}

	SECTION("Check octal number")
	{
		auto programText = "0q7654321";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 1);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::OCTAL, "0q7654321", 1, 1)));
	}

	SECTION("Check octal number when number eight is found")
	{
		auto programText = "0q76543821";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 2);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::OCTAL, "0q76543", 1, 1)));
		REQUIRE_THAT(tokens.back(), IsSameToken(Token(TokenTypename::ERROR, "821", 1, 8)));
	}

	SECTION("Check octal number when unexpected text is found")
	{
		auto programText = "0q76543text";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 2);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::OCTAL, "0q76543", 1, 1)));
		REQUIRE_THAT(tokens.back(), IsSameToken(Token(TokenTypename::ERROR, "text", 1, 8)));
	}

	SECTION("Check hexadecimal number")
	{
		auto programText = "0habcdef1234";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 1);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::HEXADECIMAL, "0habcdef1234", 1, 1)));
	}

	SECTION("Check hexadecimal number when letter is found")
	{
		auto programText = "0habcdef1234g";
		iss.str(programText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 2);
		REQUIRE_THAT(tokens.front(), IsSameToken(Token(TokenTypename::HEXADECIMAL, "0habcdef1234", 1, 1)));
		REQUIRE_THAT(tokens.back(), IsSameToken(Token(TokenTypename::ERROR, "g", 1, 13)));
	}
}

TEST_CASE("Check all keywords and delimiters")
{
	Lexer lexer;
	std::istringstream iss;

	SECTION("Include operators that delimiters too")
	{
		auto progtamText = "DECIMAL ,FIXED\n" // 3
						   "FLOAT. READ CHAR\n" // 7
						   "DECLARE READ; WRITE\n" // 11
						   "GET: PUT =INTO\n" // 16
						   "FROM( FILE SKIP\n" // 20
						   "EDIT DISPLAY )REPLY\n" // 24
						   "BEGIN% END ENTRY\n" // 28
						   "PROCEDURE+ INITIALIZE \n" // 31
						   "GOTO- CALL DO\n" // 35
						   "EXIT IF +RETURN\n" // 39
						   "STOP ELSE \"\"THEN\n" // 43
						   "WHILE/ LIST 'TO\n"; // 48
			;
		iss.str(progtamText);

		lexer.Run(iss);

		auto tokens = lexer.GetTokens();
		REQUIRE(tokens.size() == 48);
		REQUIRE_THAT(tokens[0], IsSameToken(Token(TokenTypename::KEYWORD, "DECIMAL", 1, 1)));
		REQUIRE_THAT(tokens[1], IsSameToken(Token(TokenTypename::DELIMITER, ",", 1, 9)));
		REQUIRE_THAT(tokens[2], IsSameToken(Token(TokenTypename::KEYWORD, "FIXED", 1, 10)));
		REQUIRE_THAT(tokens[3], IsSameToken(Token(TokenTypename::KEYWORD, "FLOAT", 2, 1)));
		REQUIRE_THAT(tokens[4], IsSameToken(Token(TokenTypename::DELIMITER, ".", 2, 6)));
		REQUIRE_THAT(tokens[5], IsSameToken(Token(TokenTypename::KEYWORD, "READ", 2, 8)));
		REQUIRE_THAT(tokens[6], IsSameToken(Token(TokenTypename::KEYWORD, "CHAR", 2, 13)));
		REQUIRE_THAT(tokens[7], IsSameToken(Token(TokenTypename::KEYWORD, "DECLARE", 3, 1)));
		REQUIRE_THAT(tokens[8], IsSameToken(Token(TokenTypename::KEYWORD, "READ", 3, 9)));
		REQUIRE_THAT(tokens[9], IsSameToken(Token(TokenTypename::DELIMITER, ";", 3, 13)));
		REQUIRE_THAT(tokens[10], IsSameToken(Token(TokenTypename::KEYWORD, "WRITE", 3, 15)));
		REQUIRE_THAT(tokens[11], IsSameToken(Token(TokenTypename::KEYWORD, "GET", 4, 1)));
		REQUIRE_THAT(tokens[12], IsSameToken(Token(TokenTypename::DELIMITER, ":", 4, 4)));
		REQUIRE_THAT(tokens[13], IsSameToken(Token(TokenTypename::KEYWORD, "PUT", 4, 6)));
		REQUIRE_THAT(tokens[14], IsSameToken(Token(TokenTypename::ASG_OPERATOR, "=", 4, 10)));
		REQUIRE_THAT(tokens[15], IsSameToken(Token(TokenTypename::KEYWORD, "INTO", 4, 11)));
		REQUIRE_THAT(tokens[16], IsSameToken(Token(TokenTypename::KEYWORD, "FROM", 5, 1)));
		REQUIRE_THAT(tokens[17], IsSameToken(Token(TokenTypename::DELIMITER, "(", 5, 5)));
		REQUIRE_THAT(tokens[18], IsSameToken(Token(TokenTypename::KEYWORD, "FILE", 5, 7)));
		REQUIRE_THAT(tokens[19], IsSameToken(Token(TokenTypename::KEYWORD, "SKIP", 5, 12)));
		REQUIRE_THAT(tokens[20], IsSameToken(Token(TokenTypename::KEYWORD, "EDIT", 6, 1)));
		REQUIRE_THAT(tokens[21], IsSameToken(Token(TokenTypename::KEYWORD, "DISPLAY", 6, 6)));
		REQUIRE_THAT(tokens[22], IsSameToken(Token(TokenTypename::DELIMITER, ")", 6, 14)));
		REQUIRE_THAT(tokens[23], IsSameToken(Token(TokenTypename::KEYWORD, "REPLY", 6, 15)));
		REQUIRE_THAT(tokens[24], IsSameToken(Token(TokenTypename::KEYWORD, "BEGIN", 7, 1)));
		REQUIRE_THAT(tokens[25], IsSameToken(Token(TokenTypename::DELIMITER, "%", 7, 6)));
		REQUIRE_THAT(tokens[26], IsSameToken(Token(TokenTypename::KEYWORD, "END", 7, 8)));
		REQUIRE_THAT(tokens[27], IsSameToken(Token(TokenTypename::KEYWORD, "ENTRY", 7, 12)));
		REQUIRE_THAT(tokens[28], IsSameToken(Token(TokenTypename::KEYWORD, "PROCEDURE", 8, 1)));
		REQUIRE_THAT(tokens[29], IsSameToken(Token(TokenTypename::ADD_OPERATOR, "+", 8, 10)));
		REQUIRE_THAT(tokens[30], IsSameToken(Token(TokenTypename::KEYWORD, "INITIALIZE", 8, 12)));
		REQUIRE_THAT(tokens[31], IsSameToken(Token(TokenTypename::KEYWORD, "GOTO", 9, 1)));
		REQUIRE_THAT(tokens[32], IsSameToken(Token(TokenTypename::SUB_OPERATOR, "-", 9, 5)));
		REQUIRE_THAT(tokens[33], IsSameToken(Token(TokenTypename::KEYWORD, "CALL", 9, 7)));
		REQUIRE_THAT(tokens[34], IsSameToken(Token(TokenTypename::KEYWORD, "DO", 9, 12)));
		REQUIRE_THAT(tokens[35], IsSameToken(Token(TokenTypename::KEYWORD, "EXIT", 10, 1)));
		REQUIRE_THAT(tokens[36], IsSameToken(Token(TokenTypename::KEYWORD, "IF", 10, 6)));
		REQUIRE_THAT(tokens[37], IsSameToken(Token(TokenTypename::ADD_OPERATOR, "+", 10, 9)));
		REQUIRE_THAT(tokens[38], IsSameToken(Token(TokenTypename::KEYWORD, "RETURN", 10, 10)));
		REQUIRE_THAT(tokens[39], IsSameToken(Token(TokenTypename::KEYWORD, "STOP", 11, 1)));
		REQUIRE_THAT(tokens[40], IsSameToken(Token(TokenTypename::KEYWORD, "ELSE", 11, 6)));
		REQUIRE_THAT(tokens[41], IsSameToken(Token(TokenTypename::STRING, "", 11, 12)));
		REQUIRE_THAT(tokens[42], IsSameToken(Token(TokenTypename::KEYWORD, "THEN", 11, 13)));
		REQUIRE_THAT(tokens[43], IsSameToken(Token(TokenTypename::KEYWORD, "WHILE", 12, 1)));
		REQUIRE_THAT(tokens[44], IsSameToken(Token(TokenTypename::DIV_OPERATOR, "/", 12, 6)));
		REQUIRE_THAT(tokens[45], IsSameToken(Token(TokenTypename::KEYWORD, "LIST", 12, 8)));
		REQUIRE_THAT(tokens[46], IsSameToken(Token(TokenTypename::DELIMITER, "'", 12, 13)));
		REQUIRE_THAT(tokens[47], IsSameToken(Token(TokenTypename::KEYWORD, "TO", 12, 14)));
	}
}