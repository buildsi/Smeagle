// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include "smeagle/smeagle.h"


TEST_CASE("Parameter Directionality") {

  auto corpus = smeagle::Smeagle("libdirectionality.so").parse();

  auto const& funcs = corpus.getFunctions();

  REQUIRE(funcs.size() == 1);

  auto const& parameters = funcs[0].parameters;

  SUBCASE("A simple int") {
	  CHECK(parameters[0].direction == "import");
  }
}
