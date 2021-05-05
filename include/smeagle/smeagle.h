#pragma once

#include <string>

namespace smeagle {

  /**  Output Format codes to be used with the Smeagle class */
  enum class FormatCode { Terminal, Json };

  /**
   * @brief A class for saying hello in multiple languages
   */
  class Smeagle {
    std::string library;

  public:
    /**
     * @brief Creates a new smeagle to parse the precious
     * @param name the name to greet
     */
    Smeagle(std::string library);

    /**
     * @brief Parse the library with dyninst
     * @param fmt the format to print to the screen
     * @return a string containing the output
     */
    int parse(FormatCode fmt = FormatCode::Terminal);
  };

}  // namespace smeagle
