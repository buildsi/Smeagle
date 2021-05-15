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

  // A register class for AMD64 is defined on page 16 of the System V abi pdf
  enum RegisterClass {    
    INTEGER,     // Integer types that fit into one of the general purpose registers    
    SSE,         // Types that fit into an SSE register
    SSEUP,       // ^.. and can ve passed and returned in he most significant half of it
    X87,         // Types that will be returned via the x87 FPU
    X87UP,       // ^
    COMPLEX_X87, // Types that will be returned via the x87 FPU
    NO_CLASS,    // Initalizer in the algorithms, used for padding and empty
                 // tructures and unions
    MEMORY       // Types that will be passed and returned in memory via the stack
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
     * @param regClass the RegisterClass
     * @param order the order of the parameter
     */
    std::string getStringLocationFromRegisterClass(RegisterClass regClasses, int order);

    /**
     * @brief Get a RegisterClass based on a parameter type
     * @param paramType the parameter (variable) type
     */
    std::vector <RegisterClass> getRegisterClassFromType(Type *paramType);

    /**
     * @brief Update the framebase based on the parameter type
     * @param paramType the parameter (variable) type
     * @param framebase the current location on the stack
     */
    int updateFramebaseFromType(Type * paramType, int framebase);

  };

}  // namespace corpora
