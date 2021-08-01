// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iosfwd>
#include <string>
#include <vector>
#include <utility>

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

    void toJson(param const &p, std::ostream &out, int indent) {
      auto buf = std::string(indent, ' ');
      out << buf << "\"name\":\"" << p.name() << "\",\n"
          << buf << "\"type\":\"" << p.type_name() << "\",\n"
          << buf << "\"class\":\"" << p.class_name() << "\",\n"
          << buf << "\"location\":\"" << p.location() << "\",\n"
          << buf << "\"pointer_indirections\":\"" << p.pointer_indirections() << "\",\n"
          << buf << "\"direction\":\"" << p.direction() << "\",\n"
          << buf << "\"size\":\"" << p.size_in_bytes() << "\"";
    }
  }  // namespace detail

  struct none_t final : detail::param {
    void toJson(std::ostream &out, int indent) const { out << "none"; }
  };
  struct scalar_t final : detail::param {
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };
  struct union_t final : detail::param {
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };
  struct struct_t final : detail::param {
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };
  struct array_t final : detail::param {
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };

  template <typename T>
  struct enum_t final : detail::param {
	T *dyninst_obj;
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << buf << "\n\"constants\": {\n";
      for(auto const& c : dyninst_obj->getConstants()) {
    	  out << buf << "\"" << c.first << "\" : \"" << c.second << "\",\n";
      }
      out << buf << "}\n";
      out << "\n" << buf << "}";
    }
  };

  struct function_t final : detail::param {
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };
  template <typename T> struct pointer_t final : detail::param {
    T underlying_type;
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << ",\n" << buf << "  \"underlying_type\":\n";
      underlying_type.toJson(out, indent + 4);
      out << "\n" << buf << "}";
    }
  };
}  // namespace smeagle::x86_64::types
