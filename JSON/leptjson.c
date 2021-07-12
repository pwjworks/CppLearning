#include "leptjson.h"

#include <assert.h>
#include <errno.h> /* errno, ERANGE */
#include <math.h>  /* HUGE_VAL */
#include <stdlib.h>
#include <string.h> /* memcpy() */

/**
 * @brief 相等断言，json自增1
 *
 */
#define EXPECT(c, ch)         \
  do {                        \
    assert(*c->json == (ch)); \
    c->json++;                \
  } while (0)

#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

#define PUTC(c, ch)                                     \
  do {                                                  \
    *(char *)lept_context_push(c, sizeof(char)) = (ch); \
  } while (0)

#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif
/**
 * @brief JSON内容结构体,存放JSON字符串
 *
 */
typedef struct {
  const char *json;
  char *stack;
  size_t size, top;
} lept_context;
/**
 * @brief 去除空格
 *
 * @param c
 */
static void lept_parse_whitespace(lept_context *c) {
  const char *p = c->json;
  /**
   * @brief   JSON 文本由 3
   * 部分组成，首先是空白（whitespace），接着是一个值，最后是空白。
   * 所谓空白，是由零或多个空格符（space U+0020）、制表符（tab
   * U+0009）、换行符（LF U+000A）、回车符（CR U+000D）所组成。
   *
   */

  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
  c->json = p;
}

/**
 * @brief
 * 首先检测第一个字符是否为‘n’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
 *
 * @param c JSON字符串
 * @param v 解析结果
 * @return int 解析是否成功
 */
static int lept_parse_null(lept_context *c, lept_value *v) {
  EXPECT(c, 'n');
  if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
    return LEPT_PARSE_INVALID_VALUE;
  c->json += 3;
  v->type = LEPT_NULL;
  return LEPT_PARSE_OK;
}
/**
 * @brief
 * 首先检测第一个字符是否为‘t’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
 *
 * @param c JSON字符串
 * @param v 解析结果
 * @return int 解析是否成功
 */
static int lept_parse_true(lept_context *c, lept_value *v) {
  EXPECT(c, 't');
  if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
    return LEPT_PARSE_INVALID_VALUE;
  c->json += 3;
  v->type = LEPT_TRUE;
  return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context *c, lept_value *v) {
  const char *p = c->json;
  if (*p == '-') p++;
  if (*p == '0')
    p++;
  else {
    if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
    for (p++; ISDIGIT(*p); p++)
      ;
  }
  if (*p == '.') {
    p++;
    if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
    for (p++; ISDIGIT(*p); p++)
      ;
  }
  if (*p == 'e' || *p == 'E') {
    p++;
    if (*p == '+' || *p == '-') p++;
    if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
    for (p++; ISDIGIT(*p); p++)
      ;
  }
  if (*p != '\0') {
    return LEPT_PARSE_ROOT_NOT_SINGULAR;
  }
  errno = 0;
  v->u.n = strtod(c->json, NULL);
  if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
    return LEPT_PARSE_NUMBER_TOO_BIG;
  v->type = LEPT_NUMBER;
  c->json = p;
  return LEPT_PARSE_OK;
}
/**
 * @brief
 * 首先检测第一个字符是否为‘f’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
 *
 * @param c JSON字符串
 * @param v 解析结果
 * @return int 解析是否成功
 */
static int lept_parse_false(lept_context *c, lept_value *v) {
  EXPECT(c, 'f');
  if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' ||
      c->json[3] != 'e')
    return LEPT_PARSE_INVALID_VALUE;
  c->json += 4;
  v->type = LEPT_FALSE;
  return LEPT_PARSE_OK;
}

/**
 * @brief 爬取字符串内容
 *
 * @param c JSON字符串
 * @param v JSON类型结果
 * @return int JSON类型结果
 */
static int lept_parse_value(lept_context *c, lept_value *v) {
  switch (*c->json) {
    case 't':
      return lept_parse_true(c, v);
    case 'f':
      return lept_parse_false(c, v);
    case 'n':
      return lept_parse_null(c, v);
    default:
      return lept_parse_number(c, v);
    case '\0':
      return LEPT_PARSE_EXPECT_VALUE;
  }
}

static void *lept_context_push(lept_context *c, size_t size) {
  void *ret;
  assert(size > 0);
  if (c->top + size >= c->size) {
    if (c->size == 0) c->size = LEPT_PARSE_STACK_INIT_SIZE;
    while (c->top + size >= c->size)
      c->size += c->size >> 1; /* c->size * 1.5 */
    c->stack = (char *)realloc(c->stack, c->size);
  }
  ret = c->stack + c->top;
  c->top += size;
  return ret;
}

static void *lept_context_pop(lept_context *c, size_t size) {
  assert(c->top >= size);
  return c->stack + (c->top -= size);
}

/**
 * @brief
 * 爬取执行函数，执行顺序：先假设结果为LEPT_NULL，lept_parse_whitespace去除空格，lept_parse_value爬取结果
 *
 * @param v JSON类型结果
 * @param json JSON字符串
 * @return int JSON爬取结果
 */
int lept_parse(lept_value *v, const char *json) {
  lept_context c;
  int ret;
  assert(v != NULL);
  c.json = json;
  c.stack = NULL;
  c.size = c.top = 0;
  v->type = LEPT_NULL;
  lept_parse_whitespace(&c);
  if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
    lept_parse_whitespace(&c);
    if (*c.json != '\0') ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
  }
  assert(c.top == 0);
  free(c.stack);
  return ret;
}
/**
 * @brief 释放内存，并把type设置为null
 *
 * @param v
 */
void lept_free(lept_value *v) {
  assert(v != NULL);
  if (v->type == LEPT_STRING) free(v->u.s.s);
  v->type = LEPT_NULL;
}
/**
 * @brief 获取JSON类型结果
 *
 * @param v JSON类型结果
 * @return lept_type JSON类型结果
 */
lept_type lept_get_type(const lept_value *v) {
  assert(v != NULL);
  return v->type;
}

double lept_get_number(const lept_value *v) {
  assert(v != NULL && v->type == LEPT_NUMBER);
  return v->u.n;
}

const char *lept_get_string(const lept_value *v) {
  assert(v != NULL && v->type == LEPT_STRING);
  return v->u.s.s;
}

size_t lept_get_string_length(const lept_value *v) {
  assert(v != NULL && v->type == LEPT_STRING);
  return v->u.s.len;
}

void lept_set_string(lept_value *v, const char *s, size_t len) {
  assert(v != NULL && (s != NULL || len == 0));
  lept_free(v);
  v->u.s.s = (char *)malloc(len + 1);
  memcpy(v->u.s.s, s, len);
  v->u.s.s[len] = '\0';
  v->u.s.len = len;
  v->type = LEPT_STRING;
}

static int lept_parse_string(lept_context *c, lept_value *v) {
  size_t head = c->top, len;
  const char *p;
  EXPECT(c, '\"');
  p = c->json;
  for (;;) {
    char ch = *p++;
    switch (ch) {
      case '\"':
        len = c->top - head;
        lept_set_string(v, (const char *)lept_context_pop(c, len), len);
        c->json = p;
        return LEPT_PARSE_OK;
      case '\0':
        c->top = head;
        return LEPT_PARSE_MISS_QUOTATION_MARK;
      default:
        PUTC(c, ch);
    }
  }
}