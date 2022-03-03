#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../../../../../e2e_protection/include/e2e/profile/profile07/profile_07.hpp"
#include "../../../../../e2e_protection/include/crc/crc.hpp"

namespace vsomeip_v3 {
namespace e2e {
namespace profile07 {

uint64_t profile_07::compute_crc(const profile_config &_config, const e2e_buffer &_buffer) {
    uint64_t computed_crc = 0;
    if (_config.offset_ > 0) {
        buffer_view its_before(_buffer, _config.offset_);
        computed_crc = e2e_crc::calculate_profile_07(its_before);

        if (_config.offset_ + 8 < _buffer.size()) {
            buffer_view its_after(_buffer, _config.offset_ + 8, _buffer.size());
            computed_crc = e2e_crc::calculate_profile_07(its_after, computed_crc);
        }
    } else {
        if (_config.offset_ + 8 < _buffer.size()) {
            buffer_view its_data(_buffer, _config.offset_ + 8, _buffer.size());
            computed_crc = e2e_crc::calculate_profile_07(its_data);
        }
    }

    

    return (computed_crc);
}

} // namespace profile07
} // namespace e2e
} // namespace vsomeip_v3
