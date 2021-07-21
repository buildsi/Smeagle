// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <string>
#include <vector>

#include "Function.h"
#include "Symtab.h"
#include "Type.h"
#include "allocators.hpp"
#include "classifiers.hpp"
#include "smeagle/parameter.h"
#include "type_checker.hpp"

namespace smeagle::x86_64 {
  namespace detail {
    struct parameter {
      std::string name;
      std::string type_name;
      std::string location;
    };

    struct none_t final : public parameter {
      std::string interface() const {
        return "ABI [" + name + ", " + type_name + ", " + location + "]";
      }
      std::string facts() const {
        return "FACT [" + name + ", " + type_name + ", " + location + "]";
      }
    };
    struct scalar_t final : public parameter {
      std::string facts() const {
        return "FACT [" + name + ", " + type_name + ", " + location + "]";
      }
      std::string interface() const {
        return "ABI [" + name + ", " + type_name + ", " + location + "]";
      }
    };
    struct union_t final : public parameter {};
    struct array_t final : public parameter {};
    struct enum_t final : public parameter {};
    struct function_t final : public parameter {};

    template <typename T> struct pointer_t final : public parameter {
      T underlying_type;
      int num_indirections;

      std::string facts() const {
        return "FACT [" + name + ", " + type_name + ", " + location + "]";
      }
      std::string interface() const {
        return "ABI [" + name + ", " + type_name + ", " + location + "]";
      }
    };
  }  // namespace detail
  namespace st = Dyninst::SymtabAPI;

  std::vector<parameter> parse_parameters(st::Symbol *symbol) {
    st::Function *func = symbol->getFunction();
    std::vector<st::localVar *> params;

    std::vector<parameter> typelocs;

    // Get parameters with types and names
    if (func->getParams(params)) {
      // Create a RegisterAllocator lookup class
      RegisterAllocator allocator;

      for (auto &param : params) {
        std::string paramName = param->getName();
        st::Type *paramType = param->getType();

        auto [base_type, ptr_cnt] = unwrap_underlying_type(paramType);

        if (auto *t = base_type->getScalarType()) {
          auto base_class = classify(t, 0);
          auto base_type_name = t->getName();

          if (ptr_cnt > 0) {
            auto ptr_class = classify(t, ptr_cnt);
            auto ptr_loc = allocator.getRegisterString(ptr_class.lo, ptr_class.hi, t);
            auto ptr_type_name = paramType->getName();

            auto p = smeagle::parameter{detail::pointer_t<detail::scalar_t>{
                paramName, ptr_type_name, ptr_loc, {paramName, base_type_name}}};
            typelocs.push_back(std::move(p));
          } else {
            auto loc = allocator.getRegisterString(base_class.lo, base_class.hi, base_type);
            auto p = smeagle::parameter{detail::scalar_t{paramName, base_type_name, loc}};
            typelocs.push_back(std::move(p));
          }
        } else if (auto *t = base_type->getStructType()) {
          // page 18 of abi document
        } else if (auto *t = base_type->getUnionType()) {
        } else if (auto *t = base_type->getArrayType()) {
        } else if (auto *t = base_type->getEnumType()) {
        } else if (auto *t = base_type->getFunctionType()) {
          // This can only be a function pointer
        }
      }
    }
    return typelocs;
  }

  parameter parse_return_value(Dyninst::SymtabAPI::Symbol const *) {
    using namespace detail;
    return smeagle::parameter{none_t{"None", "None", "None"}};
  }

}  // namespace smeagle::x86_64
