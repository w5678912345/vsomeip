#ifndef VSOMEIP_V3_E2E_PROFILE07_PROFILE07_HPP
#define VSOMEIP_V3_E2E_PROFILE07_PROFILE07_HPP

#include <cstdint>

#include <vsomeip/defines.hpp>

#include "../../../buffer/buffer.hpp"

namespace vsomeip_v3 {
namespace e2e {
namespace profile07 {

struct profile_config;

class profile_07 {
public:
    static uint64_t compute_crc(const profile_config &_config, const e2e_buffer &_buffer);
};

// [PRS_E2E_00480]
struct profile_config {
    profile_config() = delete;

    profile_config(uint32_t _data_id, uint32_t _offset,
            uint32_t _min_data_length, uint32_t _max_data_length,
            uint32_t _max_delta_counter)
        : data_id_(_data_id), offset_(_offset),
          min_data_length_(_min_data_length), max_data_length_(_max_data_length),
          max_delta_counter_(_max_delta_counter),
          base_(VSOMEIP_FULL_HEADER_SIZE) {
    }
    profile_config(const profile_config &_config) = default;
    profile_config &operator=(const profile_config &_config) = default;

    uint32_t data_id_;
    uint32_t offset_; 
    uint32_t min_data_length_;
    uint32_t max_data_length_;
    uint32_t max_delta_counter_;

    // SOME/IP base
    size_t base_;
};

} // namespace profile_07
} // namespace e2e
} // namespace vsomeip_v3

#endif // VSOMEIP_V3_E2E_PROFILE07_PROFILE07_HPP
