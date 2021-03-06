// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <memory>
#include <string>

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
    template <typename T>
    explicit parameter(T x) : self(std::make_shared<model<T>>(std::move(x))) {}

    std::string name() const { return self->name(); }
    std::string type_name() const { return self->type_name(); }
    std::string class_name() const { return self->class_name(); }
    std::string direction() const { return self->direction(); }
    std::string location() const { return self->location(); }
    size_t size_in_bytes() const { return self->size_in_bytes(); }
    void toJson(std::ostream &out, int indent) const { self->toJson(out, indent); }

  private:
    struct concept_t {
      virtual ~concept_t() = default;
      virtual std::string name() const = 0;
      virtual std::string type_name() const = 0;
      virtual std::string class_name() const = 0;
      virtual std::string direction() const = 0;
      virtual std::string location() const = 0;
      virtual size_t size_in_bytes() const = 0;
      virtual void toJson(std::ostream &, int) const = 0;
    };
    template <typename T> struct model : concept_t {
      model(T x) : data{std::move(x)} {}
      std::string name() const override { return data.name(); }
      std::string type_name() const override { return data.type_name(); }
      std::string class_name() const override { return data.class_name(); }
      std::string direction() const override { return data.direction(); }
      std::string location() const override { return data.location(); }
      size_t size_in_bytes() const override { return data.size_in_bytes(); }
      void toJson(std::ostream &out, int indent) const { data.toJson(out, indent); }

      T data;
    };

    std::shared_ptr<concept_t const> self;
  };

}  // namespace smeagle
