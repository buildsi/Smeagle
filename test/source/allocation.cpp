
// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include <string.h>

#include "smeagle/smeagle.h"

auto const& get_one(smeagle::Corpus const& corpus, char const* name) {
  auto const& funcs = corpus.getFunctions();

  return *std::find_if(funcs.begin(), funcs.end(), [name](smeagle::abi_description const& d) {
    return d.function_name == name;
  });
}

namespace {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();
}

TEST_CASE("Register Allocation - Integral Types") {
  SUBCASE("bool") {
    auto func = get_one(corpus, "test_bool");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer8");
  }
  SUBCASE("bool*") {
    auto func = get_one(corpus, "test_ptr_bool");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("bool**") {
    auto func = get_one(corpus, "test_ptr_ptr_bool");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char") {
    auto func = get_one(corpus, "test_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer8");
  }
  SUBCASE("char*") {
    auto func = get_one(corpus, "test_ptr_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char**") {
    auto func = get_one(corpus, "test_ptr_ptr_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("short") {
    auto func = get_one(corpus, "test_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer16");
  }
  SUBCASE("short*") {
    auto func = get_one(corpus, "test_ptr_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("short**") {
    auto func = get_one(corpus, "test_ptr_ptr_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int") {
    auto func = get_one(corpus, "test_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("int*") {
    auto func = get_one(corpus, "test_ptr_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int**") {
    auto func = get_one(corpus, "test_ptr_ptr_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long") {
    auto func = get_one(corpus, "test_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("long*") {
    auto func = get_one(corpus, "test_ptr_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long**") {
    auto func = get_one(corpus, "test_ptr_ptr_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long long") {
    auto func = get_one(corpus, "test_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("long long*") {
    auto func = get_one(corpus, "test_ptr_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long long**") {
    auto func = get_one(corpus, "test_ptr_ptr_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Signed Integral Types") {
  SUBCASE("signed") {
    auto func = get_one(corpus, "test_signed");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("signed*") {
    auto func = get_one(corpus, "test_ptr_signed");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed char") {
    auto func = get_one(corpus, "test_signed_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer8");
  }
  SUBCASE("signed char*") {
    auto func = get_one(corpus, "test_ptr_signed_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed char**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed short") {
    auto func = get_one(corpus, "test_signed_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer16");
  }
  SUBCASE("signed short*") {
    auto func = get_one(corpus, "test_ptr_signed_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed short**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed int") {
    auto func = get_one(corpus, "test_signed_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("signed int*") {
    auto func = get_one(corpus, "test_ptr_signed_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed int**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed long") {
    auto func = get_one(corpus, "test_signed_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("signed long*") {
    auto func = get_one(corpus, "test_ptr_signed_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed long**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed long long") {
    auto func = get_one(corpus, "test_signed_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("signed long long*") {
    auto func = get_one(corpus, "test_ptr_signed_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("signed long long**") {
    auto func = get_one(corpus, "test_ptr_ptr_signed_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Unsigned Integral Types") {
  SUBCASE("unsigned") {
    auto func = get_one(corpus, "test_unsigned");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("unsigned*") {
    auto func = get_one(corpus, "test_ptr_unsigned");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned char") {
    auto func = get_one(corpus, "test_unsigned_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer8");
  }
  SUBCASE("unsigned char*") {
    auto func = get_one(corpus, "test_ptr_unsigned_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned char**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned_char");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned short") {
    auto func = get_one(corpus, "test_unsigned_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer16");
  }
  SUBCASE("unsigned short*") {
    auto func = get_one(corpus, "test_ptr_unsigned_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned short**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned_short");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned int") {
    auto func = get_one(corpus, "test_unsigned_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("unsigned int*") {
    auto func = get_one(corpus, "test_ptr_unsigned_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned int**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned_int");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned long") {
    auto func = get_one(corpus, "test_unsigned_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("unsigned long*") {
    auto func = get_one(corpus, "test_ptr_unsigned_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned long**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned long long") {
    auto func = get_one(corpus, "test_unsigned_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("unsigned long long*") {
    auto func = get_one(corpus, "test_ptr_unsigned_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("unsigned long long**") {
    auto func = get_one(corpus, "test_ptr_ptr_unsigned_long_long");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Floating Point Types") {
  SUBCASE("float") {
    auto func = get_one(corpus, "test_float");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%xmm0");
    CHECK(parameters[0].type == "Float32");
  }
  SUBCASE("float*") {
    auto func = get_one(corpus, "test_ptr_float");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("float**") {
    auto func = get_one(corpus, "test_ptr_ptr_float");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("double") {
    auto func = get_one(corpus, "test_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%xmm0");
    CHECK(parameters[0].type == "Float64");
  }
  SUBCASE("double*") {
    auto func = get_one(corpus, "test_ptr_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("double**") {
    auto func = get_one(corpus, "test_ptr_ptr_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long double") {
    auto func = get_one(corpus, "test_long_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "framebase+8");
    CHECK(parameters[0].type == "Float128");
  }
  SUBCASE("long double*") {
    auto func = get_one(corpus, "test_ptr_long_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long double**") {
    auto func = get_one(corpus, "test_ptr_ptr_long_double");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("float _Complex") {
    auto func = get_one(corpus, "test_float__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "framebase+8");
    CHECK(parameters[0].type == "CplxFloat32");
  }
  SUBCASE("float _Complex*") {
    auto func = get_one(corpus, "test_ptr_float__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("float _Complex**") {
    auto func = get_one(corpus, "test_ptr_ptr_float__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("double _Complex") {
    auto func = get_one(corpus, "test_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "framebase+8");
    CHECK(parameters[0].type == "CplxFloat128");
  }
  SUBCASE("double _Complex*") {
    auto func = get_one(corpus, "test_ptr_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("double _Complex**") {
    auto func = get_one(corpus, "test_ptr_ptr_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long double _Complex") {
    auto func = get_one(corpus, "test_long_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "framebase+8");
    CHECK(parameters[0].type == "CplxFloat128");
  }
  SUBCASE("long double _Complex*") {
    auto func = get_one(corpus, "test_ptr_long_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("long double _Complex**") {
    auto func = get_one(corpus, "test_ptr_ptr_long_double__Complex");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - UTF Types") {
  SUBCASE("wchar_t") {
    auto func = get_one(corpus, "test_wchar_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("wchar_t*") {
    auto func = get_one(corpus, "test_ptr_wchar_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("wchar_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_wchar_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char16_t") {
    auto func = get_one(corpus, "test_char16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer16");
  }
  SUBCASE("char16_t*") {
    auto func = get_one(corpus, "test_ptr_char16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_char16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char32_t") {
    auto func = get_one(corpus, "test_char32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer32");
  }
  SUBCASE("char32_t*") {
    auto func = get_one(corpus, "test_ptr_char32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("char32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_char32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Size Types") {
  SUBCASE("size_t") {
    auto func = get_one(corpus, "test_size_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("size_t*") {
    auto func = get_one(corpus, "test_ptr_size_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("size_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_size_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("intmax_t") {
    auto func = get_one(corpus, "test_intmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("intmax_t*") {
    auto func = get_one(corpus, "test_ptr_intmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("intmax_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_intmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uintmax_t") {
    auto func = get_one(corpus, "test_uintmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("uintmax_t*") {
    auto func = get_one(corpus, "test_ptr_uintmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uintmax_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uintmax_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("intptr_t") {
    auto func = get_one(corpus, "test_intptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("intptr_t*") {
    auto func = get_one(corpus, "test_ptr_intptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("intptr_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_intptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uintptr_t") {
    auto func = get_one(corpus, "test_uintptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Integer64");
  }
  SUBCASE("uintptr_t*") {
    auto func = get_one(corpus, "test_ptr_uintptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uintptr_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uintptr_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Fixed-width Integral Types") {
  SUBCASE("int8_t") {
    auto func = get_one(corpus, "test_int8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int8_t*") {
    auto func = get_one(corpus, "test_ptr_int8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int16_t") {
    auto func = get_one(corpus, "test_int16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int16_t*") {
    auto func = get_one(corpus, "test_ptr_int16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int32_t") {
    auto func = get_one(corpus, "test_int32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int32_t*") {
    auto func = get_one(corpus, "test_ptr_int32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int64_t") {
    auto func = get_one(corpus, "test_int64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int64_t*") {
    auto func = get_one(corpus, "test_ptr_int64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast8_t") {
    auto func = get_one(corpus, "test_int_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_fast8_t*") {
    auto func = get_one(corpus, "test_ptr_int_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast16_t") {
    auto func = get_one(corpus, "test_int_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_fast16_t*") {
    auto func = get_one(corpus, "test_ptr_int_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast32_t") {
    auto func = get_one(corpus, "test_int_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_fast32_t*") {
    auto func = get_one(corpus, "test_ptr_int_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast64_t") {
    auto func = get_one(corpus, "test_int_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_fast64_t*") {
    auto func = get_one(corpus, "test_ptr_int_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_fast64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least8_t") {
    auto func = get_one(corpus, "test_int_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_least8_t*") {
    auto func = get_one(corpus, "test_ptr_int_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least16_t") {
    auto func = get_one(corpus, "test_int_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_least16_t*") {
    auto func = get_one(corpus, "test_ptr_int_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least32_t") {
    auto func = get_one(corpus, "test_int_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_least32_t*") {
    auto func = get_one(corpus, "test_ptr_int_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least64_t") {
    auto func = get_one(corpus, "test_int_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("int_least64_t*") {
    auto func = get_one(corpus, "test_ptr_int_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("int_least64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_int_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
TEST_CASE("Register Allocation - Unsigned Fixed-width Integral Types") {
  SUBCASE("uint8_t") {
    auto func = get_one(corpus, "test_uint8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint8_t*") {
    auto func = get_one(corpus, "test_ptr_uint8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint16_t") {
    auto func = get_one(corpus, "test_uint16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint16_t*") {
    auto func = get_one(corpus, "test_ptr_uint16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint32_t") {
    auto func = get_one(corpus, "test_uint32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint32_t*") {
    auto func = get_one(corpus, "test_ptr_uint32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint64_t") {
    auto func = get_one(corpus, "test_uint64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint64_t*") {
    auto func = get_one(corpus, "test_ptr_uint64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast8_t") {
    auto func = get_one(corpus, "test_uint_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_fast8_t*") {
    auto func = get_one(corpus, "test_ptr_uint_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_fast8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast16_t") {
    auto func = get_one(corpus, "test_uint_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_fast16_t*") {
    auto func = get_one(corpus, "test_ptr_uint_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_fast16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast32_t") {
    auto func = get_one(corpus, "test_uint_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_fast32_t*") {
    auto func = get_one(corpus, "test_ptr_uint_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_fast32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast64_t") {
    auto func = get_one(corpus, "test_uint_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_fast64_t*") {
    auto func = get_one(corpus, "test_ptr_uint_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_fast64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_fast64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least8_t") {
    auto func = get_one(corpus, "test_uint_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_least8_t*") {
    auto func = get_one(corpus, "test_ptr_uint_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least8_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_least8_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least16_t") {
    auto func = get_one(corpus, "test_uint_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_least16_t*") {
    auto func = get_one(corpus, "test_ptr_uint_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least16_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_least16_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least32_t") {
    auto func = get_one(corpus, "test_uint_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_least32_t*") {
    auto func = get_one(corpus, "test_ptr_uint_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least32_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_least32_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least64_t") {
    auto func = get_one(corpus, "test_uint_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
  }
  SUBCASE("uint_least64_t*") {
    auto func = get_one(corpus, "test_ptr_uint_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("uint_least64_t**") {
    auto func = get_one(corpus, "test_ptr_ptr_uint_least64_t");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}

TEST_CASE("Register Allocation - Null Types") {
  SUBCASE("void") {
    auto const& func = get_one(corpus, "test_void");
    auto const& parameters = func.parameters;
    CHECK(parameters.size() == 0UL);
  }
  SUBCASE("void") {
    auto const& func = get_one(corpus, "test_ptr_void");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
  SUBCASE("void") {
    auto const& func = get_one(corpus, "test_ptr_ptr_void");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location == "%rdi");
    CHECK(parameters[0].type == "Pointer64");
  }
}
