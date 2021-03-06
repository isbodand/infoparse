/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019. 05. 02..
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>
#include "../include/info/parse/utils.hpp"

#include <vector>
#include <string>

using namespace info::parse;

BOOST_AUTO_TEST_SUITE(Test_Utils)

  BOOST_AUTO_TEST_CASE(Test_Utils_MonolithArgsWithoutInnerSpace) {
      int _argc = 3;
      char* _argv[]{const_cast<char*>("abc"),
                    const_cast<char*>("asd"),
                    const_cast<char*>("def")};
      BOOST_CHECK_EQUAL(makeMonolithArgs(_argc, _argv), " abc asd def ");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_MonolithArgsWithInnerSpace) {
      int _argc = 3;
      char* _argv[]{const_cast<char*>("abc cba"),
                    const_cast<char*>("asd     dsa"),
                    const_cast<char*>("def$fed")};
      BOOST_CHECK_EQUAL(makeMonolithArgs(_argc, _argv), " abc$1$cba asd$5$dsa def\\$fed ");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrDoesNotContainSpaces) {
      std::string spacey("asd asd      asd2");
      itrStr(spacey);
      BOOST_CHECK_EQUAL(spacey.find(' '), std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrEscapes$Correctly) {
      std::string spacey("$asd asd      asd2");
      itrStr(spacey);
      BOOST_CHECK_EQUAL(spacey.substr(0, 2), "\\$");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrCountsSpacesCorrectly) {
      std::string spacey("asd      asd2");
      itrStr(spacey);
      BOOST_CHECK_EQUAL(spacey, "asd$6$asd2");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ArcItrStrIsInverseOfItrStr) {
      std::string spacey("asd asd      asd2");
      std::string spacey_2("asd asd      asd2");
      itrStr(spacey);
      arcItrStr(spacey);
      BOOST_CHECK_EQUAL(spacey, spacey_2);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrDoesNotContainSpacesW) {
      std::wstring spacey(L"asd asd      asd2");
      itrStr(spacey);
      BOOST_CHECK_EQUAL(spacey.find(L' '), std::wstring::npos);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrEscapes$CorrectlyW) {
      std::wstring spacey(L"$asd asd      asd2");
      itrStr(spacey);
      BOOST_CHECK(spacey.substr(0, 2) == L"\\$");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrCountsSpacesCorrectlyW) {
      std::wstring spacey(L"asd      asd2");
      itrStr(spacey);
      BOOST_CHECK(spacey == L"asd$6$asd2");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ArcItrStrIsInverseOfItrStrW) {
      std::wstring spacey(L"asd asd      asd2");
      std::wstring spacey_2(L"asd asd      asd2");
      itrStr(spacey);
      arcItrStr(spacey);
      BOOST_CHECK(spacey == spacey_2);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_StringWithExactSpaceMarkerformatIsEscapedCorrectly) {
      std::string dollary("asd asd$4$asd2");
      itrStr(dollary);
      BOOST_CHECK_EQUAL(dollary, R"(asd$1$asd\$4\$asd2)");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_StringWithExactSpaceMarkerformatIsArcosedCorrectly) {
      std::string dollary("asd asd$4$asd2");
      itrStr(dollary);
      arcItrStr(dollary);
      BOOST_CHECK_EQUAL(dollary, "asd asd$4$asd2");
  }

  class Base {
  };

  class Sub : public Base {
  };

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_extends) {
      bool ret = detail::extends_v<Base, Sub>;
      BOOST_CHECK(ret);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_extends_false) {
      bool ret = detail::extends_v<Sub, Base>;
      BOOST_CHECK(!ret);
  }

  class BaseFactory {
  public:
      Base manufacture(int, int);
  };

  class SubFactory {
  public:
      Sub manufacture();
  };

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct_with) {
      if constexpr (detail::can_construct_with_v<BaseFactory, Base, int, int>)
          BOOST_CHECK(true);
      else
          BOOST_CHECK(false);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct) {
      if constexpr (detail::can_construct_v<SubFactory, Sub>)
          BOOST_CHECK(true);
      else
          BOOST_CHECK(false);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct_with_false) {
      if constexpr (detail::can_construct_with_v<BaseFactory, Sub, int, int>)
          BOOST_CHECK(false);
      else
          BOOST_CHECK(true);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct_false) {
      if constexpr (detail::can_construct_v<SubFactory, Base>)
          BOOST_CHECK(false);
      else
          BOOST_CHECK(true);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_int) {
      bool ret = detail::can_stream_in_v<int>;
      BOOST_CHECK_MESSAGE(ret, "can_stream_in succeeded for built-in type `int`");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_bool) {
      bool ret = detail::can_stream_in_v<bool>;
      BOOST_CHECK_MESSAGE(ret, "can_stream_in succeeded for built-in type `bool`");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_out_int) {
      bool ret = detail::can_stream_out_v<int>;
      BOOST_CHECK_MESSAGE(ret, "can_stream_out succeeded for built-in type `int`");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_out_bool) {
      bool ret = detail::can_stream_out_v<bool>;
      BOOST_CHECK_MESSAGE(ret, "can_stream_out succeeded for built-in type `bool`");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_FinderFindsStuff) {
      std::string parser("haystack needle haystack");
      std::string parsee("needle");

      ShittyFinder<std::string::iterator> finder(parser.begin(), parser.end());

      auto[f, l] = finder(parsee.begin(), parsee.end());

      auto dist = std::distance(f, l);
      BOOST_CHECK_EQUAL(dist, parsee.size());
      std::ostringstream ss;

      for (auto it = f; it != l; ++it) {
          ss << *it;
      }
      BOOST_CHECK_EQUAL(ss.str(), parsee);
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_FinderDoesntFindNonExistingStuff) {
      std::string parser("haystack needle haystack");
      std::string parsee("cocaine");

      ShittyFinder<std::string::iterator> finder(parser.begin(), parser.end());

      auto[f, l] = finder(parsee.begin(), parsee.end());

      auto dist = std::distance(f, l);
      BOOST_CHECK_EQUAL(dist, 0);
      std::ostringstream ss;

      for (auto it = f; it != l; ++it) {
          ss << *it;
      }
      BOOST_CHECK_EQUAL(ss.str(), "");
  }

  BOOST_AUTO_TEST_CASE(Test_Utils_FinderFindsTheEmptyString) {
      std::string parser("haystack needle haystack");
      std::string parsee;

      ShittyFinder<std::string::iterator> finder(parser.begin(), parser.end());

      auto[f, l] = finder(parsee.begin(), parsee.end());

      auto dist = std::distance(f, l);
      BOOST_CHECK_EQUAL(dist, 0);
      std::ostringstream ss;

      for (auto it = f; it != l; ++it) {
          ss << *it;
      }
      BOOST_CHECK_EQUAL(ss.str(), "");
  }

BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop

