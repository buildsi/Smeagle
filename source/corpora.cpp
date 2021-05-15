// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <fmt/format.h>
#include <smeagle/corpora.h>
#include <regex>
#include <iostream>
#include <fmt/core.h>

#include "Function.h"
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)) {};

// Given a symbol, get a string representation of its type
std::string Corpus::getStringSymbolType(Symbol *symbol) {
  Symbol::SymbolType stype = symbol->getType();
  std::string sname;

  switch (stype) {
    case (Symbol::SymbolType::ST_FUNCTION): {
      sname = "function";
      break;
    }
    case (Symbol::SymbolType::ST_UNKNOWN): {
      sname = "unknown";
      break;
    }
    case (Symbol::SymbolType::ST_OBJECT): {
      sname = "object";
      break;
    }
    case (Symbol::SymbolType::ST_SECTION): {
      sname = "section";
      break;
    }
    case (Symbol::SymbolType::ST_MODULE): {
      sname = "module";
      break;
    }
    case (Symbol::SymbolType::ST_TLS): {
      sname = "tls";
      break;
    }
    case (Symbol::SymbolType::ST_DELETED): {
      sname = "deleted";
      break;
    }
    case (Symbol::SymbolType::ST_INDIRECT): {
      sname = "indirect";
      break;
    }
    case (Symbol::SymbolType::ST_NOTYPE): {
      sname = "notype";
      break;
    }
  }
  return sname;
}

// Get a framebase for a variable based on stack location and type
int Corpus::updateFramebaseFromType(Type * paramType, int framebase){

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
std::string Corpus::getParamLocationOffset(localVar * param){
    std::vector<VariableLocation> locs = param->getLocationLists();
    std::stringstream result;

    for (auto i = locs.begin(); i != locs.end(); ++i) {
        VariableLocation current = *i;

        // We only want to know where parameter is at the entrypoint
        result << std::hex << current.lowPC << " to " << current.hiPC 
               << " " << current.mr_reg.name() <<  " "
               << std::dec << current.frameOffset;

        break;
    }
           
    return result.str();
}


// Get register class given the argument type
std::vector <RegisterClass> Corpus::getRegisterClassFromType(Type *paramType) {

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
    regClass.push_back(RegisterClass::MEMORY);
  }
  if (issseup) {
    regClasses.insert(regClass.end(), {RegisterClass::SSE, RegisterClass::SSE_UP});
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
    regClasses.insert(regClass.end(), {RegisterClass::X87, RegisterClass::X87_UP});
  } 
  
  // check if the length is 0, default to RegisterClass::NO_CLASS;
  if (regClass.size() == 0) {
      regClasses.push_back(RegisterClass::NO_CLASS);
  }

  // The classification of aggregate (structures and arrays) and union types worksas follows:
  // TODO need to look for struct / arrays?
  // page 18 of abi document
  return regClasses;
}

// Get a string location from a register class
std::string Corpus::getStringLocationFromRegisterClass(RegisterClass regClasses, int order) {

  // Return a string representation of the register
  std::string regString;

  // If the class is memory, pass the argument on the stack
  if (regClasses.size() == 1 && regClasses[0] == RegisterClass::Memory) {
    regString = "stack";

  // If the class is INTEGER, the next available register in sequence is used
  } else if (regClasses.size() == 1 && regClasses[0] == RegisterClass::SSE) {
    loc = "xmm" + std::to_string(order - 1);
  } else if (regClass == RegisterClass::INTEGER) {
    switch (order) {
      case 1: {
        loc = "%rdi";
        break;
      }
      case 2: {
        loc = "%rsi";
        break;
      }
      case 3: {
        loc = "%rdx";
        break;
      }
      case 4: {
        loc = "%rcx";
        break;
      }
      case 5: {
        loc = "%r8";
        break;
      }
      case 6: {
        loc = "%r9";
        break;
      }

      // the document doesn't say it goes up this high
      case 7: {
        loc = "%r10";
        break;
      }
      case 8: {
        loc = "%r11";
        break;
      }
      case 9: {
        loc = "%r12";
        break;
      }
      case 10: {
        loc = "%r13";
        break;
      }
      case 11: {
        loc = "%r14";
        break;
      }
      case 12: {
        loc = "%r15";
        break;
      }
      // Greater than 6 is stored in memory
      default: { loc = "memory"; }
    }
  }

  return loc;
}

// dump all Type Locations to asp
void Corpus::toAsp() {

    std::cout << "corpus(" << library << ")," << std::endl; 
    for (auto &typeloc : typelocs) {

        std::cout << "abi_typelocation(" << library << ", " << typeloc.parent
                  << ", " << typeloc.name << ", " << typeloc.type << ", \""
                  << typeloc.location << ", " << typeloc.reg << "\")" << std::endl;
    }
}

// dump all Type Locations to yaml output
void Corpus::toYaml() {

    std::cout << "library: \"" << library << "\"\nlocations: " << std::endl; 
    for (auto &typeloc : typelocs) {

        std::cout << " - library: " << library << "\n   parent: " << typeloc.parent
                  << "\n   name: " << typeloc.name << "\n   type: " << typeloc.type
                  << "\n   location: " <<  typeloc.location << "\n   register: " 
                  << typeloc.reg << "\n" << std::endl;
    }
}


// dump all Type Locations to json
void Corpus::toJson() {

    std::cout << "{ \"library\": \"" << library << "\", \"locations\": [" << std::endl; 
    for (auto &typeloc : typelocs) {

        // Check if we are at the last entry (no comma) or not
        std::string endcomma;
        if (&typeloc == &typelocs.back())
            endcomma = "";
        else {
            endcomma = ",";
        }
        std::cout << "{\"library\": \"" << library << "\", \"parent\": \"" 
                  << typeloc.parent << "\", \"name\": \"" << typeloc.name << "\", \"type\": \""
                  << typeloc.type << "\", \"location\": \"" << typeloc.location << "\", " 
                  << "\"register\": \"" << typeloc.reg << "\"}"
                  << endcomma << std::endl;
    }
    std::cout << "]}" << std::endl; 

}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Symbol *symbol) {
  // Get the name and type of the symbol
  std::string sname = symbol->getMangledName();
  std::string stype = Corpus::getStringSymbolType(symbol);

  Function *func = symbol->getFunction();
  std::vector<localVar *> params;

  // The function name looks equivalent to the symbol name
  std::string fname = func->getName();

  // Get parameters with types and names
  if (func->getParams(params)) {

    // We need to keep track of the order and framebase, which starts at 8
    std::string framebaseStr;
    int framebase = 8;
    int order = 1;

    for (auto &param : params) {
      std::string paramName = param->getName();
      Type *paramType = param->getType();

      // Get register class based on type
      std::vector <RegisterClass> regClasses = getRegisterClassFromType(paramType);

      // Get register name from register classes
      std::string loc = getStringLocationFromRegisterClass(regClasses);

      // This uses location lists, not reliable
      // std::string locoffset = getParamLocationOffset(param);

      // Create a new typelocation to parse later
      TypeLocation typeloc;
      typeloc.name = paramName;
      typeloc.parent = fname;
      typeloc.type = paramType->getName();
     
      // TODO how to determine if export/import?
      typeloc.exportOrImport = "export";
      typeloc.reg = loc;
      typeloc.location = "framebase+" + std::to_string(framebase);
      typelocs.push_back (typeloc);
      order += 1;

      // Update the framebase for the next parameter based on the type
      framebase = updateFramebaseFromType(paramType, framebase);      
    }
  }
}
