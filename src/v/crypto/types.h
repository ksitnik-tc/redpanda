/*
 * Copyright 2024 Redpanda Data, Inc.
 *
 * Use of this software is governed by the Business Source License
 * included in the file licenses/BSL.md
 *
 * As of the Change Date specified in that file, in accordance with
 * the Business Source License, use of this software will be governed
 * by the Apache License, Version 2.0
 */

#pragma once

#include "base/format_to.h"
#include "base/seastarx.h"

#include <seastar/util/bool_class.hh>

#include <string_view>

namespace crypto {
enum class digest_type { MD5, SHA256, SHA512 };

constexpr std::string_view to_string_view(digest_type t) {
    switch (t) {
    case digest_type::MD5:
        return "MD5";
    case digest_type::SHA256:
        return "SHA256";
    case digest_type::SHA512:
        return "SHA512";
    }
    return "unknown_digest_type";
}

enum class key_type { RSA };

constexpr std::string_view to_string_view(key_type t) {
    switch (t) {
    case key_type::RSA:
        return "RSA";
    }
    return "unknown_key_type";
}

enum class format_type { PEM, DER };

constexpr std::string_view to_string_view(format_type t) {
    switch (t) {
    case format_type::PEM:
        return "PEM";
    case format_type::DER:
        return "DER";
    }
    return "unknown_format_type";
}

using is_private_key_t = ss::bool_class<struct is_private_key_tag>;
} // namespace crypto
