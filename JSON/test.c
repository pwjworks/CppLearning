#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                \
  do {                                                                  \
    test_count++;                                                       \
    if (equality)                                                       \
      test_pass++;                                                      \
    else {                                                              \
      fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", \
              __FILE__, __LINE__, expect, actual);                      \
      main_ret = 1;                                                     \
    }                                                                   \
  } while (0)

#define EXPECT_EQ_INT(expect, actual) \
  EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) \
  EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

#define EXPECT_EQ_STRING(expect, actual, alength)                            \
  EXPECT_EQ_BASE(                                                            \
      sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, \
      expect, actual, "%s")

#define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
#define EXPECT_FALSE(actual) \
  EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")

/**
 * @brief 爬取null字符串
 *
 */
static void test_parse_null() {
  lept_value v;
  v.type = LEPT_FALSE;
  EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
  EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
/**
 * @brief 爬取true字符串
 *
 */
static void test_parse_true() {
  lept_value v;
  v.type = LEPT_FALSE;
  EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
  EXPECT_EQ_INT(LEPT_TRUE, lept_get_type(&v));
}

/**
 * @brief 爬取false字符串
 *
 */
static void test_parse_false() {
  lept_value v;
  v.type = LEPT_FALSE;
  EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
  EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}

#define TEST_NUMBER(expect, json)                       \
  do {                                                  \
    lept_value v;                                       \
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, json)); \
    EXPECT_EQ_INT(LEPT_NUMBER, lept_get_type(&v));      \
    EXPECT_EQ_DOUBLE(expect, lept_get_number(&v));      \
  } while (0)

static void test_parse_number() {
  TEST_NUMBER(0.0, "0");
  TEST_NUMBER(0.0, "-0");
  TEST_NUMBER(0.0, "-0.0");
  TEST_NUMBER(1.0, "1");
  TEST_NUMBER(-1.0, "-1");
  TEST_NUMBER(1.5, "1.5");
  TEST_NUMBER(-1.5, "-1.5");
  TEST_NUMBER(3.1416, "3.1416");
  TEST_NUMBER(1E10, "1E10");
  TEST_NUMBER(1e10, "1e10");
  TEST_NUMBER(1E+10, "1E+10");
  TEST_NUMBER(1E-10, "1E-10");
  TEST_NUMBER(-1E10, "-1E10");
  TEST_NUMBER(-1e10, "-1e10");
  TEST_NUMBER(-1E+10, "-1E+10");
  TEST_NUMBER(-1E-10, "-1E-10");
  TEST_NUMBER(1.234E+10, "1.234E+10");
  TEST_NUMBER(1.234E-10, "1.234E-10");
  TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
}

#define TEST_ERROR(error, json)                  \
  do {                                           \
    lept_value v;                                \
    v.type = LEPT_FALSE;                         \
    EXPECT_EQ_INT(error, lept_parse(&v, json));  \
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v)); \
  } while (0)

static void test_parse_expect_value() {
  TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, "");
  TEST_ERROR(LEPT_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value() {
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "nul");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "?");

#if 1
  /* invalid number */
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+0");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+1");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE,
             ".123"); /* at least one digit before '.' */
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "1."); /* at least one digit after '.' */
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "INF");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "inf");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "NAN");
  TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "nan");
#endif
}

static void test_parse_root_not_singular() {
  TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR, "null x");

#if 1
  /* invalid number */
  TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR,
             "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
  TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR, "0x0");
  TEST_ERROR(LEPT_PARSE_ROOT_NOT_SINGULAR, "0x123");
#endif
}

static void test_parse_number_too_big() {
#if 1
  TEST_ERROR(LEPT_PARSE_NUMBER_TOO_BIG, "1e309");
  TEST_ERROR(LEPT_PARSE_NUMBER_TOO_BIG, "-1e309");
#endif
}

static void test_parse_string() {
  lept_value v;
  lept_init(&v);
  lept_set_string(&v, "", 0);
  EXPECT_EQ_STRING("", lept_get_string(&v), lept_get_string_length(&v));
  lept_set_string(&v, "Hello", 5);
  EXPECT_EQ_STRING("Hello", lept_get_string(&v), lept_get_string_length(&v));
  lept_free(&v);
}

static void test_parse() {
  test_parse_null();
  test_parse_true();
  test_parse_false();
  test_parse_number();
  test_parse_expect_value();
  test_parse_invalid_value();
  test_parse_root_not_singular();
  test_parse_number_too_big();
  test_parse_string();
}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count,
         test_pass * 100.0 / test_count);
  return main_ret;
}