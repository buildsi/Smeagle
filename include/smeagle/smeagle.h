#pragma once

#include <string>

namespace smeagle {

  /**  Language codes to be used with the Smeagle class */
  enum class LanguageCode { EN, DE, ES, FR };

  /**
   * @brief A class for saying hello in multiple languages
   */
  class Smeagle {
    std::string name;

  public:
    /**
     * @brief Creates a new smeagle
     * @param name the name to greet
     */
    Smeagle(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @param lang the language to greet in
     * @return a string containing the greeting
     */
    std::string greet(LanguageCode lang = LanguageCode::EN) const;
  };

}  // namespace smeagle
