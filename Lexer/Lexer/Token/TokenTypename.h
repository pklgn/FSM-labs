#pragma once

enum class TokenTypename
{
	IDENTIFIER,
	KEYWORD,
	NUMBER,
	OCTAL,
	DECIMAL,
	HEXADECIMAL,

	// arithmetic
	ADD_OPERATOR,
	SUB_OPERATOR,
	MUL_OPERATOR,
	DIV_OPERATOR,
	EXP_OPERATOR,

	ASG_OPERATOR,

	// comparison
	COMP_OPERATOR,

	// logical
	NOT_OPERATOR,
	AND_OPERATOR,
	OR_OPERATOR,

	// string
	CNCT_OPERATOR,

	STRING,
	DELIMITER,
	COMMENTARY,

	ERROR,
};
