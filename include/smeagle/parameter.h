// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <memory>

namespace smeagle {

  /***
   * \brief Representation of a parameter in an interface
   *
   * A parameter can be a formal parameter of a function definition,
   * an actual parameter at a callsite, or a return value from a function.
   *
   */
  class parameter {
  public:
    template <typename T> explicit parameter(T x) : self_(std::make_shared<model<T>>(std::move(x))) {}

    std::string facts() const { return self_->facts(); }
    std::string interface() const { return self_->interface(); }

  private:
    struct concept_t {
      virtual ~concept_t() = default;
      virtual std::string facts() const = 0;
      virtual std::string interface() const = 0;
    };
    template <typename T> struct model : concept_t {
      model(T x) : data_{std::move(x)} {}
      std::string facts() const override { return data_.facts(); }
      std::string interface() const override { return data_.interface(); }

      T data_;
    };

    std::shared_ptr<concept_t const> self_;
  };

}  // namespace smeagle
