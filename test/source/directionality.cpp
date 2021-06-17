// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include "smeagle/smeagle.h"


TEST_CASE("Parameter Directionality") {

  auto corpus = smeagle::Smeagle("libdirectionality.so").parse();

  auto funcs = corpus.getFunctions();

  // We only want the "foo" function
  funcs.erase(
	 std::remove_if(
		 funcs.begin(), funcs.end(),
		 [](smeagle::abi_description const& d){
	  	  	  return d.function_name.find("foo") == std::string::npos;
  	  	 }
	 ),
	 funcs.end()
  );

  REQUIRE(funcs.size() == 1);

  auto const& parameters = funcs[0].parameters;

  SUBCASE("A simple int") {
	  CHECK(parameters[0].direction == "import");
  }
}
