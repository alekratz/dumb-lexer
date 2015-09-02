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
  if(have_reached_end())
  {
    result.type = tt_t::eof;
    return result;
  }
  const int32_t TRAP_STATE = 9;
  int32_t my_state = 0;
  int32_t last_final_state = -1;
  int32_t last_final_state_idx = -1;

  // let's just use lower case characters, mkay?
  auto ab_to_lower = [](char c) { 
    return  c == 'A' ? 'a'
          : c == 'B' ? 'b'
          : c;
  };

  while(true)
  {
    if(my_state == TRAP_STATE)
      break;
    cons();
    m_curr = ab_to_lower(m_curr);
    result.annotation += m_curr;
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
    // check to see if it's an end state; this is where we do lookaheads
    if(is_final_state(my_state))
    {
      last_final_state = my_state;
      last_final_state_idx = source_index();
    }
    // EOF reached
    if(have_reached_end())
      break;
  }
  
  tt_t type = state_to_type(my_state);
  // error? let's check to see if we had a valid final state at one point
  if(type == tt_t::error && last_final_state_idx != -1)
  {
    // set the last state back to the old final state
    my_state = last_final_state;
    // unconsume the character stream
    while(source_index() != last_final_state_idx)
    {
      result.annotation.erase(result.annotation.size() - 1);
      uncons();
    }
    // get the new type from the state
    type = state_to_type(my_state);
  }

  result.type = type;
  return result;
}

tt_t explicit_scanner::state_to_type(int32_t state)
{
  switch(state)
  {
  case 1:
    return tt_t::b;
  case 3:
  case 8:
    return tt_t::bab_plus;
  case 5:
    return tt_t::ba_star_b;
  default:
    return tt_t::error;
  }
}