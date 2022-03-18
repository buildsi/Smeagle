// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "smeagle/corpora.h"

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Function.h"
#include "Symtab.h"
#include "parser/aarch64/aarch64.hpp"
#include "parser/ppc64le/ppc64le.hpp"
#include "parser/x86_64/x86_64.hpp"

using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)){};

namespace st = Dyninst::SymtabAPI;

// Print abi type locations and a type
void Corpus::printABILocation(std::vector<abi_function_description> descriptions, std::string name,
                              bool moreAfter) {
  for (auto &f : descriptions) {
    std::string outerEndcomma;

    // It's the last and we don't have more after!
    if (&f == &descriptions.back() && !moreAfter)
      outerEndcomma = "";
    else {
      outerEndcomma = ",";
    }

    // We have parameters
    if (f.parameters.size() > 0) {
      std::cout << "   {\n"
                << "    \"" << name << "\": {\n"
                << "      \"name\": \"" << f.function_name << "\",\n"
                << "      \"parameters\": [\n";

      for (auto const &p : f.parameters) {
        // Check if we are at the last entry (no comma) or not
        auto endcomma = (&p == &f.parameters.back()) ? "" : ",";
        p.toJson(std::cout, 8);
        std::cout << endcomma << '\n';
      }
      std::cout << "    ]\n";

      // If we don't have parameters, don't add anything
    } else {
      std::cout << "   {\n"
                << "    \"" << name << "\": {\n"
                << "      \"name\": \"" << f.function_name << "\"";
    }
    std::cout << "   }}" << outerEndcomma << "\n";
  }
}

// dump all Type Locations to json
void Corpus::toJson() {
  // ensure that we can replace already written characters (buffered output)
  std::ios::sync_with_stdio(false);
  std::cout << "{\n"
            << " \"library\": \"" << library << "\",\n"
            << " \"locations\":\n"
            << " [\n";

  // Total size of functions, callsites
  int totalSize = functions.size() + callsites.size();

  // Parsing of variables first
  for (auto &v : variables) {
    std::string endcomma;

    // If we are at the last variable AND there are no functions
    if (&v == &variables.back() && totalSize == 0)
      endcomma = "";
    else {
      endcomma = ",";
    }

    // Add a new variable type here
    std::cout << "   {\"variable\": {\n"
              << "      \"name\": \"" << v.variable_name << "\",\n"
              << "      \"type\": \"" << v.variable_type << "\",\n"
              << "      \"size\": \"" << v.variable_size << "\"}}" << endcomma << "\n";
  }

  // We have functions AND callsites
  bool moreAfter = false;
  if (callsites.size() > 0) {
    moreAfter = true;
  }
  if (functions.size() > 0) {
    printABILocation(functions, "function", moreAfter);
  }

  if (callsites.size() > 0) {
    printABILocation(callsites, "callsite", false);
  }

  std::cout << "]\n"
            << "}" << std::endl;
}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Dyninst::SymtabAPI::Symbol *symbol,
                                      Dyninst::Architecture arch) {
  switch (arch) {
    case Dyninst::Architecture::Arch_x86_64: {
      functions.emplace_back(x86_64::parse_parameters(symbol), x86_64::parse_return_value(symbol),
                             symbol->getMangledName());
      // callsites (todo, return value)
      callsites.emplace_back(x86_64::parse_callsites(symbol), x86_64::parse_return_value(symbol),
                             symbol->getMangledName());
      break;
    }
    case Dyninst::Architecture::Arch_aarch64:
      break;
    case Dyninst::Architecture::Arch_ppc64:
      break;
    default:
      throw std::runtime_error{"Unsupported architecture: " + std::to_string(arch)};
      break;
  }
}

// parse a variable (global) for parameters and abi location
void Corpus::parseVariableABILocation(Dyninst::SymtabAPI::Symbol *symbol,
                                      Dyninst::Architecture arch) {
  switch (arch) {
    case Dyninst::Architecture::Arch_x86_64:
      variables.emplace_back(x86_64::parse_variable(symbol));
      break;
    case Dyninst::Architecture::Arch_aarch64:
      break;
    case Dyninst::Architecture::Arch_ppc64:
      break;
    default:
      throw std::runtime_error{"Unsupported architecture: " + std::to_string(arch)};
      break;
  }
}
