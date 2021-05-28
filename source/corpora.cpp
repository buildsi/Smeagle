// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <string>
#include <stdexcept>
#include <iostream>

#include "Symtab.h"

#include "smeagle/corpora.h"
#include "parser/x86_64.hpp"
#include "parser/ppc64le.hpp"
#include "parser/aarch64.hpp"

using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)) {};

// dump all Type Locations to asp
void Corpus::toAsp() {

    std::cout << "corpus(" << library << ")," << std::endl;
    for (auto &typeloc : typelocs) {

        std::cout << "abi_typelocation(" << library << ", " << typeloc.parent
                  << ", " << typeloc.name << ", " << typeloc.type << ", \""
                  << typeloc.location << ", " << typeloc.reg << "\")" << std::endl;
    }
}

// dump all Type Locations to yaml output
void Corpus::toYaml() {

    std::cout << "library: \"" << library << "\"\nlocations: " << std::endl;
    for (auto &typeloc : typelocs) {

        std::cout << " - library: " << library << "\n   parent: " << typeloc.parent
                  << "\n   name: " << typeloc.name << "\n   type: " << typeloc.type
                  << "\n   location: " <<  typeloc.location << "\n   register: "
                  << typeloc.reg << "\n" << std::endl;
    }
}


// dump all Type Locations to json
void Corpus::toJson() {

    std::cout << "{ \"library\": \"" << library << "\", \"locations\": [" << std::endl;
    for (auto &typeloc : typelocs) {

        // Check if we are at the last entry (no comma) or not
        std::string endcomma;
        if (&typeloc == &typelocs.back())
            endcomma = "";
        else {
            endcomma = ",";
        }
        std::cout << "{\"library\": \"" << library << "\", \"parent\": \""
                  << typeloc.parent << "\", \"name\": \"" << typeloc.name << "\", \"type\": \""
                  << typeloc.type << "\", \"location\": \"" << typeloc.location << "\", "
                  << "\"register\": \"" << typeloc.reg << "\"}"
                  << endcomma << std::endl;
    }
    std::cout << "]}" << std::endl;

}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Dyninst::SymtabAPI::Symbol *symbol, Dyninst::Architecture arch) {
    switch (arch) {
      case Dyninst::Architecture::Arch_x86_64:
        typelocs = std::move(x86_64::parse_parameters(symbol));
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
