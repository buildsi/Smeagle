#include <fmt/format.h>
#include <smeagle/smeagle.h>
#include "dynC.h"

using namespace smeagle;

Smeagle::Smeagle(std::string _name) : name(std::move(_name)) {}

std::string Smeagle::greet(LanguageCode lang) const {
  switch (lang) {
    default:
    case LanguageCode::EN:
      return fmt::format("Hello, {}!", name);
    case LanguageCode::DE:
      return fmt::format("Hallo {}!", name);
    case LanguageCode::ES:
      return fmt::format("¡Hola {}!", name);
    case LanguageCode::FR:
      return fmt::format("Bonjour {}!", name);
  }
}
