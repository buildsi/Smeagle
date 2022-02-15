// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <smeagle/corpora.h>
#include <smeagle/smeagle.h>
#include <smeagle/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, "Extract library metadata, the precious.");

  std::string library;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("l,library", "Library to inspect", cxxopts::value(library))
    ("has-exceptions", "Show if a library has exceptions")
  ;

  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "Smeagle, version " << SMEAGLE_VERSION << std::endl;
    return 0;
  }

  // Library is required
  if (result["library"].count() == 0) {
    std::cout << "A library is required.\n";
    return 0;
  }

  smeagle::Smeagle smeagle(library);

  if (result["has-exceptions"].as<bool>()) {
    smeagle.has_exceptions();
    return 0;
  }
  smeagle::Corpus corpus = smeagle.parse();
  corpus.toJson();

  return 0;
}
