#include <doctest/doctest.h>
#include <smeagle/smeagle.h>
#include <smeagle/version.h>

#include <string>

TEST_CASE("Smeagle") {
  using namespace smeagle;

  Smeagle smeagle("Tests");

  CHECK(smeagle.parse(FormatCode::Terminal) == "Hello, 0!");
  CHECK(smeagle.parse(FormatCode::Json) == "Hallo 1!");
}

TEST_CASE("Smeagle version") {
  static_assert(std::string_view(SMEAGLE_VERSION) == std::string_view("1.0"));
  CHECK(std::string(SMEAGLE_VERSION) == std::string("1.0"));
}
