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
    std::string reg;
    std::string exportOrImport;  
    std::string locoffset;
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
     * @brief Dump a corpus to json
     */
    void toJson();

    /**
     * @brief Dump a corpus to yaml
     */
    void toYaml();

    /**
     * @brief Get the name of a symbol type from the enum int
     * @param symbol the symbol object
     */
    std::string getStringSymbolType(Symbol* symbol);

    /**
     * @brief get the location offset for a parameter / variable
     * @param param the parameter to get location offset for
     */
    std::string getParamLocationOffset(localVar * param);
    /**
     * @brief Get a string representation of a location from a type
     * @param paramType the parameter (variable) type
     * @param order the order of the parameter
     */
    std::string getStringLocationFromType(Type* paramType, int order);

    /**
     * @brief Update the framebase based on the parameter type
     * @param paramType the parameter (variable) type
     * @param framebase the current location on the stack
     */
    int updateFramebaseFromType(Type * paramType, int framebase);

  };

}  // namespace corpora
