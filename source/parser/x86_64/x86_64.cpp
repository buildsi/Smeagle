// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <stdexcept>
#include <string>
#include <vector>

#include "Function.h"
#include "Symtab.h"
#include "Type.h"
#include "allocators.hpp"
#include "classifiers.hpp"
#include "smeagle/abi_description.h"
#include "smeagle/parameter.h"
#include "type_checker.hpp"
#include "types.hpp"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

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

  // Determine if a type is anonymous
  bool is_anonymous(st::Type *t) {
    return t->getName().find("anonymous struct") != std::string::npos
           || t->getName().find("anonymous class") != std::string::npos
           || t->getName().find("anonymous union") != std::string::npos;
  }

  template <typename class_t, typename base_t, typename param_t, typename... Args>
  smeagle::parameter classify(std::string const &param_name, base_t *base_type, param_t *param_type,
                              RegisterAllocator &allocator, int ptr_cnt, Args &&...args) {
    // If it's anonymous, we use the base type name
    auto base_type_name = is_anonymous(base_type) ? param_type->getName() : base_type->getName();
    auto direction = getDirectionalityFromType(param_type);
    auto base_class = classify(base_type);

    if (ptr_cnt > 0) {
      // On x86, all pointers are the same ABI class
      auto ptr_class = classify_pointer(ptr_cnt);

      // Allocate space for the pointer (NOT the underlying type)
      auto ptr_loc = allocator.getRegisterString(ptr_class.lo, ptr_class.hi, param_type);
      auto ptr_type_name = param_type->getName();

      return smeagle::parameter{
          types::pointer_t<class_t>{param_name,
                                    ptr_type_name,
                                    ptr_class.name,
                                    direction,
                                    ptr_loc,
                                    param_type->getSize(),
                                    0, ptr_cnt,
                                    {"", base_type_name, base_class.name, "", "",
                                    base_type->getSize(), 0, std::forward<Args>(args)...}}};
    }
    auto loc = allocator.getRegisterString(base_class.lo, base_class.hi, base_type);
    return smeagle::parameter{class_t{param_name, base_type_name, base_class.name, direction, loc,
             base_type->getSize(), 0, std::forward<Args>(args)...}};
  }

  smeagle::abi_variable_description parse_variable(st::Symbol *symbol) {
    smeagle::abi_variable_description description;
    auto variable = symbol->getVariable();
    st::Type *vartype = variable->getType();
    description.variable_name = symbol->getMangledName();
    description.variable_type = vartype ? vartype->getName() : "[UNKNOWN TYPE]";
    description.variable_size = variable->getSize();
    return description;
  }

  // Shared functions to parse a single parameter, add to typelocs
  // TODO allocator needs to be a reference and passed correctly
  // I think we might lose the state with this implementation
  parameter parse_parameter(st::Type *calledType, std::string calledTypeName,
                            RegisterAllocator &allocator) {
    auto [underlying_type, ptr_cnt] = unwrap_underlying_type(calledType);
    if (auto *t = underlying_type->getScalarType()) {
      return classify<types::scalar_t>(calledTypeName, t, calledType, allocator, ptr_cnt);
    } else if (auto *t = underlying_type->getStructType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      return classify<types::struct_t<dyn_t>>(calledTypeName, t, calledType, allocator, ptr_cnt, t);
    } else if (auto *t = underlying_type->getUnionType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      return classify<types::union_t<dyn_t>>(calledTypeName, t, calledType, allocator, ptr_cnt, t);
    } else if (auto *t = underlying_type->getArrayType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      return classify<types::array_t<dyn_t>>(calledTypeName, t, calledType, allocator, ptr_cnt, t);
    } else if (auto *t = underlying_type->getEnumType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      return classify<types::enum_t<dyn_t>>(calledTypeName, t, calledType, allocator, ptr_cnt, t);
    } else if (auto *t = underlying_type->getFunctionType()) {
      return classify<types::function_t>(calledTypeName, t, calledType, allocator, ptr_cnt);
    }
  }
  

  // Parse callsites and inline functions of a function
  // Probably should generalize the monster block of code below to do that :)
  std::vector<abi_function_description> parse_callsites(st::Symtab *symt) {
     std::vector<std::pair<st::Symbol *, st::FunctionDescriptor *> > exts;
     std::vector<abi_function_description> abi_descs;
     bool result = symt->getExternalFunctionRefs(exts);
     if (!result)
        return abi_descs;
     

     for (auto calls : exts) {
        st::Symbol *sym = calls.first;
        st::FunctionDescriptor *desc = calls.second;

        std::string calledFunc = sym->getMangledName();

        std::vector<parameter> parameters;

        RegisterAllocator allocator;
        for (unsigned int i = 0; i < desc->GetNumParams(); i++) {
           st::Type *paramtype = desc->GetParamNType(i);
           std::stringstream ss;
           ss << "param_i" << i;
           std::string paramname = ss.str();
           parameters.push_back(parse_parameter(paramtype, paramname, allocator));
        }

        st::Type *rettype = desc->GetReturnType();
        RegisterAllocator retallocator(RegisterAllocator::ReturnAllocatorType);                                  
        parameter retparam = parse_parameter(rettype, "[RETURN]", retallocator);

        abi_descs.emplace_back(std::move(parameters), std::move(retparam), std::move(calledFunc));
     }

     return abi_descs;
  }

  std::vector<parameter> parse_parameters(st::Symbol *symbol) {
    st::Function *func = symbol->getFunction();
    std::vector<st::localVar *> params;
    std::vector<parameter> typelocs;

    // Get parameters with types and names
    if (func->getParams(params)) {
      RegisterAllocator allocator;

      for (auto &param : params) {
        auto param_name = param->getName();
        st::Type *param_type = param->getType();
        typelocs.push_back(parse_parameter(param_type, param_name, allocator));
      }
    }
    return typelocs;
  }

  parameter parse_return_value(Dyninst::SymtabAPI::Symbol const *symbol) {
    st::Function *func = symbol->getFunction();     
    st::Type *ret_type = func->getReturnType();
    if (!ret_type)
       return smeagle::parameter{types::none_t{"None", "None", "None"}};
    RegisterAllocator allocator(RegisterAllocator::ReturnAllocatorType);    
    return parse_parameter(ret_type, "[RETURN]", allocator);
  }

}  // namespace smeagle::x86_64
