// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iostream>
#include <optional>
#include <regex>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Function.h"
#include "Symtab.h"
#include "Type.h"
#include "smeagle/parameter.h"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // Return true if the dataClass is a pointer type
  inline bool is_pointer(st::dataClass dc) { return dc == st::dataPointer; }

  // Return true if the dataClass is a reference
  inline bool is_ref(st::dataClass dc) { return dc == st::dataReference; }

  // Return true if dataClass is reference or pointer
  inline bool is_indirect(st::dataClass dc) {
    return dc == st::dataPointer || dc == st::dataReference;
  }

  // Return true if dataClass is a primitive type
  inline bool is_primitive(st::dataClass dc) { return dc == st::dataEnum || dc == st::dataScalar; }
  inline bool is_typedef(st::dataClass dc) { return dc == st::dataTypedef; }

  // Dereference a pointer or reference
  inline st::Type *deref(st::Type *t) {
    if (is_pointer(t->getDataClass())) {
      return deref(t->getPointerType()->getConstituentType());
    }
    if (is_ref(t->getDataClass())) {
      return deref(t->getRefType()->getConstituentType());
    }
    return t;
  }
  // Unwrap and remove typedef
  inline st::Type *remove_typedef(st::Type *t) {
    if (is_typedef(t->getDataClass())) {
      t = t->getTypedefType()->getConstituentType();
    }
    return t;
  };

}  // namespace smeagle::x86_64
