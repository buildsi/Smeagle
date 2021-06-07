// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "smeagle/corpora.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Symtab.h"
#include "parser/aarch64.hpp"
#include "parser/ppc64le.hpp"
#include "parser/x86_64.hpp"

using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)){};

// dump all Type Locations to asp
void Corpus::toAsp() {
  std::cout << "corpus(" << library << ")," << std::endl;

  for (auto &p : params) {
    std::cout << "abi_typelocation(" << library << ", " << p.name << ", " << p.type << ", \""
              << p.location << "\", " << p.direction << ")" << std::endl;
  }
}

// dump all Type Locations to yaml output
void Corpus::toYaml() {
  std::cout << "library: \"" << library << "\"\nlocations: " << std::endl;

  for (auto &p : params) {
    std::cout << " - library: " << library << "\n   name: " << p.name << "\n   type: " << p.type
              << "\n   location: " << p.location << "\n   direction: " << p.direction << "\n"
              << std::endl;
  }
}

// dump all Type Locations to json
void Corpus::toJson() {
  std::cout << "{ \"library\": \"" << library << "\", \"locations\": [" << std::endl;
  for (auto &p : params) {
    // Check if we are at the last entry (no comma) or not
    std::string endcomma;
    if (&p == &params.back())
      endcomma = "";
    else {
      endcomma = ",";
    }

    std::cout << "{\"library\": \"" << library << "\", \"name\": \"" << p.name << "\", \"type\": \""
              << p.type << "\", \"location\": \"" << p.location << "\", \"direction\": \""
              << p.direction << "\"}" << endcomma << std::endl;
  }
  std::cout << "]}" << std::endl;
}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Dyninst::SymtabAPI::Symbol *symbol,
                                      Dyninst::Architecture arch) {
  switch (arch) {
    case Dyninst::Architecture::Arch_x86_64:
      params = std::move(x86_64::parse_parameters(symbol));
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
