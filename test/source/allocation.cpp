// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>

#include "smeagle/smeagle.h"

auto get_one(smeagle::Corpus const& corpus, char const* name) {
  auto funcs = corpus.getFunctions();

  // We only want the "foo" function
  funcs.erase(std::remove_if(funcs.begin(), funcs.end(),
                             [name](smeagle::abi_description const& d) {
                               return d.function_name.find(name) == std::string::npos;
                             }),
              funcs.end());
  return funcs;
}

TEST_CASE("Register Allocation") {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();

  SUBCASE("one int") {
    auto funcs = get_one(corpus, "f1");
    REQUIRE(funcs.size() == 1);
    auto const& parameters = funcs[0].parameters;
    CHECK(parameters[0].location == "%rdi");
  }
}
