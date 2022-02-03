#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "useful.h"

#ifdef CUNIT_TEST
    #include <curses.h>
    #include <CUnit/CUnit.h>
    #include <CUnit/Basic.h>
    #include <CUnit/Automated.h>
#endif


#define TEST(text) (printf("*** TEST: %s ***\n", text))

void testcase_1();


#ifdef CUNIT_TEST
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }
int setup_suite(void) { return 0; }
int teardown_suite(void) { return 0; }

static CU_TestInfo CU_1[] =
{
    {"CU_1:", testcase_1 },
    CU_TEST_INFO_NULL
};

static CU_SuiteInfo scenario[] =
{
  { "CU_1",  init_suite, clean_suite, setup_suite, teardown_suite, CU_1 },
  CU_SUITE_INFO_NULL
};

void AddTests(void)
{
    assert(NULL != CU_get_registry());
    assert(!CU_is_test_running());
    if (CU_register_suites(scenario) != CUE_SUCCESS)
    {
      printf("CU_register_suites Error\n");
      exit(EXIT_FAILURE);
    }
   return;
}
#endif //CUNIT_TEST

int main()
{
#ifdef __COVERAGESCANNER__
    int ret = 0;

    __coveragescanner_install("test_1");
    __coveragescanner_clear();
    __coveragescanner_testname("CU_1");
#endif

#ifdef CUNIT_TEST
    CU_BasicRunMode mode = CU_BRM_VERBOSE;
    CU_ErrorAction error_action = CUEA_IGNORE;
    setvbuf(stdout, NULL, _IONBF, 0);
    if (!CU_initialize_registry()) {
        AddTests();
        CU_basic_set_mode(mode);
        CU_set_error_action(error_action);
        printf("\nTests completed with return value %d.\n", CU_basic_run_tests());
        printf("\n");

        printf("\n\n");
        CU_set_output_filename("CU_1");
        CU_cleanup_registry();
    }
#else //!CUNIT_TEST 
   testcase_1();
#endif //!CUNIT_TEST 
   
#ifdef __COVERAGESCANNER__
    __coveragescanner_teststate("PASSED");
    __coveragescanner_save();
#else	
    return 0;
#endif
}

void testcase_1()
{
   unsigned char s[] = {  0x31, 0xa0, 0xf9, 0x44, 0x65, 0x61, 0x64, 0xFF, 0xFF };
   unsigned char hex1[] = { " " };
   unsigned char hex2[] = { '\0' };
   unsigned char hex3[] = { "deadbeff000" };
   unsigned char *hex;
   unsigned char *bin;
   size_t k, len;
   
   TEST("bin2hex");
   hex = bin2hex(s, sizeof s);
   printf("hex=%s\n", hex);

   TEST("hex2bin");
   hex2bin(NULL);
   hex2bin(hex1);
   hex2bin(hex2);
   hex2bin(hex3);
   bin = hex2bin((unsigned char *)hex);
   printf("bin=%s\n", bin);

   TEST("bin2hex");
   len = strlen((char *)s) / 2 - 1;
   bin2hex(bin, len);
   for(k=0; k < len; k++)
        printf("%02X ", bin[k]);

    printf("\n");
}





