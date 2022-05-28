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

  class ReturnValueAllocator {
  public:
    std::string getRegisterString(RegisterClass cls, RegisterClass, st::Type *paramType) {
      /*
       *   TODO:
       *   	The standard does not describe how to return aggregates and unions
       */

      if (RegisterClass::MEMORY == cls) {
        // If the type has class MEMORY, then the caller provides space for the return
        // value and passes the address of this storage in %rdi as if it were the first
        // argument to the function. In effect, this address becomes a “hidden” first ar-
        // gument. This storage must not overlap any data visible to the callee through
        // other names than this argument.
        // On return %rax will contain the address that has been passed in by the
        // caller in %rdi.
        return "%rax";
      }
      if (RegisterClass::INTEGER == cls) {
        // If the class is INTEGER, the next available register of the sequence %rax, %rdx is used.
        if (paramType->getSize() > 64) {
          return "%rax|%rdx";
        }
        return "%rax";
      }
      if (RegisterClass::SSE == cls) {
        // If the class is SSE, the next available vector register of the sequence %xmm0, %xmm1 is
        // used.
        // TODO: larger vector types (ymm, zmm)
        if (paramType->getSize() > 64) {
          return "%xmm0|%xmm1";
        }
        return "%xmm0";
      }
      if (RegisterClass::SSEUP == cls) {
        // If the class is SSEUP, the eightbyte is returned in the next available eightbyte
        // chunk of the last used vector register.
        return "SSEUP";
      }
      if (RegisterClass::X87 == cls || RegisterClass::X87UP == cls) {
        // If the class is X87, the value is returned on the X87 stack in %st0 as 80-bit x87 number.
        return "%st0";
      }
      if (RegisterClass::COMPLEX_X87 == cls) {
        // If the class is COMPLEX_X87, the real part of the value is returned in
        // %st0 and the imaginary part in %st1.
        return "%st0|%st1";
      }
      // This should never be reached
      throw std::runtime_error{"Unable to allocate return value"};
    }
  };
}  // namespace smeagle::x86_64
