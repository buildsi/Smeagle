// Functions to test register allocation

#include <iostream>

// Register Allocation String and char types
void test_stringchar_char(char x) {}
void test_stringchar_string(std::string x) {}
void test_stringchar_wchar_t(wchar_t x) {}

// Register Allocation Signed String and char types
void test_signed_char(signed char x) {}

// Register Allocation Unsigned String and char types
void test_unsigned_char(unsigned char x) {}

// Register Allocation Float types
void test_float_float(float x) {}
void test_float_double(double x) {}

// Register Allocation Unsigned Integral types
void test_integral_unsigned_int(unsigned int x) {}

// Register Allocation Signed Integral types
void test_signedintegral_signed_int(signed int x) {}

// Register Allocation Null types
void test_null_void() {}

// Register Allocation Boolean types
void test_boolean_boolean(bool x) {}

// Register Allocation Integral types
void test_single_int(int x) {}
void test_type_short(short x) {}
void test_type_single_long(long x) {}
void test_type_long_long(long long x) {}
void test_type_size_t(size_t x) {}

// Register Allocation Fixed Width Integer types
void test_type_int8_t(int8_t x) {}
void test_type_int16_t(int16_t x) {}
void test_type_int32_t(int32_t x) {}
void test_type_int64_t(int64_t x) {}
void test_type_int_fast8_t(int_fast8_t x) {}
void test_type_int_fast16_t(int_fast16_t x) {}
void test_type_int_fast32_t(int_fast32_t x) {}
void test_type_int_fast64_t(int_fast64_t x) {}
void test_type_int_least8_t(int_least8_t x) {}
void test_type_int_least16_t(int_least16_t x) {}
void test_type_int_least32_t(int_least32_t x) {}
void test_type_int_least64_t(int_least64_t x) {}

void test_type_intmax_t(intmax_t x) {}
void test_type_intptr_t(intptr_t x) {}
void test_type_uintmax_t(uintmax_t x) {}
void test_type_uintptr_t(uintptr_t x) {}

void test_type_uint8_t(uint8_t x) {}
void test_type_uint16_t(uint16_t x) {}
void test_type_uint32_t(uint32_t x) {}
void test_type_uint64_t(uint64_t x) {}
void test_type_uint_fast8_t(uint_fast8_t x) {}
void test_type_uint_fast16_t(uint_fast16_t x) {}
void test_type_uint_fast32_t(uint_fast32_t x) {}
void test_type_uint_fast64_t(uint_fast64_t x) {}
void test_type_uint_least8_t(uint_least8_t x) {}
void test_type_uint_least16_t(uint_least16_t x) {}
void test_type_uint_least32_t(uint_least32_t x) {}
void test_type_uint_least64_t(uint_least64_t x) {}
