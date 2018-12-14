#include "JSON.hpp"

namespace JSON{
    Value::Value():_type(Null){}

    Value::Value(const std::string& value):_type(Null){
        loadFromString(value);
    }

    Value::Value(const Value& value):_type(Null){
        *this = value;
    }

    Value::Value(Value&& value):_type(Null){
        *this = std::move(value);
    }

    Value::~Value(){
        setType(Null);
    }

    Value& Value::operator=(const Value& value){
        setType(value._type);

        switch(_type){
        case Object:
            _valueObject = value._valueObject;
            break;
        case Array:
            _valueArray = value._valueArray;
            break;
        case Number:
            _valueNumber = value._valueNumber;
            break;
        case String:
            _valueString = value._valueString;
            break;
        }

        return *this;
    }

    Value& Value::operator=(Value&& value){
        setType(value._type);

        switch(_type){
        case Object:
            _valueObject = std::move(value._valueObject);
            break;

        case Array:
            _valueArray = std::move(value._valueArray);
            break;

        case Number:
            _valueNumber = value._valueNumber;
            break;

        case String:
            _valueString = std::move(value._valueString);
            break;
        }

        value.setType(Null);

        return *this;
    }

/**

    {
        "a": "hola",
        "b": 123,
        "c": true,
        "d": false,
        "e": null,
        "f": { "a" :0.1e-21},
        "g": [ 1, []]
    }

**/
    int Value::loadFromString(const std::string& source, size_t startIndex){
        setType(Null);

        size_t pos = source.find_first_not_of(" \t\r\n", startIndex);

        if(pos==std::string::npos){
            return -1;
        }

        char* tempChar;
        size_t tempPos;
        Value tempValue;
        Value tempValue2;

        switch(source[pos]){
        case 'f':
            if(source.compare(pos,5, "false") != 0){
                return -1;
            }

            setType(False);

            pos += 5;

            break;

        case 't':
            if(source.compare(pos,4, "true") != 0){
                return -1;
            }

            setType(True);

            pos += 4;

            break;

        case 'n':
            if(source.compare(pos,4, "null") != 0){
                return -1;
            }

            setType(Null);

            pos += 4;

            break;

        case '0':case '1':case '2':
        case '3':case '4':case '5':
        case '6':case '7':case '8':
        case '9':case '-':
            setType(Number);

            tempPos = source.find_first_not_of("0123456789+-eE.", pos);

            if(tempPos == std::string::npos){
                tempPos = source.size();
            }

            _valueNumber = strtod(source.c_str()+pos, &tempChar);

            if(tempPos != tempChar-source.c_str()){
                return -1;
            }

            pos = tempPos;

            break;

        case '"':
            tempPos = pos;

            while(true){
                tempPos = source.find('"', tempPos+1);

                if(tempPos == std::string::npos){
                    return -1;
                }

                int slashCount = 0;

                while(source[tempPos-slashCount-1] == '\\'){
                    ++slashCount;
                }

                if(slashCount%2==0){
                    break;
                }
            }

            setUnparsedString(source, pos+1, tempPos-pos-1);

            pos = tempPos+1;

            break;

        case '{':
            setType(Object);

            while(source[pos] != '}'){
                pos = source.find_first_not_of(" \t\r\n", pos+1);

                if(pos==std::string::npos){
                    setType(Null);

                    return -1;
                }

                if(source[pos] == '}'){
                    break;
                }

                pos = tempValue.loadFromString(source, pos);

                if(pos==-1 || tempValue.getType() != ValueType::String){
                    setType(Null);

                    return -1;
                }

                pos = source.find_first_not_of(" \t\r\n", pos);

                if(pos==std::string::npos || source[pos] != ':'){
                    setType(Null);

                    return -1;
                }

                pos = tempValue2.loadFromString(source, pos+1);

                if(pos==-1){
                    setType(Null);

                    return -1;
                }

                _valueObject[tempValue.getString()] = std::move(tempValue2);

                pos = source.find_first_not_of(" \t\r\n", pos);

                if(pos==std::string::npos || (source[pos] != ',' && source[pos] != '}')){
                    setType(Null);

                    return -1;
                }
            }

            pos += 1;

            break;

        case '[':
            setType(Array);

            while(source[pos] != ']'){
                pos = source.find_first_not_of(" \t\r\n", pos+1);

                if(pos==std::string::npos){
                    setType(Null);

                    return -1;
                }

                if(source[pos] == ']'){
                    break;
                }

                pos = tempValue.loadFromString(source, pos);

                if(pos==-1){
                    setType(Null);

                    return -1;
                }

                _valueArray.emplace_back(std::move(tempValue));

                pos = source.find_first_not_of(" \t\r\n", pos);

                if(pos==std::string::npos || (source[pos] != ',' && source[pos] != ']')){
                    setType(Null);

                    return -1;
                }

            }

            pos += 1;

            break;

        default:
            return -1;
        }

        return pos;
    }

    std::string Value::toString(size_t tabSize, size_t initialTabs, bool tabulateFirstLine) const{
        std::string tab(tabSize, ' ');
        std::string fullTab(tabSize*initialTabs, ' ');

        std::string temp;

        if(tabulateFirstLine)
            temp += fullTab;

        switch(_type){
        case Null:
            temp += "null";
            break;

        case True:
            temp += "true";
            break;

        case False:
            temp += "false";
            break;

        case Number:
        {
            temp += std::to_string(_valueNumber);
            int i;
            for(i=temp.size()-1; i>=0 && temp[i]=='0'; i--);
            temp.erase(i + (temp[i]=='.'?0:1));
            break;
        }

        case Array:
            if(_valueArray.size()==0){
                temp += "[]";
            }else{
                temp += "[";
                for(int i=0; i<_valueArray.size(); i++){
                    if(i>0)
                        temp += ",";
                    temp += "\n";
                    temp += _valueArray[i].toString(tabSize, initialTabs+1, true);
                }
                temp += "\n" + fullTab + "]";
            }
            break;

        case Object:
            if(_valueObject.size()==0){
                temp += "{}";
            }else{
                temp += "{";
                bool isFirst = true;
                for(auto it : _valueObject){
                    if(!isFirst){
                        temp += ",";
                    }else isFirst = false;
                    temp += "\n" + fullTab + tab;
                    temp += "\"" + it.first + "\": ";
                    temp += it.second.toString(tabSize, initialTabs+1, false);
                }
                temp += "\n" + fullTab + "}";
            }
            break;

        case String:
            temp.reserve(_valueString.size()+2 + (tabulateFirstLine?tabSize*initialTabs:0));
            temp += "\"";
            for(int i=0; i<_valueString.size(); i++){
                switch(_valueString[i]){
                case '"':
                    temp += "\\\"";
                    break;
                case '\n':
                    temp += "\\n";
                    break;
                case '\r':
                    temp += "\\r";
                    break;
                case '\t':
                    temp += "\\t";
                    break;
                case '\f':
                    temp += "\\f";
                    break;
                case '\b':
                    temp += "\\b";
                    break;

                /// TEMPORAL
                case '\xFF':
                    temp += "\\";
                    break;

                default:
                    temp += _valueString[i];
                }
            }

            temp += "\"";
            break;
        }

        return temp;
    }

    std::string Value::toRawString() const{
        std::string temp;

        switch(_type){
        case Null:
            temp += "null";
            break;

        case True:
            temp += "true";
            break;

        case False:
            temp += "false";
            break;

        case Number:
        {
            temp += std::to_string(_valueNumber);
            int i;
            for(i=temp.size()-1; i>=0 && temp[i]=='0'; i--);
            temp.erase(i + (temp[i]=='.'?0:1));
            break;
        }

        case Array:
            if(_valueArray.size()==0){
                temp += "[]";
            }else{
                temp += "[";
                for(int i=0; i<_valueArray.size(); i++){
                    if(i>0)
                        temp += ",";
                    temp += _valueArray[i].toRawString();
                }
                temp += "]";
            }
            break;

        case Object:
            if(_valueObject.size()==0){
                temp += "{}";
            }else{
                temp += "{";
                bool isFirst = true;
                for(auto it : _valueObject){
                    if(!isFirst){
                        temp += ",";
                    }else isFirst = false;
                    temp += "\"" + it.first + "\": ";
                    temp += it.second.toRawString();
                }
                temp += "}";
            }
            break;

        case String:
            temp.reserve(_valueString.size()+2);
            temp += "\"";
            for(int i=0; i<_valueString.size(); i++){
                switch(_valueString[i]){
                case '"':
                    temp += "\\\"";
                    break;
                case '\n':
                    temp += "\\n";
                    break;
                case '\r':
                    temp += "\\r";
                    break;
                case '\t':
                    temp += "\\t";
                    break;
                case '\f':
                    temp += "\\f";
                    break;
                case '\b':
                    temp += "\\b";
                    break;

                /// TEMPORAL
                case '\xFF':
                    temp += "\\";
                    break;

                default:
                    temp += _valueString[i];
                }
            }

            temp += "\"";
            break;
        }

        return temp;
    }

    Value& Value::setType(ValueType newType){
        if(newType != _type){
            switch(_type){
            case Object:
                _valueObject.~map();
                break;

            case Array:
                _valueArray.~vector();
                break;

            case String:
                _valueString.~basic_string();
                break;
            }

            switch(newType){
            case Object:
                new (&_valueObject) std::map<std::string, Value>();
                break;

            case Array:
                new (&_valueArray) std::vector<Value>();
                break;

            case String:
                new (&_valueString) std::string();
                break;

            default:
                _valueNumber = 0;
            }

            _type = newType;
        }

        return *this;
    }

/* STRING
escape (
              %x22 /          ; "    quotation mark  U+0022
              %x5C /          ; \    reverse solidus U+005C
              %x2F /          ; /    solidus         U+002F
              %x62 /          ; b    backspace       U+0008
              %x66 /          ; f    form feed       U+000C
              %x6E /          ; n    line feed       U+000A
              %x72 /          ; r    carriage return U+000D
              %x74 /          ; t    tab             U+0009
              %x75 4HEXDIG )  ; uXXXX                U+XXXX
*/

    void Value::setUnparsedString(const std::string& source, size_t startIndex, size_t length){
        std::string temp;

        setType(String);

        if(length == std::string::npos){
            length = source.size();
        }

        for(int i=startIndex; i<startIndex+length; i++){
            if(source[i] == '\\' && i+1<source.size()){
                switch(source[i+1]){
                case 'b':
                    temp += '\b';
                    break;

                case 'f':
                    temp += '\f';
                    break;

                case 'n':
                    temp += '\n';
                    break;

                case 'r':
                    temp += '\r';
                    break;

                case 't':
                    temp += '\t';
                    break;

                /// TEMPORAL

                case 'u':
                    temp += "\xFFu";
                    break;

                default:
                    temp += source[i+1];
                }

                ++i;
            }else{
                temp += source[i];
            }
        }

        _valueString = std::move(temp);
    }

    ValueType Value::getType() const{
        return _type;
    }

    bool  Value::isObject() const{
        return _type==Object;
    }

    bool  Value::isArray() const{
        return _type==Array;
    }

    bool  Value::isNumber() const{
        return _type==Number;
    }

    bool  Value::isString() const{
        return _type==String;
    }

    bool  Value::isFalse() const{
        return _type==False;
    }

    bool  Value::isTrue() const{
        return _type==True;
    }

    bool  Value::isNull() const{
        return _type==Null;
    }

    std::map<std::string, Value>& Value::getObject(){
        return _valueObject;
    }

    std::vector<Value>& Value::getArray(){
        return _valueArray;
    }

    double& Value::getNumber(){
        return _valueNumber;
    }

    std::string& Value::getString(){
        return _valueString;
    }
}
