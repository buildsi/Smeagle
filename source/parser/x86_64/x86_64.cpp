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
#include "allocators.hpp"
#include "classifiers.hpp"
#include "smeagle/parameter.h"
#include "type_checker.hpp"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // Get a location offset for a variable
  // This function is not used because LocationLists are not reliable
  std::string getParamLocationOffset(st::localVar *param) {
    std::vector<Dyninst::VariableLocation> locs = param->getLocationLists();
    std::stringstream result;

    for (auto i = locs.begin(); i != locs.end(); ++i) {
      Dyninst::VariableLocation current = *i;

      // We only want to know where parameter is at the entrypoint
      result << std::hex << current.lowPC << " to " << current.hiPC << " " << current.mr_reg.name()
             << " " << std::dec << current.frameOffset;

      break;
    }

    return result.str();
  }

  // Get directionality from argument type
  std::string getDirectionalityFromType(st::Type *paramType) {
    // Remove any top-level typedef
    // NB: We can't call `unwrap_underlying_type` here as we need to keep
    //     any reference type for the call to `is_indirect` work.
    paramType = remove_typedef(paramType);
    auto dataClass = paramType->getDataClass();

    // Any type passed by value is imported
    if (!is_indirect(dataClass)) {
      return "import";
    }

    // Remove any remaining typedef or indirection
    paramType = unwrap_underlying_type(paramType).first;

    // A pointer/reference to a primitive is imported
    if (is_primitive(paramType->getDataClass())) {
      return "import";
    }

    // Passed by pointer or reference and not primitive, value is unknown
    return "unknown";
  }

  // Get register class given the argument type
  Class getRegisterClassFromType(st::Type *paramType) {
    auto [base_type, ptr_cnt] = unwrap_underlying_type(paramType);

    if (auto *t = base_type->getScalarType()) {
      return classify(t, ptr_cnt);
    }
    if (auto *t = base_type->getStructType()) {
      // page 18 of abi document
      return classify(t);
    }
    if (auto *t = base_type->getUnionType()) {
      return classify(t);
    }
    if (auto *t = base_type->getArrayType()) {
      return classify(t);
    }
    if (auto *t = base_type->getEnumType()) {
      return classify(t);
    }
    if (auto *t = base_type->getFunctionType()) {
      // This can only be a function pointer
      return classify(t);
    }

    throw std::runtime_error{"Unknown parameter type" + paramType->getName()};
  }

  std::vector<parameter> parse_parameters(st::Symbol *symbol) {
    // Get the name and type of the symbol
    std::string sname = symbol->getMangledName();
    st::Function *func = symbol->getFunction();
    std::vector<st::localVar *> params;

    // The function name looks equivalent to the symbol name
    std::string fname = func->getName();

    std::vector<parameter> typelocs;

    // Get parameters with types and names
    if (func->getParams(params)) {
      // Create a RegisterAllocator lookup class
      RegisterAllocator allocator;

      for (auto &param : params) {
        std::string paramName = param->getName();
        st::Type *paramType = param->getType();

        // Get register class based on type
        std::pair<RegisterClass, RegisterClass> regClasses = getRegisterClassFromType(paramType);

        // Get the directionality (export or import) given the type
        std::string direction = getDirectionalityFromType(paramType);

        // This uses location lists, not reliable
        // std::string locoffset = getParamLocationOffset(param);

        // Create a new typelocation to parse later
        parameter p;
        p.name = paramName;
        p.type = paramType->getName();
        p.location = allocator.getRegistersString(regClasses, paramType);
        p.direction = direction;
        typelocs.push_back(p);
      }
    }
    return typelocs;
  }

  parameter parse_return_value(Dyninst::SymtabAPI::Symbol const *) { return {}; }

}  // namespace smeagle::x86_64
