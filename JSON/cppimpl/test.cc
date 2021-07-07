#include <cstdio>
#include <memory>

#include "leptjson.h"

using namespace LEPTJSON;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

static std::shared_ptr<lept_parser> parser;

/**
 * @brief 因为需要打印所在行数，所以需要宏。
 *
 */
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


 /**
  * @brief 爬取null字符串
  *
  */
static void test_parse_null() {
  EXPECT_EQ_INT(lept_parse_res::LEPT_PARSE_OK, parser->parse("null"));
  EXPECT_EQ_INT(lept_type::type::LEPT_NULL, parser->getValue()->getType());
}

/**
 * @brief 爬取true字符串
 *
 */
static void test_parse_true() {
  EXPECT_EQ_INT(lept_parse_res::LEPT_PARSE_OK, parser->parse("true"));
  EXPECT_EQ_INT(lept_type::type::LEPT_TRUE, parser->getValue()->getType());
}

/**
 * @brief 爬取false字符串
 *
 */
static void test_parse_false() {
  EXPECT_EQ_INT(lept_parse_res::LEPT_PARSE_OK, parser->parse("false"));
  EXPECT_EQ_INT(lept_type::type::LEPT_FALSE, parser->getValue()->getType());
}

#define TEST_NUMBER(expect, json)\
    do {\
        EXPECT_EQ_INT(lept_parse_res::LEPT_PARSE_OK, parser->parse(json));\
        EXPECT_EQ_INT(lept_type::type::LEPT_NUMBER, parser->getValue()->getType());\
        EXPECT_EQ_DOUBLE(expect, parser->getValue()->getN());\
    } while(0)

/**
 * @brief 测试数字
 *
 */
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

/**
 * @brief 测试错误宏
 *
 * @param error 期待的结果
 * @param json 带解析JSON字符串
 */
#define TEST_ERROR(error, json)                                      \
    do {                                                             \
        EXPECT_EQ_INT(error, parser->parse(json));                   \
        EXPECT_EQ_INT(lept_type::type::LEPT_NULL, parser->getValue()->getType());\
    } while(0)
 /**
  * @brief 爬取非法字符串
  *
  */
static void test_parse_invalid_value() {
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "nul");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "?");
#if 1
  /* invalid number */
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "+0");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "+1");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "INF");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "inf");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "NAN");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_INVALID_VALUE, "nan");
#endif

}
/**
 * @brief 不单独
 *
 */
static void test_parse_root_not_singular() {
  TEST_ERROR(lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR, "null x");
#if 1
  /* invalid number */
  TEST_ERROR(lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
  TEST_ERROR(lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR, "0x0");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR, "0x123");
#endif

}
/**
 * @brief 测试过大的数字
 *
 */
static void test_parse_number_too_big() {
#if 1
  TEST_ERROR(lept_parse_res::LEPT_PARSE_NUMBER_TOO_BIG, "1e309");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_NUMBER_TOO_BIG, "-1e309");
#endif
}
/**
 * @brief 爬取非法字符串
 *
 */
static void test_parse_expect_value() {
  TEST_ERROR(lept_parse_res::LEPT_PARSE_EXPECT_VALUE, "");
  TEST_ERROR(lept_parse_res::LEPT_PARSE_EXPECT_VALUE, " ");
}

static void test_parse() {
  std::shared_ptr<lept_value> value(new lept_value(lept_type::type::LEPT_FALSE));
  parser.reset(new lept_parser(value));
  test_parse_null();
  test_parse_true();
  test_parse_false();
  test_parse_invalid_value();
  test_parse_expect_value();
  test_parse_root_not_singular();
  test_parse_number_too_big();
  test_parse_number();
}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count,
    test_pass * 100.0 / test_count);
  return main_ret;
}