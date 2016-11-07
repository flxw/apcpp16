#pragma once

#include <cstdint>
#include <string>


enum class ValueType {
    BOOL,
    INTEGER,
    FLOAT,
    STRING
};


class Value {
public:
    Value(bool b);
    Value(std::int32_t i);
    Value(float f);
    Value(const char * s);
    Value(const std::string & s);

    Value(const Value & rhs);
    Value(Value && rhs);

    ~Value();

    Value & operator=(const Value & rhs);
    Value & operator=(Value && rhs);


public:
    ValueType type() const;

    bool asBool() const;
    std::int32_t asInt() const;
    float asFloat() const;
    std::string asString() const;

    bool operator==(const Value & rhs) const;


private:
    union {
        bool m_b;
        std::int32_t m_i;
        float m_f;
        std::string m_s;
    };

    ValueType m_type;
};
