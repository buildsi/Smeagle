// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>

#include "smeagle/smeagle.h"

auto get_one(smeagle::Corpus const& corpus, char const* name) {
  auto funcs = corpus.getFunctions();

  // We only want the "foo" function
  funcs.erase(std::remove_if(funcs.begin(), funcs.end(),
                             [name](smeagle::abi_description const& d) {
                               return d.function_name.find(name) == std::string::npos;
                             }),
              funcs.end());
  return funcs;
}

TEST_CASE("Register Allocation and Types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("one int") {
    auto funcs = get_one(corpus, "test_type_int");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "int");
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("one string") {
    auto funcs = get_one(corpus, "test_type_string");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "string");
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("one char") {
    auto funcs = get_one(corpus, "test_type_char");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "char");
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("one bool") {
    auto funcs = get_one(corpus, "test_type_boolean");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "bool");
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("void") {
    auto funcs = get_one(corpus, "test_type_void");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    REQUIRE(parameters.size() == 0);
  }

  SUBCASE("one wchar_t") {
    auto funcs = get_one(corpus, "test_type_wchar_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "wchar_t");
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("one float") {
    auto funcs = get_one(corpus, "test_type_float");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "float");
    CHECK(parameters[0].location == "%xmm7");
  }

  SUBCASE("one double (double floating point)") {
    auto funcs = get_one(corpus, "test_type_double");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "double");
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  // Unsigned

  SUBCASE("one unsigned int") {
    auto funcs = get_one(corpus, "test_type_unsigned_int");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].type == "unsigned int");
    CHECK(parameters[0].location == "%rdi");
  }
}
