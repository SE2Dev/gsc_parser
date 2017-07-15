#pragma once
#include "symbol.h"

class Identifier : public Symbol
{
	public:
		char* value;
		
		Identifier(void);
		Identifier(char* str);
		Identifier(char* str, YYLTYPE loc);
		
		virtual ~Identifier(void);
		
		void PrintInfo(FILE* os = stdout) const;
		void PrintSymbol(FILE* os = stdout) const;
};