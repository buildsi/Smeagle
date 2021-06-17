// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <vector>

#include "Symtab.h"
#include "smeagle/abi_description.h"

namespace smeagle {

  /**
   * @brief A class for holding corpus metadata
   */
  class Corpus {
    std::string library;
    std::vector<abi_description> functions;

  public:
    /**
     * @brief Creates a new corpus
     */
    Corpus(std::string library);

    /**
     * @brief Parse a function symbol into parameters, types, locations
     * @param symbol the symbol that is determined to be a function
     */
    void parseFunctionABILocation(Dyninst::SymtabAPI::Symbol *, Dyninst::Architecture);

    /**
     * @brief Dump a corpus to asp
     */
    void toAsp();

    /**
     * @brief Dump a corpus to json
     */
    void toJson();

    /**
     * @brief Dump a corpus to yaml
     */
    void toYaml();

    std::vector<abi_description> const& getFunctions() const {
    	return functions;
    }
  };

}  // namespace smeagle
