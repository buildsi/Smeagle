#include <cstdint>
#include <cstddef>
#include <complex.h>

// Integral
void test_bool(bool x){}
void test_char(char x){}
void test_int__(int x){}
void test_short(short x){}
void test_long(long x){}
void test_LONG_LONG(long long x){}
void test_size_t(size_t x){}

// Signed Integral
void test_signed__(signed x){}
void test_signed_char(signed char x){}
void test_signed_int(signed int x){}
void test_signed_short(signed short x){}
void test_signed_long(signed long x){}
void test_signed_LONG_LONG(signed long long x){}

// Unsigned Integral
void test_unsigned__(unsigned x){}
void test_unsigned_char(unsigned char x){}
void test_unsigned_int(unsigned int x){}
void test_unsigned_short(unsigned short x){}
void test_unsigned_long(unsigned long x){}
void test_unsigned_LONG_LONG(unsigned long long x){}

// Floating point
void test_float(float x) {}
void test_double(double x) {}
void test_LONG_DOUBLE(long double x){}
void test_complex_float(float _Complex x){}
void test_complex_double(double _Complex x){}
void test_complex_LONG_DOUBLE(long double _Complex x){}

// Register Allocation Null types
void test_null_void() {}

// Register Allocation Fixed Width Integer types
void test_int8_t(int8_t x) {}
void test_int16_t(int16_t x) {}
void test_int32_t(int32_t x) {}
void test_int64_t(int64_t x) {}
void test_int_fast8_t(int_fast8_t x) {}
void test_int_fast16_t(int_fast16_t x) {}
void test_int_fast32_t(int_fast32_t x) {}
void test_int_fast64_t(int_fast64_t x) {}
void test_int_least8_t(int_least8_t x) {}
void test_int_least16_t(int_least16_t x) {}
void test_int_least32_t(int_least32_t x) {}
void test_int_least64_t(int_least64_t x) {}

void test_intmax_t(intmax_t x) {}
void test_intptr_t(intptr_t x) {}
void test_uintmax_t(uintmax_t x) {}
void test_uintptr_t(uintptr_t x) {}

void test_uint8_t(uint8_t x) {}
void test_uint16_t(uint16_t x) {}
void test_uint32_t(uint32_t x) {}
void test_uint64_t(uint64_t x) {}
void test_uint_fast8_t(uint_fast8_t x) {}
void test_uint_fast16_t(uint_fast16_t x) {}
void test_uint_fast32_t(uint_fast32_t x) {}
void test_uint_fast64_t(uint_fast64_t x) {}
void test_uint_least8_t(uint_least8_t x) {}
void test_uint_least16_t(uint_least16_t x) {}
void test_uint_least32_t(uint_least32_t x) {}
void test_uint_least64_t(uint_least64_t x) {}

// UTF types
void test_WCHAR_T(wchar_t x){}
void test_CHAR16_T(char16_t x){}
void test_CHAR32_T(char32_t x){}

