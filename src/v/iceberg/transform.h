/*
 * Copyright 2024 Redpanda Data, Inc.
 *
 * Licensed as a Redpanda Enterprise file under the Redpanda Community
 * License (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * https://github.com/redpanda-data/redpanda/blob/master/licenses/rcl.md
 */
#pragma once

#include "utils/fixed_string.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cstdint>
#include <ostream>
#include <variant>

namespace iceberg {

struct identity_transform {
    static constexpr fixed_string key{"identity"};
};
struct bucket_transform {
    static constexpr fixed_string key{"bucket"};
    uint32_t n;
};
struct truncate_transform {
    static constexpr fixed_string key{"truncate"};
    uint32_t length;
};
struct year_transform {
    static constexpr fixed_string key{"year"};
};
struct month_transform {
    static constexpr fixed_string key{"month"};
};
struct day_transform {
    static constexpr fixed_string key{"day"};
};
struct hour_transform {
    static constexpr fixed_string key{"hour"};
};
struct void_transform {
    static constexpr fixed_string key{"void"};
};

using transform = std::variant<
  identity_transform,
  bucket_transform,
  truncate_transform,
  year_transform,
  month_transform,
  day_transform,
  hour_transform,
  void_transform>;
bool operator==(const transform& lhs, const transform& rhs);

std::ostream& operator<<(std::ostream&, const transform&);

} // namespace iceberg

// Individual transform types format as their key name.
template<>
struct fmt::formatter<iceberg::identity_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::identity_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("identity", ctx);
    }
};
template<>
struct fmt::formatter<iceberg::bucket_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::bucket_transform& t, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "bucket[{}]", t.n);
    }
};
template<>
struct fmt::formatter<iceberg::truncate_transform>
  : fmt::formatter<std::string_view> {
    auto format(
      const iceberg::truncate_transform& t, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "truncate[{}]", t.length);
    }
};
template<>
struct fmt::formatter<iceberg::year_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::year_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("year", ctx);
    }
};
template<>
struct fmt::formatter<iceberg::month_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::month_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("month", ctx);
    }
};
template<>
struct fmt::formatter<iceberg::day_transform>
  : fmt::formatter<std::string_view> {
    auto format(const iceberg::day_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("day", ctx);
    }
};
template<>
struct fmt::formatter<iceberg::hour_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::hour_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("hour", ctx);
    }
};
template<>
struct fmt::formatter<iceberg::void_transform>
  : fmt::formatter<std::string_view> {
    auto
    format(const iceberg::void_transform&, fmt::format_context& ctx) const {
        return fmt::formatter<std::string_view>::format("void", ctx);
    }
};

template<>
struct fmt::formatter<iceberg::transform> {
    constexpr auto parse(fmt::format_parse_context& ctx) const {
        return ctx.begin();
    }
    auto format(const iceberg::transform& t, fmt::format_context& ctx) const {
        return std::visit(
          [&ctx](const auto& v) { return fmt::format_to(ctx.out(), "{}", v); },
          t);
    }
};
