// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <vector>

#include "smeagle/parameter.h"

namespace smeagle {

  struct abi_function_description {
    abi_function_description(std::vector<parameter> &&p, parameter &&rv, std::string &&fn)
        : parameters{std::move(p)}, return_value{std::move(rv)}, function_name{std::move(fn)} {}
    std::vector<parameter> parameters;
    parameter return_value;
    std::string function_name;
  };

  struct abi_variable_description {
    std::string variable_type;
    std::string variable_name;
    int variable_size;
  };
}  // namespace smeagle
