/*
 * Copyright 2025 Redpanda Data, Inc.
 *
 * Licensed as a Redpanda Enterprise file under the Redpanda Community
 * License (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * https://github.com/redpanda-data/redpanda/blob/master/licenses/rcl.md
 */
#pragma once

#include "absl/container/node_hash_map.h"
#include "cloud_topics/level_one/common/object.h"
#include "cloud_topics/level_one/common/object_id.h"
#include "container/intrusive_list_helpers.h"

#include <seastar/core/future.hh>

namespace cloud_topics {

/// LRU cache of parsed L1 object footers.
///
/// Footers are keyed on object id, so the footer cache can be shared between
/// l1::io instances.
class l1_footer_cache {
public:
    static constexpr size_t default_max_entries = 512;

    explicit l1_footer_cache(size_t max_entries = default_max_entries);
    ~l1_footer_cache();

    /// Look up a cached footer.
    std::optional<l1::footer> get(l1::object_id oid) const;

    /// Insert a footer, evicting another if full.
    void put(l1::object_id oid, const l1::footer& f);

    /// Number of cached entries.
    size_t size() const { return _map.size(); }

    /// No-op — required by the sharded service protocol.
    ss::future<> stop();

private:
    struct entry {
        entry(l1::object_id o, l1::footer f)
          : oid(o)
          , footer(std::move(f)) {}

        l1::object_id oid;
        l1::footer footer;
        safe_intrusive_list_hook _hook;
    };

    size_t _max_entries;
    mutable counted_intrusive_list<entry, &entry::_hook> _lru;
    absl::node_hash_map<l1::object_id, std::unique_ptr<entry>> _map;
};

} // namespace cloud_topics
