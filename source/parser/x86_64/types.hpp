// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

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
      size_t size_in_bytes_;

      std::string name() const { return name_; }
      std::string type_name() const { return type_name_; }
      std::string class_name() const { return class_name_; }
      std::string direction() const { return direction_; }
      std::string location() const { return location_; }
      size_t size_in_bytes() const { return size_in_bytes_; }
    };

    void toJson(param const &p, std::ostream &out, int indent) {
      auto buf = std::string(indent, ' ');
      out << buf << "\"name\":\"" << p.name() << "\",\n"
          << buf << "\"type\":\"" << p.type_name() << "\",\n"
          << buf << "\"class\":\"" << p.class_name() << "\",\n"
          << buf << "\"location\":\"" << p.location() << "\",\n"
          << buf << "\"direction\":\"" << p.direction() << "\",\n"
          << buf << "\"size\":\"" << p.size_in_bytes() << "\"";
    }
  }  // namespace detail

  // Parse a parameter into a Smeagle parameter
  void toJson(st::Type* param_type, std::string param_name, std::ostream &out, int indent);

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
  template <typename T> struct struct_t final : detail::param {
    T *dyninst_obj;
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      auto fields = *dyninst_obj->getFields();

      // Only print if we have fields
      if (fields.size() > 0) {
        auto buf = std::string(indent + 2, ' ');
        out << ",\n" << buf << "\"fields\": [\n";
        for (auto *field : fields) {
          // If we are at the last entry, no comma
          auto endcomma = (field == fields.back()) ? "" : ",";
          toJson(field->getType(), field->getName(), out, indent + 3);
          out << endcomma << "\n";
//          out << buf << "  {\"size\" : \"" << field->getSize() << "\",\n";
//          out << buf << "   \"name\" : \"" << field->getName() << "\",\n";
//          out << buf << "   \"type\" : \"" << field->getType()->getName() << "\"}" << endcomma
//              << "\n";
        }
        out << buf << "]\n";
      }
      out << buf << "}";
    }
  };

  // NOTE: we need to be able to parse call sites to do arrays
  template <typename T> struct array_t final : detail::param {
    T *dyninst_obj;
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << "\n" << buf << "}";
    }
  };

  template <typename T> struct enum_t final : detail::param {
    T *dyninst_obj;
    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << ",\n" << buf << "  \"constants\": {\n";

      // TODO: Dyninst does not provide information about underlying type
      // which we would need here
      auto constants = dyninst_obj->getConstants();
      for (auto const &c : constants) {
        auto endcomma = (c == constants.back()) ? "" : ",";
        out << buf << "    \"" << c.first << "\" : \"" << c.second << "\"" << endcomma << "\n";
      }
      out << buf << "}}";
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
    int pointer_indirections;
    T underlying_type;

    void toJson(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << ",\n" << buf << "  \"indirections\":\"" << pointer_indirections << "\"";
      out << ",\n" << buf << "  \"underlying_type\": ";
      underlying_type.toJson(out, indent + 4);
      out << "}";
    }
  };

  // Parse a parameter into a Smeagle parameter
  void toJson(st::Type* param_type, std::string param_name, std::ostream &out, int indent) {

    auto [underlying_type, ptr_cnt] = unwrap_underlying_type(param_type);
    std::string direction = "";

    // Scalar Type
    if (auto *t = underlying_type->getScalarType()) {
      auto param = smeagle::parameter{types::scalar_t{param_name, param_type->getName(), "Scalar",
                                                direction, "", param_type->getSize()}};

      // Structure Type
    } else if (auto *t = underlying_type->getStructType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = smeagle::parameter{types::struct_t<dyn_t>{param_name, param_type->getName(),
                                                       "Struct", direction, "",
                                                       param_type->getSize(), t}};

      // Union Type
    } else if (auto *t = underlying_type->getUnionType()) {
      auto param = smeagle::parameter{types::union_t{param_name, param_type->getName(), "Union",
                                               direction, "", param_type->getSize()}};

      // Array Type
    } else if (auto *t = underlying_type->getArrayType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = smeagle::parameter{types::array_t<dyn_t>{
          param_name, param_type->getName(), "Union", direction, "", param_type->getSize()}};

      // Enum Type
    } else if (auto *t = underlying_type->getEnumType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = smeagle::parameter{types::array_t<dyn_t>{param_name, param_type->getName(),
                                                      "Union", direction, "", param_type->getSize(),
                                                      t}};

      // Function Type
    } else if (auto *t = underlying_type->getFunctionType()) {
      auto param = smeagle::parameter{types::function_t{param_name, param_type->getName(), "Union",
                                                  direction, "", param_type->getSize()}};
    }
    throw std::runtime_error{"Unknown type" + param_type->getName()};
  }

}  // namespace smeagle::x86_64::types
