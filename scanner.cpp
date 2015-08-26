#include "scanner.h"
#include "token.h"
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>

#define VEC_CONTAINS(haystack, needle) (std::find(haystack.begin(), haystack.end(), needle) != haystack.end())

using namespace std;

/******************************************************************************
 * Base scanner
 *****************************************************************************/

scanner::scanner(const std::string& source_text)
	: m_source_index(-1)
	, m_line_index(0)
	, m_source_text(source_text)
	, m_curr(0)
{

}

scanner::~scanner()
	{ }

void scanner::reset()
{
	m_source_index = -1;
	m_line_index = 0;
	m_curr = 0;
}

void scanner::cons()
{
	// the previous character was a newline, so increment line count
	if(m_curr == '\n')
	{
		m_line_index++;
		m_source_index = 0;
	}
	else
		m_source_index++;
	// are we at the end?
	if(have_reached_end())
		m_curr = '\0';
	else
		m_curr = m_source_text.at(m_source_index);
}

void scanner::uncons()
{
	if(m_source_index == 0)
		return;
	m_source_index--;
	m_curr = m_source_text.at(m_source_index);
	if(m_curr == '\n') // undo the newline if we need to
		m_line_index--;
}

const char scanner::lookahead(int32_t amount) const
{
	int32_t lookahead_index = m_source_index + amount;
	return (lookahead_index >= m_source_text.size())
		? '\0'
		: m_source_text.at(lookahead_index);
}

/******************************************************************************
 * Transition table scanner
 *****************************************************************************/

table_scanner::table_scanner(const std::string& source_text)
	: base_t(source_text)
	, c_transition_table({ 
	//	  a  b  trap?
		{ 9, 1, 9 }, // 0
		{ 2, 5, 9 }, // 1 F
		{ 4, 3, 9 }, // 2
		{ 9, 6, 9 }, // 3 F
		{ 4, 5, 9 }, // 4
		{ 9, 9, 9 }, // 5 F
		{ 7, 9, 9 }, // 6
		{ 9, 8, 9 }, // 7
		{ 9, 6, 9 }, // 8 F
		{ 9, 9, 9 }  // 9 (trap)
	})
	, m_final_states({ 1, 3, 5, 8 })
{
}
 
token table_scanner::next()
{
	token result;
	if(have_reached_end())
	{
		result.type = tt_t::eof;
		return result;
	}
	const int32_t TRAP_STATE = 9;
	int32_t my_state = 0;
	int32_t last_final_state = -1;
	int32_t last_final_state_idx = -1;
	while(true)
	{
		if(my_state == TRAP_STATE)
			break;
			
		cons();
		int32_t table_index = tindex(m_curr);
		// EOF reached
		if(have_reached_end())
			break;
		// check to see if it's an end state; this is where we do lookaheads
		if(VEC_CONTAINS(m_final_states, my_state))
		{
			last_final_state = my_state;
			last_final_state_idx = source_index();
			// check to see if the next state that we would be going to is the trap state
			// if that's the case, then it's a good chance that we're done consuming this token
			if(c_transition_table[ my_state ][ table_index ] == TRAP_STATE)
			{
				// unget the previous character and break
				uncons();
				break;
			}
			// otherwise, we keep moving forward
		}
		result.annotation += m_curr;
		my_state = c_transition_table[ my_state ][ table_index ];
	}
	
	tt_t type = tt_t::error;
	
	// todo: make this its own method rather than using gotos
state_to_type:
	switch(my_state)
	{
	case 1:
		type = tt_t::b;
		break;
	case 3:
	case 8:
		type = tt_t::bab_plus;
		break;
	case 5:
		type = tt_t::ba_star_b;
		break;
	default:
		if(last_final_state != -1) // there was in fact a previous final state, so we can reset to that 
		{
			my_state = last_final_state;
			while(source_index() != last_final_state_idx)
			{
				result.annotation.erase(result.annotation.size() - 1);
				uncons();
			}
			goto state_to_type;
		}
		break;
	}
	result.type = type;
	return result;
}

