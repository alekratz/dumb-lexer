#include "scanner.h"
#include "token.h"
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>

using namespace std;

explicit_scanner::explicit_scanner(const string& source_text)
  : base_t(source_text)
  , stated_t(9, { 1, 3, 5, 8 }) // trap state, final state
{ }

token explicit_scanner::next()
{
  token result;
  result.type = tt_t::error;
  bool should_scan = true;
  int32_t my_state = 0,
          last_final_state = -1;
  const int32_t TRAP_STATE = 9;

  while(should_scan)
  {
    // consume the next character
    cons();
    if(have_reached_end())
      break; // break NOW
    // let's just use lower case characters, mkay?
    auto ab_to_lower = [](char c) { 
      return  c == 'A' ? 'a'
            : c == 'B' ? 'b'
            : c;
    };
    m_curr = ab_to_lower(m_curr);
    char lahead = lookahead(); // look ahead one character from where we're at
    switch(my_state)
    {
    case 0:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = 1;
      break;

    case 1:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = 2;
      else if(m_curr == 'b')
        my_state = 5;
      break;

    case 2:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = 4;
      else if(m_curr == 'b')
        my_state = 3;
      break;

    case 3:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = 6;
      break;

    case 4:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = 4;
      else if(m_curr == 'b')
        my_state = 5;
      break;

    case 5:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = TRAP_STATE;
      break;

    case 6:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = 7;
      else if(m_curr == 'b')
        my_state = TRAP_STATE;
      break;

    case 7:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = 8;
      break;

    case 8:
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = 6;
      break;

    case 9: // trap
      my_state = TRAP_STATE;
      if(m_curr == 'a')
        my_state = TRAP_STATE;
      else if(m_curr == 'b')
        my_state = TRAP_STATE;
      break;
    }
  }

  return result;
}

bool explicit_scanner::is_final_state(int32_t state)
  { return VEC_CONTAINS(m_final_states, state); }