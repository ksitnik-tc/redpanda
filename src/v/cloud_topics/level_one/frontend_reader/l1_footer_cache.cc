/*
 * Copyright 2025 Redpanda Data, Inc.
 *
 * Licensed as a Redpanda Enterprise file under the Redpanda Community
 * License (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * https://github.com/redpanda-data/redpanda/blob/master/licenses/rcl.md
 */
#include "cloud_topics/level_one/frontend_reader/l1_footer_cache.h"

namespace cloud_topics {

l1_footer_cache::l1_footer_cache(size_t max_entries)
  : _max_entries(max_entries) {}

l1_footer_cache::~l1_footer_cache() { _lru.clear(); }

std::optional<l1::footer> l1_footer_cache::get(l1::object_id oid) const {
    auto it = _map.find(oid);
    if (it == _map.end()) {
        return std::nullopt;
    }
    // Move to LRU tail (most recently used).
    auto& e = *it->second;
    _lru.erase(_lru.iterator_to(e));
    _lru.push_back(e);
    return e.footer.copy();
}

void l1_footer_cache::put(l1::object_id oid, const l1::footer& f) {
    auto it = _map.find(oid);
    if (it != _map.end()) {
        // Already cached — just touch it.
        auto& e = *it->second;
        _lru.erase(_lru.iterator_to(e));
        _lru.push_back(e);
        return;
    }

    // Evict LRU entry if at capacity.
    if (_map.size() >= _max_entries) {
        auto& victim = _lru.front();
        auto victim_oid = victim.oid;
        _lru.pop_front();
        _map.erase(victim_oid);
    }

    auto e = std::make_unique<entry>(oid, f.copy());
    _lru.push_back(*e);
    _map.emplace(oid, std::move(e));
}

ss::future<> l1_footer_cache::stop() { return ss::make_ready_future<>(); }

} // namespace cloud_topics
