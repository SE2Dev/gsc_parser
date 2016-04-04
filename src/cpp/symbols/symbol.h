#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string.h>

#include "../parser/gsc.tab.hpp"

#include "operator_enum.h"
#include "symbol_enum.h"
#include "../util/location.h"
#include "../util/llist.h"

//
// A symbol consists of symbol meta-data
// and also represent's a linked-list of symbols
// for each child, a new list is defined
//
class Symbol : public LList<Symbol>
{
	protected:
		SYMBOL_TYPE type;
	
		Symbol* parent;
		
		Symbol* prev;
		Symbol* next;
		
		//
		// A list of children for this symbol
		//
		Symbol* children;
		
		Range location;		
		
	public:
		Symbol(void);
		Symbol(YYLTYPE loc);
		
		~Symbol();
		
		void AddChild(Symbol* child);
		virtual void PrintInfo();
		void PrintInfoRecursive(int indentLevel = 0);
		
		void _debug_override_type(SYMBOL_TYPE type);
};

#include "expression.h"

//
// A Group consists of multiple child symbols
// it is generally used to define scopes, etc.
//
class Group : public Symbol
{
public:
	Group(Symbol* childList, YYLTYPE range);	
	~Group(void);
};

class String : public Symbol
{
	public:
		const char* value;
		
		String(void);
		String(char* str);
		String(char* str, YYLTYPE loc);
		
		~String(void);
		
		void PrintInfo();
};

class Include : public Symbol
{
	public:
		String* file;
		
		Include(void);
		Include(String* filepath, YYLTYPE loc);
		
		~Include(void);
		
		void PrintInfo();
};

class Animtree : public Symbol
{
	public:
		String* string;
		
		Animtree(void);
		Animtree(String* animtree, YYLTYPE loc);
		
		~Animtree(void);
		
		void PrintInfo();
};
