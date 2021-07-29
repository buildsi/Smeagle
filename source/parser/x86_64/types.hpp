// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iosfwd>
#include <string>

namespace smeagle::x86_64::types {
  namespace detail {
    /*
     *  This class has an intentially weird layout. We leave the members public to make it an
     * aggregate, but have accessors to satisfy the smeagle::parameter interface requirements.
     */
    struct param {
      std::string name_;
      std::string type_name_;
      std::string class_name_;
      std::string direction_;
      std::string location_;
      int pointer_indirections_;
      size_t size_in_bytes_;

      std::string name() const { return name_; }
      std::string type_name() const { return type_name_; }
      std::string class_name() const { return class_name_; }
      std::string direction() const { return direction_; }
      std::string location() const { return location_; }
      int pointer_indirections() const { return pointer_indirections_; }
      size_t size_in_bytes() const { return size_in_bytes_; }
    };
  }  // namespace detail

  struct none_t final : detail::param {
    void write(std::ostream &out) const { out << "none"; }
  };
  struct scalar_t final : detail::param {
    void write(std::ostream &out) const {
      out << "     {\n"
          << "       \"name\":\"" << name() << "\",\n"
          << "       \"type\":\"" << type_name() << "\",\n"
          << "       \"class\":\"" << class_name() << "\",\n"
          << "       \"location\":\"" << location() << "\",\n"
          << "       \"indirections\":\"" << pointer_indirections() << "\",\n"
          << "       \"direction\":\"" << direction() << "\",\n"
          << "       \"size\":\"" << size_in_bytes() << "\"\n"
          << "     }";
    }
  };
  struct union_t final : detail::param {
    void write(std::ostream &out) const { out << "union"; }
  };
  struct struct_t final : detail::param {
    void write(std::ostream &out) const { out << "struct"; }
  };
  struct array_t final : detail::param {
    void write(std::ostream &out) const { out << "array"; }
  };
  struct enum_t final : detail::param {
    void write(std::ostream &out) const { out << "enum"; }
  };
  struct function_t final : detail::param {
    void write(std::ostream &out) const { out << "function"; }
  };

  template <typename T> struct pointer_t final : detail::param {
    T underlying_type;
    void write(std::ostream &out) const {
      out << "     {\n"
          << "       \"name\":\"" << name() << "\",\n"
          << "       \"type\":\"" << type_name() << "\",\n"
          << "       \"class\":\"" << class_name() << "\",\n"
          << "       \"location\":\"" << location() << "\",\n"
          << "       \"indirections\":\"" << pointer_indirections() << "\",\n"
          << "       \"direction\":\"" << direction() << "\",\n"
          << "       \"size\":\"" << size_in_bytes() << "\"\n"
          << "       \"underlying_type\":\n";
      underlying_type.write(out);
      out << "     }";
    }
  };
}  // namespace smeagle::x86_64::types
