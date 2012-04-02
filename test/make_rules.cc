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
    Match *m;
	r = new MakeRule();

	// Test non-matching regular rule
	CU_ASSERT( r->match( "hello", &m ) == false );

	delete r;
}

void test_make_rules_2(void)
{
    Match *m;
	r = new MakeRule();

	// Test matching regular rule
	r->addTarget("hello");
	CU_ASSERT( r->match( "hello", &m ) == true );

    delete m;
	delete r;
}

void test_make_rules_3(void)
{
    Match *m;
	r = new MakeRule();

	// Test prefix rule
	r->addTarget("%.hello");
	CU_ASSERT( r->match( "", &m) == false );
	CU_ASSERT( r->match( ".hello", &m) == true );
    delete m;
	CU_ASSERT( r->match( "a.hello", &m) == true );
    delete m;
	CU_ASSERT( r->match( "a.goodbye", &m) == false );
	CU_ASSERT( r->match( "target.hello", &m) == true );
	CU_ASSERT( r->expand_command( "echo $@", "target.hello", m ) == string( "echo target.hello" ) );
    delete m;
	CU_ASSERT( r->match( "target.hello", &m) == true );
	CU_ASSERT( r->expand_command( "$@ echo", "target.hello", m ) == string( "target.hello echo" ) );
    delete m;
	CU_ASSERT( r->match( "target.hello", &m) == true );
	CU_ASSERT( r->expand_command( "echo $@ $@ echo", "target.hello", m ) == string( "echo target.hello target.hello echo" ) );
    delete m;

	delete r;
}

void test_make_rules_4(void)
{
    Match *m;
	r = new MakeRule();

	// Test suffix rule
	r->addTarget("hello%");
	CU_ASSERT( r->match( "", &m) == false );
	CU_ASSERT( r->match( "hello", &m) == true );
    delete m;
	CU_ASSERT( r->match( "hellothere", &m) == true );
    delete m;
	CU_ASSERT( r->match( "goodbyethere", &m ) == false );

	delete r;
}

void test_make_rules_5(void)
{
    Match *m;
	r = new MakeRule();

	// Test internal wildcard
	r->addTarget("a%b");
	CU_ASSERT( r->match( "", &m) == false );
	CU_ASSERT( r->match( "ab", &m) == true );
    delete m;
	CU_ASSERT( r->match( "acb", &m) == true );
    delete m;
	CU_ASSERT( r->match( "bca", &m ) == false );

	delete r;
}
