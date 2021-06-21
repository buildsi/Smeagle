// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

namespace smeagle::x86_64 {

  // A register class for AMD64 is defined on page 16 of the System V abi pdf
  enum class RegisterClass {
    INTEGER,      // Integer types that fit into one of the general purpose registers
    SSE,          // Types that fit into an SSE register
    SSEUP,        // ^.. and can ve passed and returned in he most significant half of it
    X87,          // Types that will be returned via the x87 FPU
    X87UP,        // ^
    COMPLEX_X87,  // Types that will be returned via the x87 FPU
    NO_CLASS,     // Initalizer in the algorithms, used for padding and empty
                  // tructures and unions
    MEMORY        // Types that will be passed and returned in memory via the stack
  };

}  // namespace smeagle::x86_64
