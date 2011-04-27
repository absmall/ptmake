#include "tests.h"
#include "CUnit/Basic.h"

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
   pSuite = CU_add_suite("Suite deps", init_deps, clean_deps);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test dependencies", test_deps))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   pSuite = CU_add_suite("Suite make rules", init_make_rules, clean_make_rules);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test make rules 1", test_make_rules_1))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test make rules 2", test_make_rules_2))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test make rules 3", test_make_rules_3))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test make rules 4", test_make_rules_4))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test make rules 5", test_make_rules_5))) {
	   CU_cleanup_registry();
	   return CU_get_error();
   }

   /* Run all tests using the console interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}


