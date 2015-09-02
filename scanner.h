#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <initializer_list>
#include "token.h"
#include "util.h"

/**
 * Abstract class that gives the quality of states
 */
class stated
{
public:
	stated(int32_t trap_state, std::initializer_list<int32_t> final_states) 
		: m_trap_state(trap_state)
		, m_final_states(final_states)
		{ }

	~stated() { }
protected:
	bool is_final_state(int32_t state)
		{ return VEC_CONTAINS(m_final_states, state); }
	bool is_trap_state(int32_t state)
		{ return m_trap_state == state; } // TODO: maybe make there be many trap states?
		
	virtual tt_t state_to_type(int32_t state) = 0;
	
private: // aka "readonly"
	int32_t m_trap_state;
	std::vector<int32_t> m_final_states;
};

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
class table_scanner
	: public scanner
	, protected stated
{
	/* typedefs */
private:
	typedef scanner base_t;
	typedef stated stated_t;
	typedef table_scanner this_t;
	/* ctor/dtor */
public:
	table_scanner(const std::string& source_text);
	~table_scanner()
		{ }
	/* operations */
public:
	virtual token next();

protected:
	virtual tt_t state_to_type(int32_t state);

	/* properties */
private:
	/**
	 * Gets the table index of the given character
	 */
	int32_t tindex(char c);

private:
	const int32_t c_transition_table[10][3];
};

/**
 * Explicit implementation of the scanner
 */

class explicit_scanner 
	: public scanner
	, protected stated
{
	/* typedefs */
private:
	typedef scanner base_t;
	typedef stated stated_t;
	typedef explicit_scanner this_t;
	/* ctor/dtor */
public:
	explicit_scanner(const std::string& source_text);
	~explicit_scanner()
		{ }
	/* operations */
public:
	virtual token next();
protected:
	virtual tt_t state_to_type(int32_t state);

};
