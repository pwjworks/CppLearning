#include "leptjson.h"

#include <assert.h>
#include <stdlib.h>

/**
 * @brief 相等断言
 *
 */
#define EXPECT(c, ch)         \
  do {                        \
    assert(*c->json == (ch)); \
    c->json++;                \
  } while (0)

 /**
  * @brief JSON内容结构体
  *
  */
typedef struct {
  const char* json;
} lept_context;
/**
 * @brief 去除空格
 *
 * @param c
 */
static void lept_parse_whitespace(lept_context* c) {
  const char* p = c->json;
  /**
   * @brief   JSON 文本由 3 部分组成，首先是空白（whitespace），接着是一个值，最后是空白。
   * 所谓空白，是由零或多个空格符（space U+0020）、制表符（tab U+0009）、换行符（LF U+000A）、回车符（CR U+000D）所组成。
   *
   */

  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
  c->json = p;
}
/**
 * @brief 首先检测第一个字符是否为‘n’，再检测其他字符
 *
 * @param c JSON字符串
 * @param v 解析结果
 * @return int 解析是否成功
 */
static int lept_parse_null(lept_context* c, lept_value* v) {
  EXPECT(c, 'n');
  if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    return LEPT_PARSE_INVALID_VALUE;
  c->json += 3;
  v->type = LEPT_NULL;
  return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
  switch (*c->json) {
  case 'n':
    return lept_parse_null(c, v);
  case '\0':
    return LEPT_PARSE_EXPECT_VALUE;
  default:
    return LEPT_PARSE_INVALID_VALUE;
  }
}

int lept_parse(lept_value* v, const char* json) {
  lept_context c;
  assert(v != NULL);
  c.json = json;
  v->type = LEPT_NULL;
  lept_parse_whitespace(&c);
  return lept_parse_value(&c, v);
}

lept_type lept_get_type(const lept_value* v) {
  assert(v != NULL);
  return v->type;
}