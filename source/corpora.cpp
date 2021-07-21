// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "smeagle/corpora.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Symtab.h"
#include "parser/aarch64/aarch64.hpp"
#include "parser/ppc64le/ppc64le.hpp"
#include "parser/x86_64/x86_64.hpp"

using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)){};

// dump all Type Locations to asp
void Corpus::toAsp() {
//  for (auto &f : functions) {
//    for (auto const &p : f.parameters) {
//      std::cout << "abi_typelocation("
//                << "\"" << library << "\", \"" << f.function_name << "\", \"" << p.name << "\", \""
//                << p.type << "\", \"" << p.location << "\", \"" << p.direction << "\")."
//                << std::endl;
//    }
//  }
}

// dump all Type Locations to yaml output
void Corpus::toYaml() {
  std::cout << "library: \"" << library << "\"\nlocations: " << std::endl;

  for (auto const &f : functions) {
    std::cout << "  - function:\n      name: \"" << f.function_name << "\"\n      parameters:";
    for (auto const &p : f.parameters) {
    	std::cout << "ABI interface:\n" << p.interface() << "\n";
    	std::cout << "Model facts:\n" << p.facts() << "\n";

//      std::cout << "\n        - name: \"" << p.name << "\"\n          type: \"" << p.type
//                << "\"\n          location: \"" << p.location << "\"\n          direction: \""
//                << p.direction << "\"";
    }
    std::cout << std::endl;
  }
}

// dump all Type Locations to json
void Corpus::toJson() {
//  std::cout << "{\n"
//            << " \"library\": \"" << library << "\",\n"
//            << " \"locations\":\n"
//            << " [\n";
//
//  for (auto &f : functions) {
//    std::string endcomma;
//    if (&f == &functions.back())
//      endcomma = "";
//    else {
//      endcomma = ",";
//    }
//    std::cout << "   {\n"
//              << "    \"function\": {\n"
//              << "      \"name\": \"" << f.function_name << ",\n"
//              << "      \"parameters\": [\n";
//
//    for (auto const &p : f.parameters) {
//      // Check if we are at the last entry (no comma) or not
//      std::string endcomma;
//      if (&p == &f.parameters.back())
//        endcomma = "";
//      else {
//        endcomma = ",";
//      }
//
//      std::cout << "     {"
//                << "\"name\":\"" << p.name << "\", "
//                << "\"type\":\"" << p.type << "\", "
//                << "\"location\":\"" << p.location << "\", "
//                << "\"direction\":\"" << p.direction << "\""
//                << "}" << endcomma << '\n';
//    }
//    std::cout << "    ]\n"
//              << "   }" << endcomma << "\n";
//  }
//  std::cout << " ]\n"
//            << "}" << std::endl;
}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Dyninst::SymtabAPI::Symbol *symbol,
                                      Dyninst::Architecture arch) {
  switch (arch) {
    case Dyninst::Architecture::Arch_x86_64:
      functions.emplace_back(x86_64::parse_parameters(symbol), x86_64::parse_return_value(symbol),
                             symbol->getMangledName());
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
