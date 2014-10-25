/*
 * rules.h - Enumeration of production rule names used in the grammar.
 *
 * Copyright (C) 2014 Andrew Schwartzmeyer
 *
 * This file released under the AGPLv3.
 *
 */

#ifndef RULES_H
#define RULES_H

enum rule {
	PROGRAM,
	UNQUALID_2,
	QUAL_ID,
	NESTED_NAME1,
	POSTFIX_EXPR2,
	POSTFIX_EXPR3,
	POSTFIX_EXPR4,
	POSTFIX_EXPR5,
	POSTFIX_EXPR6,
	POSTFIX_EXPR7,
	POSTFIX_EXPR8,
	POSTFIX_EXPR9,
	POSTFIX_EXPR10,
	EXPR_LIST2,
	UNARY_EXPR2,
	UNARY_EXPR3,
	UNARY_EXPR4,
	UNARY_EXPR5,
	UNARY_EXPR6,
	UNARY_EXPR7,
	UNARY_EXPR8,
	NEW_EXPR1,
	NEW_EXPR2,
	NEW_TYPE_ID,
	NEW_DECL1,
	DIRECT_NEW_DECL1,
	DIRECT_NEW_DECL2,
	DELETE_EXPR1,
	DELETE_EXPR2,
	DELETE_EXPR3,
	DELETE_EXPR4,
	PM_EXPR2,
	PM_EXPR3,
	MULT_EXPR2,
	MULT_EXPR3,
	MULT_EXPR4,
	ADD_EXPR2,
	ADD_EXPR3,
	SHIFT_EXPR2,
	SHIFT_EXPR3,
	REL_EXPR2,
	REL_EXPR3,
	REL_EXPR4,
	REL_EXPR5,
	EQUAL_EXPR2,
	EQUAL_EXPR3,
	AND_EXPR2,
	XOR_EXPR2,
	OR_EXPR2,
	LOGICAL_AND_EXPR2,
	LOGICAL_OR_EXPR2,
	COND_EXPR2,
	ASSIGN_EXPR2,
	EXPR2,
	LABELED_STATEMENT1,
	LABELED_STATEMENT2,
	STATEMENT_SEQ1,
	STATEMENT_SEQ2,
	SELECT1,
	SELECT2,
	SELECT3,
	CONDITION2,
	ITER1,
	ITER2,
	ITER3,
	JUMP3,
	DECL_SEQ2,
	SIMPLE_DECL1,
	DECL_SPEC_SEQ2,
	SIMPLE_TYPE_SPEC2,
	ELAB_TYPE_SPEC1,
	ELAB_TYPE_SPEC2,
	INIT_DECL_LIST2,
	INIT_DECL,
	DECL2,
	DIRECT_DECL2,
	DIRECT_DECL3,
	DIRECT_DECL4,
	DIRECT_DECL5,
	DIRECT_DECL6,
	PTR3,
	PTR4,
	DECL_D3,
	TYPE_ID,
	TYPE_SPEC_SEQ,
	ABSTRACT_DECL1,
	DIRECT_ABSTRACT_DECL1,
	DIRECT_ABSTRACT_DECL3,
	DIRECT_ABSTRACT_DECL4,
	PARAM_DECL_LIST1,
	PARAM_DECL1,
	PARAM_DECL2,
	PARAM_DECL3,
	PARAM_DECL4,
	FUNCTION_DEF1,
	FUNCTION_DEF2,
	INIT1,
	INIT_CLAUSE2,
	INIT_LIST2,
	CLASS_SPEC,
	CLASS_HEAD1,
	CLASS_HEAD2,
	MEMBER_SPEC1,
	MEMBER_SPEC2,
	MEMBER_DECL1,
	MEMBER_DECL_LIST1,
	MEMBER_DECL_LIST2,
	MEMBER_DECLARATOR1,
	MEMBER_DECLARATOR2,
	MEMBER_DECLARATOR3,
	CONSTANT_INIT,
	CTOR_INIT,
	MEM_INIT_LIST2,
	MEM_INIT,
	MEM_INIT_ID1,
	MEM_INIT_ID3
};

char *print_rule(int rule);

#endif /* RULES_H */
