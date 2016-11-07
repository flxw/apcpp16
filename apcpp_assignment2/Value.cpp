#include "Value.h"

#include <stdexcept>

using namespace std;


Value::Value(bool b)
: m_b(b)
, m_type(ValueType::BOOL) {
}

Value::Value(int32_t si)
: m_i{ si }
, m_type{ ValueType::INTEGER } {
}

Value::Value(float f)
: m_f{ f }
, m_type{ ValueType::FLOAT } {
}

Value::Value(const char * s)
: m_s{ s }
, m_type{ ValueType::STRING } {
}

Value::Value(const string & s)
: m_s{ s }
, m_type{ ValueType::STRING } {
}

Value::Value(const Value & rhs) {
    m_type = rhs.m_type;

    switch (m_type) {
        case ValueType::BOOL:
            m_b = rhs.m_b;
            break;
        case ValueType::INTEGER:
            m_i = rhs.m_i;
            break;
        case ValueType::FLOAT:
            m_f = rhs.m_f;
            break;
        case ValueType::STRING:
            new(&m_s) string(rhs.m_s);
            break;
    }
}


Value::Value(Value && rhs) {
    m_type = rhs.m_type;

    switch (m_type) {
        case ValueType::BOOL:
            m_b = rhs.m_b;
            break;
        case ValueType::INTEGER:
            m_i = rhs.m_i;
            break;
        case ValueType::FLOAT:
            m_f = rhs.m_f;
            break;
        case ValueType::STRING:
            new(&m_s) string(move(rhs.m_s));
            break;
    }
}


Value::~Value() {
    if (m_type == ValueType::STRING) {
        m_s.~string();
    }
};


Value & Value::operator=(const Value & rhs) {
    if (this == &rhs) {
        return *this;
    }

    if (m_type == ValueType::STRING) {
        m_s.~string();
    }

    m_type = rhs.m_type;

    switch (m_type) {
        case ValueType::BOOL:
            m_b = rhs.m_b;
            break;
        case ValueType::INTEGER:
            m_i = rhs.m_i;
            break;
        case ValueType::FLOAT:
            m_f = rhs.m_f;
            break;
        case ValueType::STRING:
            new(&m_s) string(rhs.m_s);
            break;
    }

    return *this;
}


Value & Value::operator=(Value && rhs) {
    if (this == &rhs) {
        return *this;
    }

    if (m_type == ValueType::STRING) {
        m_s.~string();
    }

    m_type = rhs.m_type;

    switch (m_type) {
        case ValueType::BOOL:
            m_b = rhs.m_b;
            break;
        case ValueType::INTEGER:
            m_i = rhs.m_i;
            break;
        case ValueType::FLOAT:
            m_f = rhs.m_f;
            break;
        case ValueType::STRING:
            new(&m_s) string(move(rhs.m_s));
            break;
    }

    return *this;
}


ValueType Value::type() const {
    return m_type;
}


bool Value::asBool() const {
    if (m_type != ValueType::BOOL) {
        throw runtime_error("type mismatch");
    }

    return m_b;
}


int32_t Value::asInt() const {
    if (m_type != ValueType::INTEGER) {
        throw runtime_error("type mismatch");
    }

    return m_i;
}


float Value::asFloat() const {
    if (m_type != ValueType::FLOAT) {
        throw runtime_error("type mismatch");
    }

    return m_f;
}


string Value::asString() const {
    if (m_type != ValueType::STRING) {
        throw runtime_error("type mismatch");
    }

    return m_s;
}


bool Value::operator==(const Value & rhs) const {
    if (m_type != rhs.m_type) {
        return false;
    }

    switch (m_type) {
        case ValueType::BOOL:
            return m_b == rhs.m_b;
        case ValueType::INTEGER:
            return m_i == rhs.m_i;
        case ValueType::FLOAT:
            return m_f == rhs.m_f;
        case ValueType::STRING:
            return m_s == rhs.m_s;
    }

    return false;
}
