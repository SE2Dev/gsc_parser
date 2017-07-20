#include "symbol.h"

Symbol::Symbol(void) : type(S_TYPE_NONE), prev(NULL), next(NULL), children(NULL)
{
	this->SetOwner(this);
	//fprintf(os, "SYMBOL CTOR %s\n", SYMBOL_TYPE_STRING(type));
}

Symbol::Symbol(YYLTYPE loc) : type(S_TYPE_NONE), prev(NULL), next(NULL), children(NULL), location(loc)
{
	this->SetOwner(this);
	//fprintf(os, "CTOR %s\n", SYMBOL_TYPE_STRING(type));
}

Symbol::~Symbol()
{
	/*fprintf(os, "~%s with %d children at %d(%d) - %d(%d)\n",
		SYMBOL_TYPE_STRING(type),
		this->children ? this->children->Size() + 1 : 0,
		location.start.line,
		location.start.character,
		location.end.line,
		location.end.character);*/
	
	delete this->children;
	this->children = NULL;
	delete this->NextElem();
	this->next = NULL;
}

SYMBOL_TYPE Symbol::Type(void) const
{
	return this->type;
}

Range Symbol::Location(void) const
{
	return this->location;
}

void Symbol::AddChild(Symbol* child)
{
	if(!this->children)
	{
		children = child;
		child->parent = this;
	}
	else
	{
		children->AddToEnd(child);
		child->parent = this;
	}
}

void Symbol::FreeChildren(void)
{
	for(Symbol* s = this->children; s; s = s->NextElem())
	{
		s->FreeChildren();
	}
	
	delete this->children;
	this->children = NULL;
}

Symbol* Symbol::Children(void) const
{
	return this->children;
}

void Symbol::UpdateChildren(void)
{
	for(Symbol* c = this->children; c; c = c->NextElem())
	{
		c->parent = this;
	}
}

Symbol* Symbol::Parent(void) const
{
	return this->parent;
}

void Symbol::PrintInfo(FILE* os) const
{
	fprintf(os, "%s with %d children at %d(%d) - %d(%d)\n",
		SYMBOL_TYPE_STRING(type),
		this->children ? this->children->Size() + 1 : 0,
		location.start.line,
		location.start.character,
		location.end.line,
		location.end.character);
}

//
// Used to provide symbol data to CoD-Sense
//
void Symbol::PrintSymbol(FILE* os) const
{
	//
	// type|location[|name|details]
	// By default do not provide type specific info
	//
	//fprintf(os, "%s|%d %d %d %d\n",
	//	SYMBOL_TYPE_STRING(type),
	//	location.start.line,
	//	location.start.character,
	//	location.end.line,
	//	location.end.character);
}

void Symbol::PrintInfoRecursive(FILE* os, std::vector<const Symbol*>& stack) const
{
	// Iterate over all symbols on the stack from oldest to newest
	
	for(unsigned int i = 0; i < stack.size(); i++)
	{
		if(stack[i] == this->parent)
		{
			fprintf(os, "%s", this->NextElem() ? "├── " : "└── ");
		}
		else if (i+1 < stack.size() && stack[i+1]->NextElem())
		{
			fprintf(os, "%s", "│   ");
		}
		else
		{
			fprintf(os, "    ");
		}
	}

	this->PrintInfo(os);

	stack.push_back(this);
	for(Symbol* c = this->children; c; c = c->NextElem())
	{
		c->PrintInfoRecursive(os, stack);
	}
	stack.pop_back();
}

void Symbol::PrintInfoRecursive(FILE* os) const
{
	std::vector<const Symbol*> stack;
	this->PrintInfoRecursive(os, stack);
}

void Symbol::_debug_override_type(SYMBOL_TYPE type)
{
	this->type = type;
}
