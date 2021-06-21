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

  // A register class for AMD64 is defined on page 16 of the System V abi pdf
  enum RegisterClass {
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

  // A FramebaseAllocator keeps track of framebase index
  class FramebaseAllocator {
  private:
    int nextMultipleEight(int number);
    int framebase = 8;
    void updateFramebaseFromType(st::Type *paramType);

  public:
    std::string nextFramebaseFromType(st::Type *paramType);
  };

  // Get a framebase for a variable based on stack location and type
  // Framebase values must be 8 byte aligned.
  void FramebaseAllocator::updateFramebaseFromType(st::Type *paramType) {
    framebase += nextMultipleEight(paramType->getSize());
  }

  // Get the next greater multiple of 8
  int FramebaseAllocator::nextMultipleEight(int number) { return ((number + 7) & (-8)); }

  // Get the next framebase
  std::string FramebaseAllocator::nextFramebaseFromType(st::Type *paramType) {
    std::string framebaseStr = "framebase+" + std::to_string(framebase);

    // Update the framebase for the next parameter based on the type
    updateFramebaseFromType(paramType);

    return framebaseStr;
  }

  // A RegisterAllocator can provide the next register location
  class RegisterAllocator {
  public:
    RegisterAllocator();
    std::string getRegisterString(RegisterClass regClass, st::Type *paramType);
    std::string getRegistersString(std::pair<RegisterClass, RegisterClass> regClasses,
                                   st::Type *paramType);

  private:
    FramebaseAllocator fallocator;
    std::stack<std::string> intRegisters{{"%r9", "%r8", "%rcx", "%rdx", "%rsi", "%rdi"}};
    std::stack<std::string> sseRegisters;
    std::optional<std::string> getNextIntRegister();
    std::optional<std::string> getNextSseRegister();
    int framebase = 8;
  };

  // Constructor
  RegisterAllocator::RegisterAllocator() {
    // Populate the sse register stack
    for (int i = 1; i < 8; ++i) {
      sseRegisters.push("%xmm" + std::to_string(i));
    }
  }

  // Get the next available integer register
  std::optional<std::string> RegisterAllocator::getNextIntRegister() {
    // If we are empty, return stack
    if (intRegisters.empty()) return {};
    std::string regString = intRegisters.top();
    intRegisters.pop();
    return regString;
  }

  // Get the next available integer register
  std::optional<std::string> RegisterAllocator::getNextSseRegister() {
    // If we are empty, return stack
    if (sseRegisters.empty()) return {};
    std::string regString = sseRegisters.top();
    sseRegisters.pop();
    return regString;
  }

  // Given two registers, return one combined string
  std::string RegisterAllocator::getRegistersString(
      std::pair<RegisterClass, RegisterClass> regClasses, st::Type *paramType) {
    std::string locA = this->getRegisterString(regClasses.first, paramType);
    std::string locB = this->getRegisterString(regClasses.second, paramType);

    // If B is empty (NO_CLASS) then return A
    if (locB == "") {
      return locA;
    }
    return locA + "|" + locB;
  }

  // Get a string location from a register class
  std::string RegisterAllocator::getRegisterString(RegisterClass regClass, st::Type *paramType) {
    std::optional<std::string> regString;

    // If the class is memory, pass the argument on the stack
    if (regClass == RegisterClass::NO_CLASS) regString = "";
    if (regClass == RegisterClass::SSE) regString = this->getNextSseRegister();
    if (regClass == RegisterClass::INTEGER) regString = this->getNextIntRegister();
    if (regClass == RegisterClass::MEMORY) regString = std::nullopt;

    // If we don't have a value, we need a framebase
    if (!regString.has_value()) {
      regString = fallocator.nextFramebaseFromType(paramType);
    }

    // If we've run out of registers we get to this point
    return regString.value();
  }

  // Given a symbol, get a string representation of its type
  std::string getStringSymbolType(st::Symbol *symbol) {
    st::Symbol::SymbolType stype = symbol->getType();
    std::string sname;

    switch (stype) {
      case (st::Symbol::SymbolType::ST_FUNCTION): {
        sname = "function";
        break;
      }
      case (st::Symbol::SymbolType::ST_UNKNOWN): {
        sname = "unknown";
        break;
      }
      case (st::Symbol::SymbolType::ST_OBJECT): {
        sname = "object";
        break;
      }
      case (st::Symbol::SymbolType::ST_SECTION): {
        sname = "section";
        break;
      }
      case (st::Symbol::SymbolType::ST_MODULE): {
        sname = "module";
        break;
      }
      case (st::Symbol::SymbolType::ST_TLS): {
        sname = "tls";
        break;
      }
      case (st::Symbol::SymbolType::ST_DELETED): {
        sname = "deleted";
        break;
      }
      case (st::Symbol::SymbolType::ST_INDIRECT): {
        sname = "indirect";
        break;
      }
      case (st::Symbol::SymbolType::ST_NOTYPE): {
        sname = "notype";
        break;
      }
    }
    return sname;
  }

  // Return true if the dataClass is a pointer type
  static bool is_pointer(st::dataClass dc) { return dc == st::dataPointer; }

  // Return true if the dataClass is a reference
  static bool is_ref(st::dataClass dc) { return dc == st::dataReference; }

  // Return true if dataClass is reference or pointer
  static bool is_indirect(st::dataClass dc) {
    return dc == st::dataPointer || dc == st::dataReference;
  }

  // Return true if dataClass is a primitive type
  static bool is_primitive(st::dataClass dc) { return dc == st::dataEnum || dc == st::dataScalar; }

  // Dereference a pointer or reference
  static st::Type *deref(st::Type *t) {
    if (is_pointer(t->getDataClass())) {
      return deref(t->getPointerType()->getConstituentType());
    }
    if (is_ref(t->getDataClass())) {
      return deref(t->getRefType()->getConstituentType());
    }
    return t;
  }

  static bool is_typedef(st::dataClass dc) { return dc == st::dataTypedef; }

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

  // Unwrap and remove typedef
  static st::Type *remove_typedef(st::Type *t) {
    if (is_typedef(t->getDataClass())) {
      t = t->getTypedefType()->getConstituentType();
    }
    return t;
  };

  // Get directionality from argument type
  std::string getDirectionalityFromType(st::Type *paramType) {
    // Remove any top-level typedef
    paramType = remove_typedef(paramType);
    auto dataClass = paramType->getDataClass();

    // Any type passed by value is imported
    if (!is_indirect(dataClass)) {
      return "import";
    }

    // Remove any reference or pointer indirection
    paramType = deref(paramType);

    // Remove any remaining typedef
    paramType = remove_typedef(paramType);
    dataClass = paramType->getDataClass();

    // A pointer/reference to a primitive is imported
    if (is_primitive(dataClass)) {
      return "import";
    }

    // Passed by pointer or reference and not primitive, value is unknown
    return "unknown";
  }

  // Get register class given the argument type
  std::pair<RegisterClass, RegisterClass> getRegisterClassFromType(st::Type *paramType) {
    // Remove top-level typedef
    paramType = remove_typedef(paramType);

    // If it's a pointer, remove it
    if (is_indirect(paramType->getDataClass())) {
      paramType = deref(paramType);
      paramType = remove_typedef(paramType);
    }

    // Now get a string version of the type
    std::string paramTypeString = paramType->getName();

    // Signed and unsigned Bool,char,short,int,long,long long, and pointers
    std::regex checkinteger("(int|char|short|long|pointer|bool)");

    // Is it a constant?
    std::regex checkconstant("(const)");

    // float,double,_Decimal32,_Decimal64and__m64are in class SSE.
    std::regex checksse("(double|decimal|float|Decimal|m64)");

    // __float128 and__m128are split into two halves, least significant SSE,
    // most significant in SSEUP.
    std::regex checksseup("(m128|float128)");

    // The 64-bit mantissa of arguments of type long double belongs to classX87
    // the 16-bit exponent plus 6 bytes of padding belongs to class X87UP
    std::regex checklongdouble("(long|double)");

    // A variable of type complex long double is classified as type COMPLEX_X87
    std::regex checkcomplexlong("(complex long double)");

    // We will return a vector of classes
    std::pair<RegisterClass, RegisterClass> regClasses;

    // Does the type string match one of the types?
    bool isinteger = (std::regex_search(paramTypeString, checkinteger));
    bool isconst = (std::regex_search(paramTypeString, checkconstant));
    bool issse = (std::regex_search(paramTypeString, checksse));
    bool issseup = (std::regex_search(paramTypeString, checksseup));
    bool islongdouble = (std::regex_search(paramTypeString, checklongdouble));
    bool iscomplexlong = (std::regex_search(paramTypeString, checkcomplexlong));

    // A parameter can have more than one class!
    if (isconst) {
      regClasses = {RegisterClass::MEMORY, RegisterClass::NO_CLASS};
    }
    if (issseup) {
      regClasses = {RegisterClass::SSE, RegisterClass::SSEUP};
    }
    if (issse) {
      regClasses = {RegisterClass::SSE, RegisterClass::NO_CLASS};
    }
    if (isinteger) {
      regClasses = {RegisterClass::INTEGER, RegisterClass::NO_CLASS};
    }
    if (iscomplexlong) {
      regClasses = {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS};
    }
    if (islongdouble) {
      regClasses = {RegisterClass::X87, RegisterClass::X87UP};
    }

    // The classification of aggregate (structures and arrays) and union types worksas follows:
    // TODO need to look for struct / arrays?
    // page 18 of abi document
    return regClasses;
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
