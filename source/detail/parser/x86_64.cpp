// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "smeagle/TypeLocation.h"

#include "Function.h"
#include "Symtab.h"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // A register class for AMD64 is defined on page 16 of the System V abi pdf
  enum RegisterClass {
    INTEGER,     // Integer types that fit into one of the general purpose registers
    SSE,         // Types that fit into an SSE register
    SSEUP,       // ^.. and can ve passed and returned in he most significant half of it
    X87,         // Types that will be returned via the x87 FPU
    X87UP,       // ^
    COMPLEX_X87, // Types that will be returned via the x87 FPU
    NO_CLASS,    // Initalizer in the algorithms, used for padding and empty
                 // tructures and unions
    MEMORY       // Types that will be passed and returned in memory via the stack
  };

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

  // Get a framebase for a variable based on stack location and type
  int updateFramebaseFromType(st::Type * paramType, int framebase){

      // sizeof 16 with alignment bytes 16
      std::regex check16("(__int128|long double|__float128|__m128)");

      // sizeof 8 with alignment bytes 8
      std::regex check8("(long|[*]|double|m_64|__int128)");

      // sizeof 1 with alginment bytes 1
      std::regex check1("(char|bool)");

      // sizeof 2 with alignment bytes 2
      std::regex check2("short");

      // sizeof 4 with alignment bytes 4
      std::regex check4("(int|enum|float)");

      std::string paramTypeString = paramType->getName();

      if (std::regex_search(paramTypeString, check16)){
          framebase += 16;
      } else if (std::regex_search(paramTypeString, check8)){
          framebase += 8;
      } else if (std::regex_search(paramTypeString, check1)){
          framebase += 1;
      } else if (std::regex_search(paramTypeString, check2)){
          framebase += 2;
      } else if (std::regex_search(paramTypeString, check4)){
          framebase += 4;
      }
      return framebase;
  }

  // Get a location offset for a variable
  // This function is not used because LocationLists are not reliable
  std::string getParamLocationOffset(st::localVar * param){
      std::vector<Dyninst::VariableLocation> locs = param->getLocationLists();
      std::stringstream result;

      for (auto i = locs.begin(); i != locs.end(); ++i) {
          Dyninst::VariableLocation current = *i;

          // We only want to know where parameter is at the entrypoint
          result << std::hex << current.lowPC << " to " << current.hiPC
                 << " " << current.mr_reg.name() <<  " "
                 << std::dec << current.frameOffset;

          break;
      }

      return result.str();
  }


  // Get register class given the argument type
  std::vector <RegisterClass> getRegisterClassFromType(st::Type *paramType) {

    // We need a string version of the type
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
    std::vector <RegisterClass> regClasses;

    // Does the type string match one of the types?
    bool isinteger = (std::regex_search(paramTypeString, checkinteger));
    bool isconst = (std::regex_search(paramTypeString, checkconstant));
    bool issse = (std::regex_search(paramTypeString, checksse));
    bool issseup = (std::regex_search(paramTypeString, checksseup));
    bool islongdouble = (std::regex_search(paramTypeString, checklongdouble));
    bool iscomplexlong = (std::regex_search(paramTypeString, checkcomplexlong));

    // A parameter can have more than one class!
    if (isconst) {
      regClasses.push_back(RegisterClass::MEMORY);
    }
    if (issseup) {
      regClasses.insert(regClasses.end(), {RegisterClass::SSE, RegisterClass::SSEUP});
    }
    if (issse) {
      regClasses.push_back(RegisterClass::SSE);
    }
    if (isinteger) {
      regClasses.push_back(RegisterClass::INTEGER);
    }
    if (iscomplexlong) {
      regClasses.push_back(RegisterClass::COMPLEX_X87);
    }
    if (islongdouble) {
      regClasses.insert(regClasses.end(), {RegisterClass::X87, RegisterClass::X87UP});
    }

    // check if the length is 0, default to RegisterClass::NO_CLASS;
    if (regClasses.size() == 0) {
        regClasses.push_back(RegisterClass::NO_CLASS);
    }

    // The classification of aggregate (structures and arrays) and union types worksas follows:
    // TODO need to look for struct / arrays?
    // page 18 of abi document
    return regClasses;
  }

  // Get a string location from a register class
  std::string getStringLocationFromRegisterClass(RegisterClass regClass, int order) {

    // Return a string representation of the register
    std::string regString;

    // If the class is memory, pass the argument on the stack
    if (regClass == RegisterClass::MEMORY) {
      regString = "stack";

    // If the class is INTEGER, the next available register in sequence is used
    } else if (regClass == RegisterClass::SSE) {
      regString = "xmm" + std::to_string(order - 1);
    } else if (regClass == RegisterClass::INTEGER) {
      switch (order) {
        case 1: {
          regString = "%rdi";
          break;
        }
        case 2: {
          regString = "%rsi";
          break;
        }
        case 3: {
          regString = "%rdx";
          break;
        }
        case 4: {
          regString = "%rcx";
          break;
        }
        case 5: {
          regString = "%r8";
          break;
        }
        case 6: {
          regString = "%r9";
          break;
        }

        // the document doesn't say it goes up this high
        case 7: {
          regString = "%r10";
          break;
        }
        case 8: {
          regString = "%r11";
          break;
        }
        case 9: {
          regString = "%r12";
          break;
        }
        case 10: {
          regString = "%r13";
          break;
        }
        case 11: {
          regString = "%r14";
          break;
        }
        case 12: {
          regString = "%r15";
          break;
        }
        // Greater than 6 is stored in memory
        default: { regString = "memory"; }
      }
    }

    return regString;
  }

  std::vector<TypeLocation> parse_parameters(st::Symbol *symbol) {
	  // Get the name and type of the symbol
	  std::string sname = symbol->getMangledName();
	  std::string stype = getStringSymbolType(symbol);

	  st::Function *func = symbol->getFunction();
	  std::vector<st::localVar *> params;

	  // The function name looks equivalent to the symbol name
	  std::string fname = func->getName();

	  std::vector<TypeLocation> typelocs;

	  // Get parameters with types and names
	  if (func->getParams(params)) {

	    // We need to keep track of the order and framebase, which starts at 8
	    std::string framebaseStr;
	    int framebase = 8;
	    int order = 1;

	    for (auto &param : params) {
	      std::string paramName = param->getName();
	      st::Type *paramType = param->getType();

	      // Get register class based on type
	      std::vector <RegisterClass> regClasses = getRegisterClassFromType(paramType);

	      // Get register name from register classes
	//      std::string loc = getStringLocationFromRegisterClass(regClasses);

	      // This uses location lists, not reliable
	      // std::string locoffset = getParamLocationOffset(param);

	      // Create a new typelocation to parse later
	      TypeLocation typeloc;
	      typeloc.name = paramName;
	      typeloc.parent = fname;
	      typeloc.type = paramType->getName();

	      // TODO how to determine if export/import?
	      typeloc.exportOrImport = "export";
	//      typeloc.reg = loc;
	      typeloc.location = "framebase+" + std::to_string(framebase);
	      typelocs.push_back(typeloc);
	      order += 1;

	      // Update the framebase for the next parameter based on the type
	      framebase = updateFramebaseFromType(paramType, framebase);
	    }
	  }
	  return typelocs;
  }

}  // namespace smeagle::x86_64
