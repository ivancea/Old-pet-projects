#ifndef JSON_HPP
#define JSON_HPP

#include <vector>
#include <map>

namespace JSON {
    enum ValueType{
        Object,
        Array,
        Number,
        String,
        False,
        True,
        Null
    };

    class Value {
        ValueType _type;

        union{
            std::map<std::string, Value> _valueObject;
            std::vector<Value> _valueArray;
            std::string _valueString;
            double _valueNumber;
        };

    public:

        Value();
        Value(const std::string& value);
        Value(const Value& value);
        Value(Value&& value);
        ~Value();

        Value& operator=(const Value& value);
        Value& operator=(Value&& value);

        int loadFromString(const std::string& source, size_t startIndex = 0);

        std::string toString(size_t tabSize = 4, size_t initialTabs = 0, bool tabulateFirstLine = true) const;
        std::string toRawString() const;

        Value& setType(ValueType newType);

        void setUnparsedString(const std::string& source, size_t startIndex = 0, size_t length = std::string::npos);

        ValueType getType() const;

        bool isObject() const;
        bool isArray() const;
        bool isNumber() const;
        bool isString() const;
        bool isFalse() const;
        bool isTrue() const;
        bool isNull() const;

        std::map<std::string, Value>& getObject();
        std::vector<Value>& getArray();
        double& getNumber();
        std::string& getString();

    };
}

#endif // JSON_HPP
