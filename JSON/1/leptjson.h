#ifndef LEPTJSON_H__
#define LEPTJSON_H__

/**
 * @brief 类型枚举
 *
 */
typedef enum {
  LEPT_NULL,
  LEPT_FALSE,
  LEPT_TRUE,
  LEPT_NUMBER,
  LEPT_STRING,
  LEPT_ARRAY,
  LEPT_OBJECT
} lept_type;

/**
 * @brief JSON类型结构体
 *
 */
typedef struct {
  lept_type type;
} lept_value;

/**
 * @brief 类型结果枚举
 *
 */
enum {
  LEPT_PARSE_OK = 0,
  LEPT_PARSE_EXPECT_VALUE,
  LEPT_PARSE_INVALID_VALUE,
  LEPT_PARSE_ROOT_NOT_SINGULAR
};

int lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);

#endif /* LEPTJSON_H__ */