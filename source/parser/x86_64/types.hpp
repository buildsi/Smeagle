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
      if (!p.name().empty()) out << buf << "\"name\":\"" << p.name() << "\",\n";
      if (!p.type_name().empty()) out << buf << "\"type\":\"" << p.type_name() << "\",\n";
      if (!p.class_name().empty()) out << buf << "\"class\":\"" << p.class_name() << "\",\n";
      if (!p.location().empty()) out << buf << "\"location\":\"" << p.location() << "\",\n";
      if (!p.direction().empty()) out << buf << "\"direction\":\"" << p.direction() << "\",\n";
      out << buf << "\"size\":\"" << p.size_in_bytes() << "\"";
    }
  }  // namespace detail

  // Parse a parameter into a Smeagle parameter
  // Note that this function cannot be named toJson as overload resolution won't work
  void makeJson(st::Type *param_type, std::string param_name, std::ostream &out, int indent);

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

  template <typename T> struct union_t final : detail::param {
    T *dyninst_obj;

    // Keep track of all of the typenames we've seen.
    inline static std::unordered_set<std::string> seen;

    struct recursive_t final {};

    void toJson(std::ostream &out, int indent, recursive_t) const { parse(out, indent); }
    void toJson(std::ostream &out, int indent) const {
      seen.clear();
      parse(out, indent);
    }

  private:
    void parse(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);

      {
        // Do not re-parse the fields of struct types we've seen before
        // This prevents endless recursion
        auto [underlying_type, ptr_cnt] = unwrap_underlying_type(dyninst_obj);
        auto found = seen.find(underlying_type->getName()) != seen.end();
        if (found) {
          // terminate the base entry for this struct's type
          out << "\n" << buf << "}";
          return;
        }
        seen.insert(underlying_type->getName());
      }

      auto const &fields = *dyninst_obj->getComponents();

      // Only print if we have fields
      if (fields.size() > 0) {
        auto buf = std::string(indent + 2, ' ');
        out << ",\n" << buf << "\"fields\": [\n";

        for (auto cur = fields.begin(); cur != fields.end(); ++cur) {
          if (cur != fields.begin()) {
            out << ",";
          }
          auto *field = *cur;
          makeJson(field->getType(), field->getName(), out, indent + 3);
        }
        out << "]\n";
      }
      out << buf << "}";
    }
  };
  template <typename T> struct struct_t final : detail::param {
    T *dyninst_obj;
    // Keep track of all of the typenames we've seen.
    inline static std::unordered_set<std::string> seen;

    struct recursive_t final {};

    void toJson(std::ostream &out, int indent, recursive_t) const { parse(out, indent); }

    void toJson(std::ostream &out, int indent) const {
      seen.clear();
      parse(out, indent);
    }

  private:
    void parse(std::ostream &out, int indent) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);

      {
        // Do not re-parse the fields of struct types we've seen before
        // This prevents endless recursion
        auto [underlying_type, ptr_cnt] = unwrap_underlying_type(dyninst_obj);
        auto found = seen.find(underlying_type->getName()) != seen.end();
        if (found) {
          // terminate the base entry for this struct's type
          out << "\n" << buf << "}";
          return;
        }
        seen.insert(underlying_type->getName());
      }

      auto const &fields = *dyninst_obj->getFields();

      // Only print if we have fields
      if (fields.size() > 0) {
        auto buf = std::string(indent + 2, ' ');
        out << ",\n" << buf << "\"fields\": [\n";

        for (auto cur = fields.begin(); cur != fields.end(); ++cur) {
          if (cur != fields.begin()) {
            out << ",";
          }
          auto *field = *cur;
          makeJson(field->getType(), field->getName(), out, indent + 3);
        }
        out << "]\n";
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

      // There seems to be a bug with Dyninst duplicating information?
      std::unordered_set<std::string> seen;

      // TODO: Dyninst does not provide information about underlying type
      // which we would need here
      auto constants = dyninst_obj->getConstants();
      for (auto const &c : constants) {
        auto endcomma = (c == constants.back()) ? "" : ",";
        auto found = seen.find(c.first) != seen.end();
        if (!found) {
          out << buf << "    \"" << c.first << "\" : \"" << c.second << "\"" << endcomma << "\n";
          seen.insert(c.first);
        }
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

    template<typename... Args>
    void toJson(std::ostream &out, int indent, Args &&... args) const {
      auto buf = std::string(indent, ' ');
      out << buf << "{\n";
      detail::toJson(*this, out, indent + 2);
      out << ",\n" << buf << "  \"indirections\":\"" << pointer_indirections << "\"";
      out << ",\n" << buf << "  \"underlying_type\": ";
      underlying_type.toJson(out, indent + 4, std::forward<Args>(args)...);
      out << "}";
    }
  };

  // Parse a parameter into a Smeagle parameter
  void makeJson(st::Type *param_type, std::string param_name, std::ostream &out, int indent) {
    auto [underlying_type, ptr_cnt] = unwrap_underlying_type(param_type);
    std::string direction = "";

    // Scalar Type
    if (auto *t = underlying_type->getScalarType()) {
      auto param = types::scalar_t{param_name, param_type->getName(), "Scalar", direction,
                                   "",         param_type->getSize()};

      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};
        ptr.toJson(out, indent);
      } else {
        param.toJson(out, indent);
      }

      // Structure Type
    } else if (auto *t = underlying_type->getStructType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = types::struct_t<dyn_t>{param_name, param_type->getName(), "Struct", direction,
                                          "",         param_type->getSize(), t};

      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};

        ptr.toJson(out, indent, types::struct_t<dyn_t>::recursive_t{});
      } else {
        param.toJson(out, indent, types::struct_t<dyn_t>::recursive_t{});
      }

      // Union Type
    } else if (auto *t = underlying_type->getUnionType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = types::union_t<dyn_t>{param_name, param_type->getName(), "Union", direction,
                                         "",         param_type->getSize(), t};
      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};

        ptr.toJson(out, indent, types::union_t<decltype(param)>::recursive_t{});
      } else {
        param.toJson(out, indent, types::union_t<decltype(param)>::recursive_t{});
      }

      // Array Type
    } else if (auto *t = underlying_type->getArrayType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = types::array_t<dyn_t>{param_name, param_type->getName(), "Array", direction,
                                         "",         param_type->getSize()};

      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};

        ptr.toJson(out, indent);
      } else {
        param.toJson(out, indent);
      }

      // Enum Type
    } else if (auto *t = underlying_type->getEnumType()) {
      using dyn_t = std::decay_t<decltype(*t)>;
      auto param = types::enum_t<dyn_t>{param_name, param_type->getName(), "Enum", direction,
                                        "",         param_type->getSize(), t};

      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};

        ptr.toJson(out, indent);
      } else {
        param.toJson(out, indent);
      }

      // Function Type
    } else if (auto *t = underlying_type->getFunctionType()) {
      auto param = types::function_t{param_name, param_type->getName(), "Function", direction,
                                     "",         param_type->getSize()};
      if (ptr_cnt > 0) {
        auto ptr = types::pointer_t<decltype(param)>{
            param_name, underlying_type->getName(), "Pointer", "",
            "",         param_type->getSize(),      ptr_cnt,   std::move(param)};

        ptr.toJson(out, indent);
      } else {
        param.toJson(out, indent);
      }

    } else {
      throw std::runtime_error{"Unknown type " + param_type->getName()};
    }
  }
}  // namespace smeagle::x86_64::types
