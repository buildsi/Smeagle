// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <utility>

#include "Type.h"
#include "register_class.hpp"

namespace smeagle::x86_64 {

  struct classification {
    RegisterClass lo, hi;
    int pointer_indirections;
    std::string name;
  };

  namespace st = Dyninst::SymtabAPI;

  inline classification classify(st::typeScalar *t, int ptr_cnt) {
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
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, ptr_cnt, "Pointer"};
    }

    // paramType properties have booleans to indicate types
    auto const &props = t->properties();

    // Integral types
    if (props.is_integral || props.is_UTF) {
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt, "IntegerVec"};
      }
      if (size == 128) {
        // __int128 is treated as struct{long,long};
        // This is NOT correct, but we don't handle aggregates yet.
        // How do we differentiate between __int128 and __m128i?
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, ptr_cnt, "Integer"};
      }

      // _Decimal32, _Decimal64, and __m64 are supposed to be SSE.
      // TODO How can we differentiate them here?
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, ptr_cnt, "Integer"};
    }

    if (props.is_floating_point) {
      if (props.is_complex_float) {
        if (size == 128) {
          // x87 `complex long double`
          return {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS, ptr_cnt, "CplxFloat"};
        }
        // This is NOT correct.
        // TODO It should be struct{T r,i;};, but we don't handle aggregates yet
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, ptr_cnt, "CplxFloat"};
      }
      if (size <= 64) {
        // 32- or 64-bit floats
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt, "Float"};
      }
      if (size == 128) {
        // x87 `long double` OR __m128[d]
        // TODO: How do we differentiate the vector type here? Dyninst should help us
        return {RegisterClass::X87, RegisterClass::X87UP, ptr_cnt, "Float"};
      }
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, ptr_cnt, "FloatVec"};
      }
    }

    throw std::runtime_error{"Unknown scalar type"};
  }

  inline classification classify(st::typeStruct *) { return {}; }
  inline classification classify(st::typeUnion *) { return {}; }
  inline classification classify(st::typeArray *) { return {}; }
  inline classification classify(st::typeEnum *) { return {}; }
  inline classification classify(st::typeFunction *) { return {}; }

}  // namespace smeagle::x86_64
