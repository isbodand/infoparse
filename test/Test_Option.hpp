//
// Created by bodand on 2019. 05. 01..
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>
#include "../_Option.hpp"

using namespace InfoParse;

BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithDeductedShortName) {
    int i;
    auto opt = new _Option<decltype(i)>("test", &i);
    BOOST_CHECK_NE(opt, nullptr);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithExplicitShortName) {
    int i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    BOOST_CHECK_NE(opt, nullptr);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameSole) {
    int i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text --test 4 text");
    BOOST_CHECK_EQUAL(i, 4);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameSole) {
    int i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text -c 4 text");
    BOOST_CHECK_EQUAL(i, 4);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameSole) {
    std::string s;
    auto opt = new _Option<decltype(s)>("test", 'c', &s);
    opt->match("text --test string text");
    BOOST_CHECK_EQUAL(s, "string");
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameSole) {
    std::string s;
    auto opt = new _Option<decltype(s)>("test", 'c', &s);
    opt->match("text -c string text");
    BOOST_CHECK_EQUAL(s, "string");
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameCherryPick) {
    int i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text --test 42 -w whatever -o inflate --exclude humanity text");
    BOOST_CHECK_EQUAL(i, 42);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameCherryPick) {
    int i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text --explode -c 42 -w whatever -o inflate text");
    BOOST_CHECK_EQUAL(i, 42);
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameCherryPick) {
    std::string i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text --test cocaine -w whatever -o inflate --exclude humanity text");
    BOOST_CHECK_EQUAL(i, "cocaine");
    delete opt;
}

BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameCherryPick) {
    std::string i;
    auto opt = new _Option<decltype(i)>("test", 'c', &i);
    opt->match("text --explode -c apple -w whatever -o inflate text");
    BOOST_CHECK_EQUAL(i, "apple");
    delete opt;
}

#pragma clang diagnostic pop
