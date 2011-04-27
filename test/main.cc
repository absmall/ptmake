#include <stdio.h>
#include <string.h>
#include <dependencies.h>
#include "CUnit/Basic.h"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
	dependencies_init();
	return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
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

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test_of_dependencies", test_deps))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   /* Run all tests using the console interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}


