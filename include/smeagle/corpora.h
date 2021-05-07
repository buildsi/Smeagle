// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>

#include "Function.h"
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;

namespace smeagle {

  // A type location has a name, type, export/import, and location
  struct TypeLocation {
    std::string name;
    std::string parent;
    std::string type;
    std::string exportOrImport;  
    std::string location;
  };

  /**
   * @brief A class for holding corpus metadata
   */
  class Corpus {
    std::string library;
    std::vector <TypeLocation> typelocs;

  public:
    /**
     * @brief Creates a new corpus
     */
    Corpus(std::string library);

    /**
     * @brief Parse a function symbol into parameters, types, locations
     * @param symbol the symbol that is determined to be a function
     */
    void parseFunctionABILocation(Symbol *symbol);

    /**
     * @brief Dump a corpus to asp 
     */
    void toAsp();

    /**
     * @brief Get the name of a symbol type from the enum int
     * @param symbol the symbol object
     */
    std::string getStringSymbolType(Symbol* symbol);

    /**
     * @brief Get a string representation of a location from a type
     * @param paramType the parameter (variable) type
     */
    std::string getStringLocationFromType(Type* paramType, int order);

  };

}  // namespace corpora
