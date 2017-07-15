MAKE_CHILD_OPTIONS=--no-print-directory

CXX=g++
CXXFLAGS=-Wall -D_DEBUG -g -pthread

BIN_FILE=parser

BIN_DIR=bin
OBJ_DIR=obj
DEP_DIR=$(OBJ_DIR)

SRC_DIR=src/cpp
SRC_PATTERN=\( -name "*.c" -o -name "*.cpp" \)

SRCS=$(shell find $(SRC_DIR) $(SRC_PATTERN))
OBJS=$(patsubst $(SRC_DIR)%,$(OBJ_DIR)%.o,$(basename $(SRCS)))
DEPS=$(patsubst $(SRC_DIR)%,$(DEP_DIR)%.d,$(basename $(SRCS)))

-include $(DEPS)

PARSER_SRC=src/gsc.ypp
LEXER_SRC=src/gsc.l

PARSER_SRC_OUT=parser/gsc.tab
LEXER_SRC_OUT=parser/gsc.yy

YACC=bison
LEX=flex
YFLAGS=-d
LFLAGS=-d -Cfa --header-file="$(SRC_DIR)/$(LEXER_SRC_OUT).h"

$(SRC_DIR)/$(LEXER_SRC_OUT).cpp $(SRC_DIR)/$(LEXER_SRC_OUT).h: $(LEXER_SRC)
	@echo "[COMPILE] $@"
	@$(LEX) -o "$(SRC_DIR)/$(LEXER_SRC_OUT).cpp" $(LFLAGS) "$(LEXER_SRC)"

$(SRC_DIR)/$(PARSER_SRC_OUT).cpp $(SRC_DIR)/$(PARSER_SRC_OUT).hpp: $(PARSER_SRC)
	@echo "[COMPILE] $@"
	@$(YACC) -v -o "$(SRC_DIR)/$(PARSER_SRC_OUT).cpp" $(YFLAGS) "$(PARSER_SRC)"

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp $(SRC_DIR)/$(PARSER_SRC_OUT).hpp $(SRC_DIR)/$(LEXER_SRC_OUT).h
	@echo "[DEPS] $@"
	@mkdir --parents $(dir $@)
	@$(CXX) -MM $(word 1,$^) -MT $(OBJ_DIR)/$*.o -MF $@

$(OBJ_DIR)/%.o: $(DEP_DIR)/%.d
	@echo "[COMPILE] $*.o"
	@mkdir --parents $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(filter $(SRCS),$^)

$(BIN_FILE): $(OBJ_DIR)/$(PARSER_SRC_OUT).o $(OBJ_DIR)/$(LEXER_SRC_OUT).o $(OBJS)
	@echo "[LINK] $(BIN_DIR)/$(BIN_FILE)"
	@mkdir --parents $(dir $(BIN_DIR)/$(BIN_FILE))
	@$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$(BIN_FILE)

.DEFAULT_GOAL:=$(BIN_FILE)

.PHONY: clean
clean:
	@rm -rf $(BIN_DIR)/$(BIN_FILE)
	@rm -rf $(DEP_DIR)
	@rm -rf $(OBJ_DIR)
	@mkdir -p src/cpp/parser && rm -f -r src/cpp/parser/*

.PHONY: test
test:
	@$(MAKE) $(MAKE_CHILD_OPTIONS)
	@gnome-terminal -x ./bin/parser

.PHONY: test-file
test-file:
	@$(MAKE) $(MAKE_CHILD_OPTIONS)
	@gnome-terminal -x ./bin/test.sh