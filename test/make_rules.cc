#include "make_rules.h"
#include <CUnit/Basic.h>
#include <iostream>

using namespace std;

Rule *r;

int init_make_rules(void)
{
	return 0;
}

int clean_make_rules(void)
{
	return 0;
}

void test_make_rules_1(void)
{
	r = new MakeRule();

	// Test non-matching regular rule
	CU_ASSERT( r->match( "hello" ) == false );

	delete r;
}

void test_make_rules_2(void)
{
	r = new MakeRule();

	// Test matching regular rule
	r->addTarget("hello");
	CU_ASSERT( r->match( "hello" ) == true );

	delete r;
}

void test_make_rules_3(void)
{
	r = new MakeRule();

	// Test prefix rule
	r->addTarget("%.hello");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( ".hello") == true );
	CU_ASSERT( r->match( "a.hello") == true );
	CU_ASSERT( r->match( "a.goodbye") == false );
	CU_ASSERT( r->expand_command( "echo $@" ) == string( "echo hello" ) );
	CU_ASSERT( r->expand_command( "$@ echo" ) == string( "hello echo" ) );
	CU_ASSERT( r->expand_command( "echo $@ $@ echo" ) == string( "echo hello hello echo" ) );

	delete r;
}

void test_make_rules_4(void)
{
	r = new MakeRule();

	// Test suffix rule
	r->addTarget("hello%");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( "hello") == true );
	CU_ASSERT( r->match( "hellothere") == true );
	CU_ASSERT( r->match( "goodbyethere" ) == false );

	delete r;
}

void test_make_rules_5(void)
{
	r = new MakeRule();

	// Test internal wildcard
	r->addTarget("a%b");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( "ab") == true );
	CU_ASSERT( r->match( "acb") == true );
	CU_ASSERT( r->match( "bca" ) == false );

	delete r;
}
