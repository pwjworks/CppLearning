#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <memory>
#include <cassert>


namespace LEPTJSON {
  /**
   * @brief 类型枚举类
   *
   */
  class lept_type {
  public:
    enum type {
      LEPT_NULL,
      LEPT_FALSE,
      LEPT_TRUE,
      LEPT_NUMBER,
      LEPT_STRING,
      LEPT_ARRAY,
      LEPT_OBJECT
    };
  };
  /**
   * @brief JSON解析结果枚举类
   *
   */
  class lept_parse_res {
  public:
    enum type {
      LEPT_PARSE_OK = 0,
      LEPT_PARSE_EXPECT_VALUE,
      LEPT_PARSE_INVALID_VALUE,
      LEPT_PARSE_ROOT_NOT_SINGULAR
    };
  };
  /**
   * @brief JSON类型类
   *
   */
  class lept_value {
  private:
    lept_type::type type;
  public:
    typedef std::shared_ptr<lept_value> ptr;
    lept_value(lept_type::type _type) :type(_type) {
    }
    /**
     * @brief 返回解析结果类型
     *
     * @return lept_type::type 解析结果类型
     */
    lept_type::type getType() const { return type; };
    void setType(lept_type::type _type) { type = _type; };
  };

  class lept_parser {
  private:
    // JSON类型
    lept_value::ptr value;
    // 待识别JSON数据
    const char* json;
  public:
    typedef std::shared_ptr<lept_parser> ptr;
    /**
     * @brief 相等断言，json自增1
     *
     */
    void EXPECT(char _json) {
      assert(*json == _json);
      json++;
    };
    /**
     * @brief 爬取执行函数，执行顺序：先假设结果为LEPT_NULL，lept_parse_whitespace去除空格，lept_parse_value爬取结果
     *
     * @param json JSON字符串
     * @return int JSON爬取结果
     */
    int parse(const char* _json);
    /**
     * @brief 返回解析结果类型
     *
     * @return lept_type::type 解析结果类型
     */
    lept_type::type getType() { return value->getType(); };

    /**
     * @brief 首先检测第一个字符是否为‘n’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
     *
     * @return int 解析是否成功
     */
    int lept_parse_null();
    /**
     * @brief 首先检测第一个字符是否为‘t’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
     *
     * @return int 解析是否成功
     */
    int lept_parse_true();
    /**
     * @brief 去除空格
     *
     * JSON 文本由 3 部分组成，首先是空白（whitespace），接着是一个值，最后是空白。
     * 所谓空白，是由零或多个空格符（space U+0020）、制表符（tab U+0009）、换行符（LF U+000A）、回车符（CR U+000D）所组成。
     */
    void lept_parse_whitespace();
    /**
     * @brief 首先检测第一个字符是否为‘f’，再检测其他字符，不等于null返回LEPT_PARSE_INVALID_VALUE
     *
     * @return int 解析是否成功
     */
    int lept_parse_false();
    /**
     * @brief 爬取字符串内容
     *
     * @return int JSON类型结果
     */
    int lept_parse_value();
    /**
     * @brief Construct a new lept parser object
     *
     * @param _value
     * @param _json
     */
    lept_parser(lept_value::ptr _value);
    /**
     * @brief Destroy the lept parser object
     *
     */
    ~lept_parser() {};
  };
};

#endif /* LEPTJSON_H__ */
