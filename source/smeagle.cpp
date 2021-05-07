// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <fmt/format.h>
#include <smeagle/smeagle.h>
#include <smeagle/corpora.h>
#include <regex>
#include <iostream>
#include <fmt/core.h>

#include "Function.h"
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace smeagle;

Smeagle::Smeagle(std::string _library) : library(std::move(_library)) {}


// Parse the library with smeagle
int Smeagle::parse(FormatCode fmt) {

  // We are going to read functions and symbols
  Symtab *obj = NULL;
  std::vector<Symbol *> symbols;
  std::vector<Function *> funcs;

  // Read the library into the Symtab object, cut out early if there's error
  if (not Symtab::openFile(obj, library)) {
    std::cout << "There was a problem reading " << library << "\n";
    return 1;
  }

  // Get all functions in the library
  if (not obj->getAllFunctions(funcs)) {
    std::cout << "There was a problem getting functions from " << library << "\n";
    return 1;
  }

  // Get all functions in the library
  // Note: looping through this doesn't seem to work
  if (not obj->getAllSymbols(symbols)) {
    std::cout << "There was a problem getting symbols from " << library << "\n";
    return 1;
  }

  // Create a corpus
  Corpus corpus(library);
  
  // Loop through the vector and look at symbols
  for (auto &symbol : symbols) {
    // We are interested in symbols in the dynamic symbol table
    if (symbol->isInDynSymtab()) {
      // If It's a function, parse the parameters
      if (symbol->isFunction()) {
        corpus.parseFunctionABILocation(symbol);

        // The symbol is something else (we likely want a subset of these?)
      }  // else {
         // std::cout << "symbol_notparsed(" <<  sname << ")" << "\n";
      //}
    }
  }

  // Generate output (for now, all are asp).
  switch (fmt) {
    default:
    case FormatCode::Json:
      corpus.toAsp();
      break;
    case FormatCode::Terminal:
      corpus.toAsp();
      break;
    case FormatCode::Asp:
      corpus.toAsp();
      break;
  }

  return 0;
}
