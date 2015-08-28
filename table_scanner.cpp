#include "scanner.h"
#include "token.h"
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>

/******************************************************************************
 * Transition table scanner
 *****************************************************************************/

using namespace std;

table_scanner::table_scanner(const std::string& source_text)
  : base_t(source_text)
  , stated_t (9, { 1, 3, 5, 8 }) // trap state, final states 
  , c_transition_table { 
  //  a  b  trap
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
              }
{ }

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
    if(is_final_state(my_state))
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

int32_t table_scanner::tindex(char c)
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

tt_t table_scanner::state_to_type(int32_t state)
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