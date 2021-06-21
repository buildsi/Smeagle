// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <stack>
#include <vector>

#include "Symtab.h"
#include "register_class.hpp"
#include "smeagle/parameter.h"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // A FramebaseAllocator keeps track of framebase index
  class FramebaseAllocator {
  private:
    int nextMultipleEight(int number);
    int framebase = 8;
    void updateFramebaseFromType(st::Type *paramType);

  public:
    std::string nextFramebaseFromType(st::Type *paramType);
  };

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

}  // namespace smeagle::x86_64
