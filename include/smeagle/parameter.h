// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>

namespace smeagle {

  /***
   * \brief Representation of a parameter in an interface
   *
   * A parameter can be a formal parameter of a function definition,
   * an actual parameter at a callsite, or a return value from a function.
   *
   */
  struct parameter {
    std::string name;
    std::string type;
    std::string direction;
    std::string pointer_indirections;
    std::string location;
  };

}  // namespace smeagle
