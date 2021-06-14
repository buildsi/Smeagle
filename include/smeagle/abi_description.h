// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <vector>

#include "smeagle/parameter.h"

namespace smeagle {

  struct abi_description {
    abi_description(std::vector<parameter> &&p, parameter &&rv, std::string &&fn)
        : parameters{std::move(p)}, return_value{std::move(rv)}, function_name{std::move(fn)} {}
    const std::vector<parameter> parameters;
    parameter return_value;
    const std::string function_name;
  };

}  // namespace smeagle
