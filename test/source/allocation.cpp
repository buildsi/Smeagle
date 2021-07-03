// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include <string.h>

#include "smeagle/smeagle.h"

auto const& get_one(smeagle::Corpus const& corpus, char const* name) {
  auto const& funcs = corpus.getFunctions();

  return *std::find_if(funcs.begin(), funcs.end(),
					 [name](smeagle::abi_description const& d) {
					   return d.function_name == name;
					 });
}

TEST_CASE("Register Allocation String and char types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("string") {
    auto funcs = get_one(corpus, "test_stringchar_string");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("char") {
    auto funcs = get_one(corpus, "test_stringchar_char");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("wchar_t") {
    auto funcs = get_one(corpus, "test_stringchar_wchar_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Signed String and char types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("signed char") {
    auto funcs = get_one(corpus, "test_signed_char");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Unsigned String and char types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("unsigned char") {
    auto funcs = get_one(corpus, "test_unsigned_char");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Float types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("float") {
    auto funcs = get_one(corpus, "test_float_float");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%xmm0");
  }

  SUBCASE("double (double floating point)") {
    auto funcs = get_one(corpus, "test_float_double");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
}

TEST_CASE("Register Allocation Unsigned Integral types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("unsigned int") {
    auto funcs = get_one(corpus, "test_integral_unsigned_int");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Signed Integral types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("signed int") {
    auto funcs = get_one(corpus, "test_signedintegral_signed_int");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Null types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("void") {
    auto funcs = get_one(corpus, "test_null_void");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    REQUIRE(parameters.size() == 0);
  }
}

TEST_CASE("Register Allocation Boolean types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("bool") {
    auto funcs = get_one(corpus, "test_boolean_boolean");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation Fixed Width Integer types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("int8_t") {
    auto funcs = get_one(corpus, "test_type_int8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int16_t") {
    auto funcs = get_one(corpus, "test_type_int16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int32_t") {
    auto funcs = get_one(corpus, "test_type_int32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int64_t") {
    auto funcs = get_one(corpus, "test_type_int64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast8_t") {
    auto funcs = get_one(corpus, "test_type_int_fast8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast16_t") {
    auto funcs = get_one(corpus, "test_type_int_fast16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("int_fast32_t") {
    auto funcs = get_one(corpus, "test_type_int_fast32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("int_fast64_t") {
    auto funcs = get_one(corpus, "test_type_int_fast64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("int_least8_t") {
    auto funcs = get_one(corpus, "test_type_int_least8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least16_t") {
    auto funcs = get_one(corpus, "test_type_int_least16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least32_t") {
    auto funcs = get_one(corpus, "test_type_int_least32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least64_t") {
    auto funcs = get_one(corpus, "test_type_int_least64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("uint8_t") {
    auto funcs = get_one(corpus, "test_type_uint8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint16_t") {
    auto funcs = get_one(corpus, "test_type_uint16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint32_t") {
    auto funcs = get_one(corpus, "test_type_uint32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint64_t") {
    auto funcs = get_one(corpus, "test_type_uint64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast8_t") {
    auto funcs = get_one(corpus, "test_type_uint_fast8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast16_t") {
    auto funcs = get_one(corpus, "test_type_uint_fast16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("uint_fast32_t") {
    auto funcs = get_one(corpus, "test_type_uint_fast32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("uint_fast64_t") {
    auto funcs = get_one(corpus, "test_type_uint_fast64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("uint_least8_t") {
    auto funcs = get_one(corpus, "test_type_uint_least8_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least16_t") {
    auto funcs = get_one(corpus, "test_type_uint_least16_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least32_t") {
    auto funcs = get_one(corpus, "test_type_uint_least32_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least64_t") {
    auto funcs = get_one(corpus, "test_type_uint_least64_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("intmax_t") {
    auto funcs = get_one(corpus, "test_type_intmax_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("intptr_t") {
    auto funcs = get_one(corpus, "test_type_intptr_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("uintmax_t") {
    auto funcs = get_one(corpus, "test_type_uintmax_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uintptr_t") {
    auto funcs = get_one(corpus, "test_type_uintptr_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
}

TEST_CASE("Register Allocation Integral types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("int") {
    auto funcs = get_one(corpus, "test_single_int");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("short") {
    auto funcs = get_one(corpus, "test_type_short");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("long") {
    auto funcs = get_one(corpus, "test_type_single_long");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("long long") {
    auto funcs = get_one(corpus, "test_type_long_long");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }

  SUBCASE("size_t") {
    auto funcs = get_one(corpus, "test_type_size_t");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
}
