// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <optional>
#include <stack>
#include <stdexcept>
#include <string>

#include "Type.h"
#include "register_class.hpp"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // A FramebaseAllocator keeps track of framebase index
  class FramebaseAllocator {
  private:
    int framebase = 8;

    // Get the next greater multiple of 8
    int nextMultipleEight(int number) { return ((number + 7) & (-8)); }

    // Get a framebase for a variable based on stack location and type
    // Framebase values must be 8 byte aligned.
    void updateFramebaseFromType(st::Type *paramType) {
      framebase += nextMultipleEight(paramType->getSize());
    }

  public:
    std::string nextFramebaseFromType(st::Type *paramType) {
      std::string framebaseStr = "framebase+" + std::to_string(framebase);

      // Update the framebase for the next parameter based on the type
      updateFramebaseFromType(paramType);

      return framebaseStr;
    }
  };

  // A RegisterAllocator can provide the next register location
  class RegisterAllocator {
  public:
    // Constructor
    RegisterAllocator() {
      // Populate the sse register stack
      for (int i = 7; i >= 0; --i) {
        sseRegisters.push("%xmm" + std::to_string(i));
      }
    }

    // Given two registers, return one combined string
    std::string getRegisterString(RegisterClass lo, RegisterClass hi, st::Type *paramType) {
      // Empty structs and unions don't have a location
      if ((lo == RegisterClass::NO_CLASS)
          && (paramType->getUnionType() || paramType->getStructType())) {
        return "none";
      }

      if (lo == RegisterClass::NO_CLASS) {
        throw std::runtime_error{"Can't allocate a {NO_CLASS, *}"};
      }

      if (lo == RegisterClass::MEMORY) {
        // goes on the stack
        return fallocator.nextFramebaseFromType(paramType);
      }

      if (lo == RegisterClass::INTEGER) {
        auto reg = getNextIntRegister();
        if (!reg) {
          // Ran out of registers, put it on the stack
          return fallocator.nextFramebaseFromType(paramType);
        }
        return reg.value();
      }

      if (lo == RegisterClass::SSE) {
        auto reg = getNextSseRegister();
        if (!reg) {
          // Ran out of registers, put it on the stack
          return fallocator.nextFramebaseFromType(paramType);
        }

        if (hi == RegisterClass::SSEUP) {
          // If the class is SSEUP, the eightbyte is passed in the next available eightbyte
          // chunk of the last used vector register.
        }
        return reg.value();

        /* TODO
         *
         *  For objects allocated in multiple registers, use the syntax '%r1 | %r2 | ...'
         *  to denote this. This can only happen for aggregates.
         *
         *  Use ymm and zmm for larger vector types and check for aliasing
         */
      }

      // If the class is X87, X87UP or COMPLEX_X87, it is passed in memory
      if (lo == RegisterClass::X87 || lo == RegisterClass::COMPLEX_X87
          || hi == RegisterClass::X87UP) {
        return fallocator.nextFramebaseFromType(paramType);
      }

      // This should never be reached
      throw std::runtime_error{"Unknown classification"};
    }

  private:
    FramebaseAllocator fallocator;
    std::stack<std::string> intRegisters{{"%r9", "%r8", "%rcx", "%rdx", "%rsi", "%rdi"}};
    std::stack<std::string> sseRegisters;
    int framebase = 8;

    // Get the next available integer register
    std::optional<std::string> getNextIntRegister() {
      // If we are empty, return stack
      if (intRegisters.empty()) return {};
      std::string regString = intRegisters.top();
      intRegisters.pop();
      return regString;
    }

    // Get the next available integer register
    std::optional<std::string> getNextSseRegister() {
      // If we are empty, return stack
      if (sseRegisters.empty()) return {};
      std::string regString = sseRegisters.top();
      sseRegisters.pop();
      return regString;
    }
  };

}  // namespace smeagle::x86_64
