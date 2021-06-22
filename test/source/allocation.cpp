// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include <string.h>

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

auto corpus = smeagle::Smeagle("liballocation.so").parse();

TEST_CASE("Register Allocation - Integral Types") {
  SUBCASE("bool") {
    auto funcs = get_one(corpus, "test_bool");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("char") {
    auto funcs = get_one(corpus, "test_char");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int") {
    auto funcs = get_one(corpus, "test_int__");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("short") {
    auto funcs = get_one(corpus, "test_short");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("long") {
    auto funcs = get_one(corpus, "test_long");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("long long") {
    auto funcs = get_one(corpus, "test_LONG_LONG");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("size_t") {
    auto funcs = get_one(corpus, "test_size_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation - Signed Integral Types") {
  SUBCASE("signed") {
    auto funcs = get_one(corpus, "test_signed__");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("signed char") {
    auto funcs = get_one(corpus, "test_signed_char");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("signed int") {
    auto funcs = get_one(corpus, "test_signed_int");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("signed short") {
    auto funcs = get_one(corpus, "test_signed_short");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("signed long") {
    auto funcs = get_one(corpus, "test_signed_long");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("signed long long") {
    auto funcs = get_one(corpus, "test_signed_LONG_LONG");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation - Unsigned Integral Types") {
  SUBCASE("unsigned") {
    auto funcs = get_one(corpus, "test_unsigned__");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("unsigned char") {
    auto funcs = get_one(corpus, "test_unsigned_char");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("unsigned int") {
    auto funcs = get_one(corpus, "test_unsigned_int");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("unsigned short") {
    auto funcs = get_one(corpus, "test_unsigned_short");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("unsigned long") {
    auto funcs = get_one(corpus, "test_unsigned_long");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("unsigned long long") {
    auto funcs = get_one(corpus, "test_unsigned_LONG_LONG");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation - Floating Point types") {
  SUBCASE("float") {
    auto funcs = get_one(corpus, "test_float");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%xmm0");
  }
  SUBCASE("double") {
    auto funcs = get_one(corpus, "test_double");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
  SUBCASE("double") {
    auto funcs = get_one(corpus, "test_LONG_DOUBLE");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
  SUBCASE("complex float") {
    auto funcs = get_one(corpus, "test_complex_float");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
  SUBCASE("complex double") {
    auto funcs = get_one(corpus, "test_complex_double");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
  SUBCASE("complex long double") {
    auto funcs = get_one(corpus, "test_complex_LONG_DOUBLE");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "framebase+8|framebase+16");
  }
}

TEST_CASE("Register Allocation - Fixed-Width Integer types") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("int8_t") {
    auto funcs = get_one(corpus, "test_int8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int16_t") {
    auto funcs = get_one(corpus, "test_int16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int32_t") {
    auto funcs = get_one(corpus, "test_int32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int64_t") {
    auto funcs = get_one(corpus, "test_int64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast8_t") {
    auto funcs = get_one(corpus, "test_int_fast8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast16_t") {
    auto funcs = get_one(corpus, "test_int_fast16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast32_t") {
    auto funcs = get_one(corpus, "test_int_fast32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_fast64_t") {
    auto funcs = get_one(corpus, "test_int_fast64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least8_t") {
    auto funcs = get_one(corpus, "test_int_least8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least16_t") {
    auto funcs = get_one(corpus, "test_int_least16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least32_t") {
    auto funcs = get_one(corpus, "test_int_least32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("int_least64_t") {
    auto funcs = get_one(corpus, "test_int_least64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint8_t") {
    auto funcs = get_one(corpus, "test_uint8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint16_t") {
    auto funcs = get_one(corpus, "test_uint16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint32_t") {
    auto funcs = get_one(corpus, "test_uint32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint64_t") {
    auto funcs = get_one(corpus, "test_uint64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast8_t") {
    auto funcs = get_one(corpus, "test_uint_fast8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast16_t") {
    auto funcs = get_one(corpus, "test_uint_fast16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast32_t") {
    auto funcs = get_one(corpus, "test_uint_fast32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_fast64_t") {
    auto funcs = get_one(corpus, "test_uint_fast64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least8_t") {
    auto funcs = get_one(corpus, "test_uint_least8_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least16_t") {
    auto funcs = get_one(corpus, "test_uint_least16_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least32_t") {
    auto funcs = get_one(corpus, "test_uint_least32_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uint_least64_t") {
    auto funcs = get_one(corpus, "test_uint_least64_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("intmax_t") {
    auto funcs = get_one(corpus, "test_intmax_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("intptr_t") {
    auto funcs = get_one(corpus, "test_intptr_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uintmax_t") {
    auto funcs = get_one(corpus, "test_uintmax_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }

  SUBCASE("uintptr_t") {
    auto funcs = get_one(corpus, "test_uintptr_t");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}

TEST_CASE("Register Allocation - Null types") {
   SUBCASE("void") {
    auto funcs = get_one(corpus, "test_null_void");
    CHECK(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters.size() == 0);
  }
}

TEST_CASE("Register Allocation - UTF types") {
  SUBCASE("wchar_t") {
    auto funcs = get_one(corpus, "test_WCHAR_T");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("char16_t") {
    auto funcs = get_one(corpus, "test_CHAR16_T");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("char32_t") {
    auto funcs = get_one(corpus, "test_CHAR32_T");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}
