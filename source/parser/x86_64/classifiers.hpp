// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <utility>

#include "Type.h"
#include "register_class.hpp"

namespace smeagle::x86_64 {

  struct Class {
    RegisterClass lo, hi;
    int pointer_indirections;
    std::string name;
  };

  namespace st = Dyninst::SymtabAPI;

  inline Class classify(st::typeScalar *t, int ptr_cnt) {
    // size in BITS
    const auto size = t->getSize() * 8;

    if (ptr_cnt > 0) {
      /*
       * A pointer to a type X is converted to three abi_typelocation rules:
       * 	1) 	A Pointer64 type at the base location and the base direction.
       *
       * 	2) 	A recursive conversion of application type X, at location “(base location)”
       * 		(the base location wrapped in parentheses) and with direction ‘Export’.
       *
       * 	3)	A recursive conversion of application type X, at location “(base location)”
       * 	(the base location wrapped in parentheses) and with direction ‘Import’.
       *
       * 	For example, an application type int* at base location %rax and direction ‘Import’
       * would convert to: abi_typelocation(..., Import, Pointer64, “%rax”). abi_typelocation(...,
       * Export, Integer32, “(%rax)”). abi_typelocation(..., Import, Integer32, “(%rax)”).
       *
       */
      // TODO Should we integrate the directionality calculation here?
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, ptr_cnt, "Pointer64"};
    }

    // paramType properties have booleans to indicate types
    auto const &props = t->properties();

    // Integral types
    if (props.is_integral || props.is_UTF) {
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt,
                "IntegerVec" + std::to_string(size)};
      }
      if (size == 128) {
        // __int128 is treated as struct{long,long};
        // This is NOT correct, but we don't handle aggregates yet.
        // How do we differentiate between __int128 and __m128i?
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, ptr_cnt, "Integer128"};
      }

      // _Decimal32, _Decimal64, and __m64 are supposed to be SSE.
      // TODO How can we differentiate them here?
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, ptr_cnt,
              "Integer" + std::to_string(size)};
    }

    if (props.is_floating_point) {
      if (props.is_complex_float) {
        if (size == 128) {
          // x87 `complex long double`
          return {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS, ptr_cnt, "CplxFloat128"};
        }
        // This is NOT correct.
        // TODO It should be struct{T r,i;};, but we don't handle aggregates yet
        std::cout << "CplxFloat: " << t->getName() << " [" << t->getSize() << "]" << std::endl;
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, ptr_cnt,
                "CplxFloat" + std::to_string(size / 2)};
      }
      if (size <= 64) {
        // 32- or 64-bit floats
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt, "Float" + std::to_string(size)};
      }
      if (size == 128) {
        // x87 `long double` OR __m128[d]
        // TODO: How do we differntiate the vector type here? Dyninst should help us
        return {RegisterClass::X87, RegisterClass::X87UP, ptr_cnt, "Float128"};
      }
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt,
                "FloatVec" + std::to_string(size)};
      }
    }

    throw std::runtime_error{"Unknown scalar type"};
  }

  inline Class classify(st::typeStruct *) { return {}; }
  inline Class classify(st::typeUnion *) { return {}; }
  inline Class classify(st::typeArray *) { return {}; }
  inline Class classify(st::typeEnum *) { return {}; }
  inline Class classify(st::typeFunction *) { return {}; }

}  // namespace smeagle::x86_64
