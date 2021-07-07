#include "leptjson.h"

#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
namespace LEPTJSON {
  lept_parser::lept_parser(lept_value::ptr _value) :value(_value) {
  }

  int lept_parser::lept_parse_null() {
    EXPECT('n');
    if (json[0] != 'u' || json[1] != 'l' || json[2] != 'l')
      return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
    json += 3;
    value->setType(lept_type::LEPT_NULL);
    return lept_parse_res::LEPT_PARSE_OK;
  }

  int lept_parser::lept_parse_true() {
    EXPECT('t');
    if (json[0] != 'r' || json[1] != 'u' || json[2] != 'e')
      return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
    json += 3;
    value->setType(lept_type::LEPT_TRUE);
    return lept_parse_res::LEPT_PARSE_OK;
  }

  int lept_parser::lept_parse_false() {
    EXPECT('f');
    if (json[0] != 'a' || json[1] != 'l' || json[2] != 's' || json[3] != 'e')
      return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
    json += 4;
    value->setType(lept_type::LEPT_FALSE);
    return lept_parse_res::LEPT_PARSE_OK;
  }

  int lept_parser::lept_parse_number() {
    const char* p = json;
    if (*p == '-')p++;
    if (*p == '0')p++;
    else {
      if (!ISDIGIT1TO9(*p)) return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
      for (p++;ISDIGIT(*p);p++);
    }
    // 小数点后要接数字，否则返回数字非法。
    if (*p == '.') {
      p++;
      if (!ISDIGIT(*p))return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
      for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
      p++;
      if (*p == '+' || *p == '-')p++;
      if (!ISDIGIT(*p))return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
      for (p++; ISDIGIT(*p); p++);
    }
    if (*p != '\0') {
      return lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    errno = 0;
    value->setN(strtod(json, NULL));
    if (errno == ERANGE && (value->getN() == HUGE_VAL || value->getN() == -HUGE_VAL))
      return lept_parse_res::LEPT_PARSE_NUMBER_TOO_BIG;
    value->setType(lept_type::LEPT_NUMBER);
    json = p;
    return lept_parse_res::LEPT_PARSE_OK;
  }

  void lept_parser::lept_parse_whitespace() {
    const char* p = json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    json = p;
  }

  int lept_parser::lept_parse_value() {
    switch (*json) {
    case 't':
      return lept_parse_true();
    case 'f':
      return lept_parse_false();
    case 'n':
      return lept_parse_null();
    default:
      return lept_parse_number();
    case '\0':
      return lept_parse_res::LEPT_PARSE_EXPECT_VALUE;
    }
  }

  int lept_parser::parse(const char* _json) {
    json = _json;
    assert(json != NULL);
    int ret;
    value->setType(lept_type::LEPT_NULL);
    lept_parse_whitespace();
    if ((ret = lept_parse_value()) == lept_parse_res::LEPT_PARSE_OK) {
      lept_parse_whitespace();
      if (*json != '\0')
        ret = lept_parse_res::LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
  }
}