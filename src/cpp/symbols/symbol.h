#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string.h>
#include <vector>

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
	private:
		// Only used internally to keep track of the tree state
		void PrintInfoRecursive(FILE* os, std::vector<const Symbol*>& stack) const;

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

		// Ensure that all children of this symbol
		// have the correct parent pointer value
		void UpdateChildren(void);
		
	public:
		Symbol(void);
		Symbol(YYLTYPE loc);
		
		virtual ~Symbol();
		
		SYMBOL_TYPE Type(void) const;
		Range Location(void) const;
		
		void AddChild(Symbol* child);
		void FreeChildren(void);
		
		Symbol* Children(void) const;
		
		virtual void PrintInfo(FILE* os = stdout) const;
		void PrintInfoRecursive(FILE* os = stdout) const;
		virtual void PrintSymbol(FILE* os = stdout) const;
		
		void _debug_override_type(SYMBOL_TYPE type);
};

//
// Include any child class types AFTER Symbol is defined
//
#include "literal.h"

#include "include.h"
#include "animtree.h"
#include "function.h"

#include "conditional.h"

#include "expression.h"
#include "member.h"
#include "identifier.h"
#include "reference.h"
#include "pointer.h"
#include "group.h"

#include "return.h"
#include "wait.h"
