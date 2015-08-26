#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include "token.h"

class scanner
{
	/* typedefs */
private:
	typedef scanner this_t;
	/* ctor/dtor */
public:
	scanner(const std::string& source_text);
	~scanner();

	/* operations */
public:
	virtual token next() = 0;
  /**
   * Resets the lexer to start from the beginning again.
   */
	void reset();

	/**
	 * Gets whether or not we've reached the end of the source text
	 */
	const bool have_reached_end() const 
		{ return m_source_index >= static_cast<int32_t>(m_source_text.size()); }
	
protected:
	/**
	 * Consume the next character
	 */
	void cons();
	/**
	 * Unconsume the current character. This goes backwards in the stream,
	 * undoing source and line indexing along the way.
	 */
	void uncons();
	
	/**
	 * Looks ahead amount number of characters. Default is 1.
	 */
	const char lookahead(int32_t amount = 1) const;

	/* properties */
public:
	const int64_t source_index() const { return m_source_index; }
	const int64_t line_index() const { return m_line_index; }

	/* member variables */
protected:
	char m_curr;

private:
	int64_t m_source_index;
	int64_t m_line_index;
	std::string m_source_text;
};

/**
 * Transition state table implementation of the scanner
 */
class table_scanner :
	public scanner
{
	/* typedefs */
private:
	typedef scanner base_t;
	typedef table_scanner this_t;
	/* ctor/dtor */
public:
	table_scanner(const std::string& source_text);
	~table_scanner()
		{ }
	/* operations */
public:
	virtual token next();
private:
	/**
	 * Gets the table index of the given character
	 */
	int32_t tindex(char c)
	{
		switch(c)
		{
		case 'a':
		case 'A':
			return 0;
		case 'b':
		case 'B':
			return 1;
		default:
			return 2;
		}
	}

private:
	const int32_t c_transition_table[10][3];
	std::vector<int32_t> m_final_states;
};

/**
 * Explicit implementation of the scanner
 */

class explicit_scanner :
	public scanner
{
	/* typedefs */
private:
	typedef scanner base_t;
	typedef explicit_scanner this_t;
	/* ctor/dtor */
public:
	explicit_scanner(const std::string& source_text);
	~explicit_scanner()
		{ }
}
