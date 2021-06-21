// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "allocators.hpp"

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

}  // namespace smeagle::x86_64
