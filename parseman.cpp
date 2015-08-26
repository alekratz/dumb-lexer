#include "scanner.h"
#include <iostream>
#include <cstring>

using namespace std;

static void usage(int argc, char **argv);

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		usage(argc, argv);
		std::exit(1);
	}

	scanner *s;
	if(strcmp(argv[1], "table") == 0)
		s = new table_scanner(argv[2]);
	else if(strcmp(argv[1], "explicit") == 0)
		s = new explicit_scanner(argv[2]);
	else
	{
		cout << "error: unknown scanner type" << endl;
		usage(argc, argv);
		std::exit(1);
	}
	
	token result;
	do
	{
		result = s->next();
		const char *token_type_str;
		switch(result.type)
		{
		case tt_t::b:
			token_type_str = "b";
			break;
		case tt_t::bab_plus:
			token_type_str = "(bab)+";
			break;
		case tt_t::ba_star_b:
			token_type_str = "ba*b";
			break;
		case tt_t::eof:
			cout << "matched EOF, exiting" << endl;
			break;
		case tt_t::error:
			cout << "error! exiting" << endl;
			break;
		}
		
		if(!(result.type == tt_t::eof || result.type == tt_t::error))
		{
			cout << "matched pattern " << token_type_str << " with string " << result.annotation << endl;
		}
	} while(!(result.type == tt_t::eof || result.type == tt_t::error));
	
	delete s;
	return 0;
}

static void usage(int argc, char **argv)
{
	cout << "usage: " << argv[0] << " type source" << endl;
	cout << "the type argument may be either \"explicit\" or \"table\"" << endl;
}
