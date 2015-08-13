template<class IT>
static json_value_t iter_parse(IT it, const IT end){

    json_value_t temp;

    //replace white space
    while(it!=end && std::isspace(*it))it++;
    if(it==end){
        throw json_error("[Parser] Unexpected end of string before json entity");
    }

    //determine type of current object
    switch(*it){
        case '"':{
            //consume "
            it++;
            temp.type = JSON_STR;
            char stack_char = '\0';
            while(*it != '\"' || (*it == '\"' && stack_char=='\\')){
                if(it == end) throw json_error("[Parser]::string unexpectd end of input string inside json string");
                if(*it=='\\'){
                    if(stack_char == '\\'){
                        temp.str.append("\\");
                        stack_char='\0';
                    }
                    else {
                      stack_char = '\\';
                    }
                } else if(*it=='"') {
                    if(stack_char == '\\'){
                        temp.str.append("\"");
                        stack_char='\0';
                    } else {
                        //consume "
                        it++;
                        return temp;
                    }
                } else {
/*
 *                   if(stack_char == '\\'){
 *                     temp.str.push_back('\\');
 *                     stack_char = '\0';
 *                   }
 */
                  temp.str.push_back(*it);
                }
                it++;
            }
            it++;
            return temp;
        }
        case '{':{
            //consume {
            it++;
            temp.type = JSON_OBJ;

            //empty object
            if (*it == '}'){
                *it++;
                return temp;
            }

            while(1){
                json_value_t index = json_value_t::iter_parse<IT>(it,end);
                if(index.type != JSON_STR){
                    throw json_error("[Parser]::object Elements must be indexed by string");
                }
                std::string s_index = index.str;

                //replace white space
                while(it!=end && std::isspace(*it))it++;
                if(it==end){
                    throw json_error("[Parser]::object Unexpected end of inside `key:value` pair");
                }
                if (*it!=':'){
                    throw json_error(std::string("[Parser]:object Unexpected character '")+*it+"'after key `"+s_index+"`");
                }
                //consume :
                it++;

                //replace white space
                while(it!=end && std::isspace(*it))it++;
                if(it==end){
                    throw json_error("[Parser]::object Unexpected end of inside `key:value` pair");
                }

                //insert to map
                temp.obj[s_index] = json_value_t::iter_parse<IT>(it,end);

                if(*it=='}'){
                    return temp;
                } else if (*it!=','){
                    throw json_error(std::string("[Parser]::object Unexpected character '")+*it+"'after "+std::to_string(temp.arr.size())+" element");
                }
                //consume ,
                it++;
            }
        }
        case '[':{
            //consume
            it++;
            temp.type = JSON_ARR;

            //empty array
            if (*it == ']'){
                *it++;
                return temp;
            }

            while(1){
                temp.arr.push_back(json_value_t::iter_parse<IT>(it,end));
                //replace white space
                while(it!=end && std::isspace(*it))it++;
                if(it==end){
                    throw json_error("[Parser]::array Unexpected end of string before json entity");
                }
                if(*it==']'){
                    it++;
                    return temp;
                } else if (*it!=','){
                    throw json_error(std::string("[Parser]::array Unexpected character '")+*it+"'after "+std::to_string(temp.arr.size())+" element");
                }
                //consume ,
                it++;
            }
        }
        case 'n':{
            if(!it_cmp<IT>(it,end,"null")){
              throw json_error(std::string("[Parser]::null Invalid json near ")+it_tostring(it,end,5));
            } else {
              return json_value_t();
            }
        }
        case 't':{
            if(!it_cmp<IT>(it,end,"true")){
              throw json_error(std::string("[Parser]::true Invalid json near ")+it_tostring(it,end,5));
            } else {
              return json_value_t(true);
            }
        }
        case 'f':{
            if(!it_cmp<IT>(it,end,"false")){
              throw json_error(std::string("[Parser]::false Invalid json near ")+it_tostring(it,end,5));
            } else {
              return json_value_t(false);
            }
        }
        default:
            if(std::isdigit(*it) || *it=='-'){
                std::string tmp{*it};
                const std::string allowed_number_parts=".+-eE";
                it++;

                while(std::isdigit(*it) || allowed_number_parts.find(*it)!=std::string::npos){
                    tmp.push_back(*it);
                    it++;
                }

                double tmp_d;
                try{
                    tmp_d = std::stod(tmp);
                } catch (...){
                    throw json_error(std::string("[Parser]::number cant convert `")+tmp+"` to double");
                }
                return json_value_t(tmp_d);

            } else throw json_error(std::string("[Parser] Unknown token type '")+it_tostring(it,end,20)+"'");
    }
}
