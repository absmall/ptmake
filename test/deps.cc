#include <dependencies.h>
#include <CUnit/Basic.h>

int init_deps(void)
{
	dependencies_init();
	return 0;
}

int clean_deps(void)
{
	dependencies_deinit();
	return 0;
}

void test_deps(void)
{
	std::list<std::pair<std::string, bool> >::iterator i;
	bool found_a, found_b;
	const unsigned char *name=(const unsigned char *)"0123456789ABCDEF0123456789ABCDEF";
	std::set<std::pair<std::string, bool> > deps;
	std::list<std::pair<std::string, bool> > *ret;

	deps.insert(std::pair<std::string, bool>("a", true));
	deps.insert(std::pair<std::string, bool>("b", false));
	clear_dependencies(name);
	add_dependencies(name, deps);

	ret = retrieve_dependencies(name);
	found_a = false;
	found_b = false;
	for( i = ret->begin(); i != ret->end(); i ++ ) {
		if( i->first == "a" ) {
			CU_ASSERT( i->second == true );
			found_a = true;
		}
		if( i->first == "b" ) {
			CU_ASSERT( i->second == false );
			found_b = true;
		}
	}
	CU_ASSERT( found_a == true );
	CU_ASSERT( found_b == true );
	delete ret;

	clear_dependencies(name);

	ret = retrieve_dependencies(name);
	CU_ASSERT( ret == NULL );
}


