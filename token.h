#pragma once

#include <string>

enum class token_type
{
	b,
	bab_plus,
	ba_star_b,
	eof,
	error
};

typedef token_type tt_t; // token_type type

class token
{
public:
	token() { }
	~token() { }

public:
	tt_t type;
	std::string annotation;
};
