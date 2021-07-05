#include <cstdio>
#include <memory>

#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

inline void EXPECT_EQ_BASE(bool equality, int expect, int actual) {
  test_count++;
  if (equality)
    test_pass++;
  else {
    fprintf(stderr, "%s:%d: expect: %d actual: %d\n", __FILE__, __LINE__, expect, actual);
    main_ret = 1;
  }
}

inline void EXPECT_EQ_INT(int expect, int actual) {
  EXPECT_EQ_BASE((expect) == (actual), expect, actual);
}

/**
 * @brief 爬取null字符串
 *
 */
static void test_parse_null(LEPTJSON::lept_parser::ptr parser) {
  using namespace LEPTJSON;
  EXPECT_EQ_INT(lept_parse_res::LEPT_PARSE_OK, parser->parse("null"));
  EXPECT_EQ_INT(lept_type::type::LEPT_NULL, parser->getType());
}

static void test_parse() {
  std::shared_ptr<LEPTJSON::lept_value> value(new LEPTJSON::lept_value(LEPTJSON::lept_type::type::LEPT_FALSE));
  std::shared_ptr<LEPTJSON::lept_parser> parser(new LEPTJSON::lept_parser(value));
  test_parse_null(parser);

}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count,
    test_pass * 100.0 / test_count);
  return main_ret;
}