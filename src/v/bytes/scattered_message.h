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
#pragma once
#include "bytes/iobuf.h"

#include <seastar/core/temporary_buffer.hh>

#include <numeric>
#include <vector>

/// \brief converts an iobuf into a vector of shared temporary_buffer<char>,
/// each referencing a fragment of the original iobuf without copying.
/// The iobuf is kept alive via the shared deleter in each temporary_buffer.
std::vector<ss::temporary_buffer<char>> iobuf_to_buffer_vector(iobuf b);

/// \brief reassembles a vector of temporary buffers back into an iobuf
inline iobuf
buffer_vector_to_iobuf(std::vector<ss::temporary_buffer<char>> bufs) {
    iobuf result;
    for (auto& b : bufs) {
        result.append(std::move(b));
    }
    return result;
}

/// \brief returns the total size of a vector of temporary buffers
inline size_t
scattered_size(const std::vector<ss::temporary_buffer<char>>& bufs) {
    return std::accumulate(
      bufs.begin(), bufs.end(), size_t{0}, [](size_t acc, const auto& buf) {
          return acc + buf.size();
      });
}
