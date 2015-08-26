#include "scanner.h"
#include "token.h"
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>

using namespace std;

explicit_scanner::explicit_scanner(const string& source_text)
  : base_t(source_text)
{ }

token explicit_scanner::next()
{
  return token(); // fix it
}

