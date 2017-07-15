#pragma once
#include "symbol.h"

class Reference : public Symbol
{
	public:
		Literal* file;
		Identifier* identifier;
		
		Reference(void);
		Reference(Literal* filepath, Identifier* identifier, YYLTYPE loc);
		
		~Reference(void);
		
		void PrintInfo(FILE* os = stdout) const;
};