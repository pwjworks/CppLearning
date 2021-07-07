#include "leptjson.h"

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

  void lept_parser::lept_parse_whitespace() {
    const char* p = json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    json = p;
  }

  int lept_parser::lept_parse_false() {
    EXPECT('f');
    if (json[0] != 'a' || json[1] != 'l' || json[2] != 's' || json[3] != 'e')
      return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
    json += 4;
    value->setType(lept_type::LEPT_FALSE);
    return lept_parse_res::LEPT_PARSE_OK;
  }

  int lept_parser::lept_parse_value() {
    switch (*json) {
    case 't':
      return lept_parse_true();
    case 'f':
      return lept_parse_false();
    case 'n':
      return lept_parse_null();
    case '\0':
      return lept_parse_res::LEPT_PARSE_EXPECT_VALUE;
    default:
      return lept_parse_res::LEPT_PARSE_INVALID_VALUE;
    }
  }

  int lept_parser::parse(const char* _json) {
    json = _json;
    assert(json != NULL);
    value->setType(lept_type::LEPT_NULL);
    lept_parse_whitespace();
    return lept_parse_value();
  }
}