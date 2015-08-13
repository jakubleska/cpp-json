#include "json_value_t.h"

#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>

std::string json_value_t::encode_string(const std::string& value){

  std::string temp;
  const std::string replecable{"\\/\t\n\r\f\b"};
  for(std::size_t i=0;i<value.length();i++){
    switch(value[i]){
        case '\\':
            temp+="\\\\";
            break;
        case '/':
            temp+="\\/";
            break;
        case '\t':
            temp+="\\t";
            break;
        case '\n':
            temp+="\\n";
            break;
        case '\r':
            temp+="\\r";
            break;
        case '\f':
            temp+="\\f";
            break;
        case '\b':
            temp+="\\b";
            break;
        case '\"':
            temp+="\\\"";
            break;
        default:
            temp+=value[i];
            break;
    }
  }
  return temp;
}

std::string json_value_t::to_string(){
  switch(type){
      case JSON_NULL:
          return "null";
      case JSON_BOOL:
          return boolean?"true":"false";
      case JSON_NUM:{
          std::stringstream ss;
          ss<<number;
          return ss.str();
      }
      case JSON_STR:
          return std::string("\"")+encode_string(str)+"\"";
      case JSON_ARR:{
          std::string temp{'['};
          for(auto it=arr.begin();it!=arr.end();it++){
              temp+=it->to_string();
              if(std::next(it, 1)!=arr.end()){
                  temp+=", ";
              }
          }
          return temp+']';
      }
      case JSON_OBJ:{
          std::string temp{'{'};
          for(auto it=obj.begin();it!=obj.end();it++){
              temp+=std::string("\"")+json_value_t::encode_string(it->first)+"\":"+it->second.to_string();
              if(std::next(it, 1)!=obj.end()){
                  temp+=", ";
              }
          }
          return temp+'}';
      }
      default:
          throw json_error("implementation error");
          return "";
  }
}

enum token_type_e 
{
    T_OBJ_BEG,
    T_OBJ_END,
    T_ARR_BEG,
    T_ARR_END,
    T_COLON,
    T_NUMBER,
    T_STRING,
    T_BOOL,
    T_NULL
};


json_value_t json_value_t::parse(std::string val){

    auto it=val.begin(),end=val.end();
    return iter_parse<std::string::iterator&>(it,end);
}





json_value_t json_value_t::operator[](std::string index){

    if(type!=JSON_OBJ)throw json_error("Invalid type, cant convert to object");
    if(obj.find(index)==obj.end()) throw json_error("Unknown index, cant get");
    return obj[index];
}

json_value_t json_value_t::operator[](std::size_t index){

    if(type!=JSON_ARR)throw json_error("Invalid type, cant convert to array");
    if(arr.size()<index) throw json_error("Index out of range");
    return arr[index];
}


bool json_value_t::isNull(){
    return type==JSON_NULL;
}

json_value_t::operator bool(){
    if(type == JSON_NULL)
        return false;
    if(type == JSON_BOOL)
        return boolean;
    if(type == JSON_STR)
        return !str.empty();
    if(type == JSON_NUM)
        return (bool)number;
    if(type == JSON_ARR)
        return arr.size();
    if(type == JSON_OBJ)
        return obj.size();
    return false;
}






