#include "scanner.h"
#include "token.h"
#include <string>
#include <cstdint>

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
