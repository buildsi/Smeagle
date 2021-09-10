// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <utility>

#include "Type.h"
#include "register_class.hpp"
#include "type_checker.hpp"

namespace smeagle::x86_64 {

  struct classification {
    RegisterClass lo, hi;
    std::string name;
    int pointer_indirections;
  };

  namespace st = Dyninst::SymtabAPI;

  inline classification classify(st::Field *f);
  inline classification classify(st::typeFunction *t);
  inline classification classify(st::typeEnum *t);
  inline classification classify(st::typeScalar *t);
  inline classification classify(st::typeStruct *t);
  inline classification classify(st::typeUnion *t);
  inline classification classify(st::typeArray *t);

  inline classification classify_pointer(int ptr_cnt) {
    return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Pointer", ptr_cnt};
  }

  // classify a base underlying type
  inline classification classify_type(st::Type *fieldType) {
    auto [underlying_type, ptr_cnt] = unwrap_underlying_type(fieldType);

    if (ptr_cnt > 0) {
      return classify_pointer(ptr_cnt);
    }

    if (auto *t = underlying_type->getScalarType()) {
      return classify(t);
    } else if (auto *t = underlying_type->getStructType()) {
      return classify(t);
    } else if (auto *t = underlying_type->getUnionType()) {
      return classify(t);
    } else if (auto *t = underlying_type->getArrayType()) {
      return classify(t);
    } else if (auto *t = underlying_type->getEnumType()) {
      return classify(t);
    } else if (auto *t = underlying_type->getFunctionType()) {
      return classify(t);
    }
    return {RegisterClass::NO_CLASS, RegisterClass::NO_CLASS, "Unknown"};
  }

  inline classification classify(st::typeScalar *t) {
    // paramType properties have booleans to indicate types
    auto const &props = t->properties();

    // size in BITS
    const auto size = t->getSize() * 8;

    // Integral types
    if (props.is_integral || props.is_UTF) {
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, "IntegerVec"};
      }
      if (size == 128) {
        // __int128 is treated as struct{long,long};
        // This is NOT correct, but we don't handle aggregates yet.
        // How do we differentiate between __int128 and __m128i?
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "Integer"};
      }

      // _Decimal32, _Decimal64, and __m64 are supposed to be SSE.
      // TODO How can we differentiate them here?
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Integer"};
    }

    if (props.is_floating_point) {
      if (props.is_complex_float) {
        if (size == 128) {
          // x87 `complex long double`
          return {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS, "CplxFloat"};
        }
        // This is NOT correct.
        // TODO It should be struct{T r,i;};, but we don't handle aggregates yet
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "CplxFloat"};
      }
      if (size <= 64) {
        // 32- or 64-bit floats
        return {RegisterClass::SSE, RegisterClass::SSEUP, "Float"};
      }
      if (size == 128) {
        // x87 `long double` OR __m128[d]
        // TODO: How do we differentiate the vector type here? Dyninst should help us
        return {RegisterClass::X87, RegisterClass::X87UP, "Float"};
      }
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, "FloatVec"};
      }
    }

    // TODO we will eventually want to throw this
    // throw std::runtime_error{"Unknown scalar type"};
    return {RegisterClass::NO_CLASS, RegisterClass::NO_CLASS, "Unknown"};
  }

  // Page 21 (bottom) AMD64 ABI - method to come up with final classification based on two
  RegisterClass merge(RegisterClass originalReg, RegisterClass newReg) {
    // a. If both classes are equal, this is the resulting class.
    if (originalReg == newReg) {
      return originalReg;
    }

    // b. If one of the classes is NO_CLASS, the resulting class is the other
    if (originalReg == RegisterClass::NO_CLASS) {
      return newReg;
    }
    if (newReg == RegisterClass::NO_CLASS) {
      return originalReg;
    }

    // (c) If one of the classes is MEMORY, the result is the MEMORY class.
    if (newReg == RegisterClass::MEMORY || originalReg == RegisterClass::MEMORY) {
      return RegisterClass::MEMORY;
    }

    // (d) If one of the classes is INTEGER, the result is the INTEGER.
    if (newReg == RegisterClass::INTEGER || originalReg == RegisterClass::INTEGER) {
      return RegisterClass::INTEGER;
    }

    // (e) If one of the classes is X87, X87UP, COMPLEX_X87 class, MEMORY is used as class.
    if (newReg == RegisterClass::X87 || newReg == RegisterClass::X87UP
        || newReg == RegisterClass::COMPLEX_X87) {
      return RegisterClass::MEMORY;
    }
    if (originalReg == RegisterClass::X87 || originalReg == RegisterClass::X87UP
        || originalReg == RegisterClass::COMPLEX_X87) {
      return RegisterClass::MEMORY;
    }

    // (f) Otherwise class SSE is used.
    return RegisterClass::SSE;
  }

  // Page 22 AMD64 ABI point 5 - this is the most merger "cleanup"
  void post_merge(RegisterClass &lo, RegisterClass &hi, size_t size) {
    // (a) If one of the classes is MEMORY, the whole argument is passed in memory.
    if (lo == RegisterClass::MEMORY || hi == RegisterClass::MEMORY) {
      lo = RegisterClass::MEMORY;
      hi = RegisterClass::MEMORY;
    }

    // (b) If X87UP is not preceded by X87, the whole argument is passed in memory.
    if (hi == RegisterClass::X87UP && lo != RegisterClass::X87) {
      lo = RegisterClass::MEMORY;
      hi = RegisterClass::MEMORY;
    }

    // (c) If the size of the aggregate exceeds two eightbytes and the first eight- byte isn’t SSE
    // or any other eightbyte isn’t SSEUP, the whole argument is passed in memory.
    if (size > 128 && (lo != RegisterClass::SSE || hi != RegisterClass::SSEUP)) {
      lo = RegisterClass::MEMORY;
      hi = RegisterClass::MEMORY;
    }
    // (d) If SSEUP is // not preceded by SSE or SSEUP, it is converted to SSE.
    if (hi == RegisterClass::SSEUP && (lo != RegisterClass::SSE && lo != RegisterClass::SSEUP)) {
      hi = RegisterClass::SSE;
    }
  }

  // Classify the whole struct
  inline classification classify(st::typeStruct *t) {
    const auto size = t->getSize();

    // If an object is larger than eight eightbyes (i.e., 64) class MEMORY.
    if (size > 64) {
      return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "Struct"};
    }

    RegisterClass hi = RegisterClass::NO_CLASS;
    RegisterClass lo = RegisterClass::NO_CLASS;
    for (auto *f : *t->getFields()) {
      auto c = classify(f);
      hi = merge(hi, c.hi);
      lo = merge(lo, c.lo);
    }

    // Pass a reference so they are updated here, and we also need size
    post_merge(lo, hi, size);
    return {lo, hi, "Struct"};
  }

  // Classify the fields
  std::vector<classification> classify_fields(st::typeStruct *t) {
    std::vector<classification> classes;
    for (auto *f : *t->getFields()) {
      classes.push_back(classify(f));
    }
    return classes;
  }

  inline classification classify(st::typeUnion *t) {
    const auto size = t->getSize();
    if (size > 64) {
      return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "Union"};
    }
    return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Union"};
  }

  inline classification classify(st::typeArray *t) {
    const auto size = t->getSize();

    if (size > 64) {
      return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "Array"};
    }

    // Just classify the base type
    return classify_type(t->getBaseType());
  }

  inline classification classify(st::typeEnum *t) {
    return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Enum"};
  }

  inline classification classify(st::typeFunction *t) { return {}; }

  // Classify a single field
  classification classify(st::Field *f) {
    // Just classify the type of the field
    return classify_type(f->getType());
  }

}  // namespace smeagle::x86_64
