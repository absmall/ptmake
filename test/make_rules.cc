#include "make_rules.h"
#include <CUnit/Basic.h>

Rule *r;

int init_make_rules(void)
{
	r = new MakeRule();
	return 0;
}

int clean_make_rules(void)
{
	delete r;
	return 0;
}

void test_make_rules_1(void)
{
	// Test non-matching regular rule
	CU_ASSERT( r->match( "hello" ) == false );
}

void test_make_rules_2(void)
{
	// Test matching regular rule
	r->addTarget("hello");
	CU_ASSERT( r->match( "hello" ) == true );
}

void test_make_rules_3(void)
{
	// Test prefix rule
	r->addTarget("%.hello");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( ".hello") == true );
	CU_ASSERT( r->match( "a.hello") == true );
	CU_ASSERT( r->match( "a.goodbye") == false );
}

void test_make_rules_4(void)
{
	// Test suffix rule
	r->addTarget("hello%");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( "hello") == true );
	CU_ASSERT( r->match( "hellothere") == true );
	CU_ASSERT( r->match( "goodbyethere" ) == false );
}

void test_make_rules_5(void)
{
	// Test internal wildcard
	r->addTarget("a%b");
	CU_ASSERT( r->match( "") == false );
	CU_ASSERT( r->match( "ab") == true );
	CU_ASSERT( r->match( "acb") == true );
	CU_ASSERT( r->match( "bca" ) == false );
}
