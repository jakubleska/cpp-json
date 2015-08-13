
#ifndef JSON_VALUE_T_H
#define JSON_VALUE_T_H

#include<map>
#include<vector>
#include<exception>
#include<iostream>
#include <utility>
#include<iterator>
#include <initializer_list>



class json_error : public std::runtime_error {
public:
     explicit json_error(const std::string& __arg):
        std::runtime_error(__arg)
     {}

     virtual const char* what(){
         return std::runtime_error::what();
     }
};



namespace {

template<class IT>
bool it_cmp(IT it, IT end, std::string val){
    std::size_t i=0;
    for(;i<val.size() && it != end;i++,it++){
        if(*it!=val[i]){
            return false;
        }
    }
    return i==val.size();
}

template<class IT>
std::string it_tostring(IT it, const IT& end, std::size_t len){
    std::string temp;
    for(std::size_t i=0;i<len && it!=end;i++,it++){
    temp+=*it;
    }
    return temp;
}

}


class json_value_t
{

    enum json_value_type_e 
    {
        JSON_NULL,
        JSON_BOOL,
        JSON_NUM,
        JSON_STR,
        JSON_ARR,
        JSON_OBJ
    };

public:

    typedef std::vector<json_value_t> arr_t;
    typedef std::map<std::string,json_value_t> obj_t;

private:
    obj_t obj;
    arr_t arr;
    std::string str;
    double number;
    bool boolean;
    json_value_type_e type;

public:

    json_value_t(const obj_t val):obj(val),type(json_value_t::JSON_OBJ){}
    json_value_t(const std::initializer_list<std::pair<std::string,json_value_t>> val):obj(),type(json_value_t::JSON_OBJ){obj.insert(val.begin(), val.end());}
    json_value_t(const arr_t val):arr(val),type(json_value_t::JSON_ARR){}
    json_value_t(const std::string& val):str(val),type(json_value_t::JSON_STR){}
    json_value_t(const char* val):str(val),type(json_value_t::JSON_STR){}
    json_value_t(const double val):number(val),type(json_value_t::JSON_NUM){}
    json_value_t(const int val):json_value_t((double)val){}
    json_value_t(const long val):json_value_t((double)val){}
    json_value_t(const bool val):boolean(val),type(json_value_t::JSON_BOOL){}
    json_value_t():type(json_value_t::JSON_NULL){}

    std::string to_string();

    static json_value_t parse(std::string value);
    static json_value_t parse(std::string::iterator& it,const std::string::iterator& end);
    static std::string encode_string(const std::string& value);

//     template <typename Iterator>
//     static json_value_t parse(Iterator& it,const Iterator& end);
//
//     #include "json_value_t.i"

    json_value_t operator[](std::size_t index);
    json_value_t operator[](std::string index);

    bool isNull();
    operator bool();


    template<class T>T value();


    #include "json_value_t.i"
    /*
    template<class IT>
    static json_value_t iter_parse(IT it, const IT end, int max=5);
    */


};

template<> int json_value_t::value(){
    if(type == JSON_NUM){
        return number;
    }
    throw json_error("Invalid type");
}

template<> float json_value_t::value(){
    if(type == JSON_NUM){
        return number;
    }
    throw json_error("Invalid type");
}

template<> double json_value_t::value(){
    if(type == JSON_NUM){
        return number;
    }
    throw json_error("Invalid type");
}

template<> std::string json_value_t::value(){
    if(type == JSON_STR){
        return str;
    }
    throw json_error("Invalid type");
}

template<> bool json_value_t::value(){
    if(type == JSON_BOOL){
        return boolean;
    }
    throw json_error("Invalid type");
}

template<> json_value_t::arr_t json_value_t::value(){
    if(type == JSON_ARR){
        return arr;
    }
    throw json_error("Invalid type");
}

template<> json_value_t::obj_t json_value_t::value(){
    if(type == JSON_OBJ){
        return obj;
    }
    throw json_error("Invalid type");
}


namespace JS{
    typedef std::initializer_list<json_value_t> array;
    typedef json_value_t::obj_t object;
}

#endif // JSON_VALUE_T_H
