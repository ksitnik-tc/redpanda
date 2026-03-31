/*
 * Copyright 2022 Redpanda Data, Inc.
 *
 * Use of this software is governed by the Business Source License
 * included in the file licenses/BSL.md
 *
 * As of the Change Date specified in that file, in accordance with
 * the Business Source License, use of this software will be governed
 * by the Apache License, Version 2.0
 */
#include "bytes/scattered_message.h"

std::vector<ss::temporary_buffer<char>> iobuf_to_buffer_vector(iobuf b) {
    std::vector<ss::temporary_buffer<char>> bufs;
    bufs.reserve(std::distance(b.begin(), b.end()));
    for (auto& frag : b) {
        bufs.push_back(frag.share());
    }
    return bufs;
}
