// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <vector>

#include "Symtab.h"

#include "smeagle/TypeLocation.h"

namespace smeagle::x86_64 {

  std::vector<TypeLocation> parse_parameters(Dyninst::SymtabAPI::Symbol *symbol);
}  // namespace smeagle::x86_64
