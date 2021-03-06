// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>

#include "Symtab.h"
#include "corpora.h"

using namespace Dyninst;
using namespace SymtabAPI;

namespace smeagle {

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
    smeagle::Corpus parse();

    // Determine if the library has exceptions with smeagle
    bool has_exceptions();
  };

}  // namespace smeagle
