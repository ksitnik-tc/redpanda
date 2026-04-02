/*
 * Copyright 2023 Redpanda Data, Inc.
 *
 * Licensed as a Redpanda Enterprise file under the Redpanda Community
 * License (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * https://github.com/redpanda-data/redpanda/blob/master/licenses/rcl.md
 */
#pragma once
#include "base/format_to.h"
#include "strings/string_switch.h"

#include <string_view>

namespace security::audit {

enum class event_type : std::uint8_t {
    management = 0,
    produce,
    consume,
    describe,
    heartbeat,
    authenticate,
    admin,
    schema_registry,
    unknown,
    num_elements
};

constexpr std::string_view to_string_view(event_type t) {
    switch (t) {
    case event_type::management:
        return "management";
    case event_type::produce:
        return "produce";
    case event_type::consume:
        return "consume";
    case event_type::describe:
        return "describe";
    case event_type::heartbeat:
        return "heartbeat";
    case event_type::authenticate:
        return "authenticate";
    case event_type::admin:
        return "admin";
    case event_type::schema_registry:
        return "schema_registry";
    case event_type::unknown:
        return "unknown";
    case event_type::num_elements:
        return "invalid";
    }
    return "invalid";
}

inline event_type string_to_event_type(const std::string_view s) {
    return string_switch<event_type>(s)
      .match("management", event_type::management)
      .match("produce", event_type::produce)
      .match("consume", event_type::consume)
      .match("describe", event_type::describe)
      .match("heartbeat", event_type::heartbeat)
      .match("authenticate", event_type::authenticate)
      .match("admin", event_type::admin)
      .match("schema_registry", event_type::schema_registry)
      .default_match(event_type::unknown);
}

} // namespace security::audit
