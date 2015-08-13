#include <iostream>
#include "../json_value_t.h"
#include "gtest/gtest.h"



TEST(init, null){
    json_value_t jsv;

    EXPECT_EQ(jsv.isNull(),true);
    EXPECT_EQ(jsv.to_string(),"null");
    EXPECT_FALSE(jsv);
}


TEST(init, bool_false){
    json_value_t jsv(false);

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"false");
    EXPECT_FALSE(jsv);
}


TEST(init, bool_true){
    json_value_t jsv(true);

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"true");
    EXPECT_TRUE(jsv);
}

TEST(init, num_positive){
    json_value_t jsv(1);

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"1");
    EXPECT_TRUE(jsv);
}

TEST(init, num_negative){
    json_value_t jsv(0);

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"0");
    EXPECT_FALSE(jsv);
}

TEST(init, num_float){
    json_value_t jsv(1.25);

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"1.25");
    EXPECT_TRUE(jsv);
}

TEST(init, arr_empty){
    json_value_t jsv(json_value_t::arr_t{});

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"[]");
    EXPECT_FALSE(jsv);
}

TEST(init, arr){
    json_value_t jsv(json_value_t::arr_t{"first","second"});

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"[\"first\", \"second\"]");
    EXPECT_TRUE(jsv);
}

TEST(init, obj_empty){
    json_value_t jsv({});

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"{}");
    EXPECT_FALSE(jsv);
}

TEST(init, obj){
    json_value_t jsv({{"a",1},{"b",2}});

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"{\"a\":1, \"b\":2}");
    EXPECT_TRUE(jsv);
}

TEST(init, complex){
    json_value_t jsv({{"a",json_value_t::arr_t{"a",1,3}},{"b",2}});

    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.to_string(),"{\"a\":[\"a\", 1, 3], \"b\":2}");
    EXPECT_TRUE(jsv);
}










TEST(parse,null){
    json_value_t jsv = json_value_t::parse("null");
    EXPECT_EQ(jsv.isNull(),true);
    EXPECT_THROW(jsv.value<int>(),json_error);
    EXPECT_EQ(jsv.to_string(),"null");
    EXPECT_FALSE(jsv);
}

TEST(parse,boolean_false){
    json_value_t jsv = json_value_t::parse("false");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_FALSE(jsv.value<bool>());
    EXPECT_FALSE(jsv);
}

TEST(parse,boolean_true){
    json_value_t jsv = json_value_t::parse("true");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_TRUE(jsv.value<bool>());
    EXPECT_TRUE(jsv);
}

TEST(parse, number_int_zero){
    json_value_t jsv = json_value_t::parse("0");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<int>(),0);
    EXPECT_FALSE(jsv);
}

TEST(parse, number_float_zero){
    json_value_t jsv = json_value_t::parse("0.0");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<double>(),0);
    EXPECT_FALSE(jsv);
}

TEST(parse, number_int){
    json_value_t jsv = json_value_t::parse("1");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<int>(),1);
    EXPECT_TRUE(jsv);
}

TEST(parse, number_float){
    json_value_t jsv = json_value_t::parse("1.25");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<double>(),1.25);
    EXPECT_TRUE(jsv);
}

TEST(parse, number_exponent){
    json_value_t jsv = json_value_t::parse("1.25e5");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<int>(),125000);
    EXPECT_TRUE(jsv);
}

TEST(parse,string_empty){
    json_value_t jsv = json_value_t::parse("\"\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"");
    EXPECT_FALSE(jsv);
}

TEST(parse,string){
    json_value_t jsv = json_value_t::parse("\"text\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text");
    EXPECT_TRUE(jsv);
}

TEST(parse,string_escape){
    json_value_t jsv = json_value_t::parse("\"text\\\"with\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text\"with");
    EXPECT_TRUE(jsv);
}

TEST(parse,string_escape_backslash){
    json_value_t jsv = json_value_t::parse("\"text\\\\with\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text\\with");
    EXPECT_TRUE(jsv);
}

TEST(DISABLED_escapeString,string_escape_tab){
    json_value_t jsv = json_value_t::parse("\"text\\twith\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text\twith");
    EXPECT_TRUE(jsv);
}

TEST(DISABLED_escapeString,string_escape_newline){
    json_value_t jsv = json_value_t::parse("\"text\\nwith\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text\nwith");
    EXPECT_TRUE(jsv);
}

TEST(DISABLED_escapeString,string_escape_newline_win){
    json_value_t jsv = json_value_t::parse("\"text\\r\\nwith\"");
    EXPECT_EQ(jsv.isNull(),false);
    EXPECT_EQ(jsv.value<std::string>(),"text\r\nwith");
    EXPECT_TRUE(jsv);
}


int main(int argc, char **argv) {
    std::cout << "Running main() from gtest_main.cc\n";

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}