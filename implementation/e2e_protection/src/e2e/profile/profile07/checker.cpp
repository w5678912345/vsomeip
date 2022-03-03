#include <iomanip>

#include <vsomeip/internal/logger.hpp>
#include "../../utility/include/byteorder.hpp"

#include "../../../../../e2e_protection/include/e2e/profile/profile07/checker.hpp"


namespace vsomeip_v3 {
namespace e2e {
namespace profile07 {

// [PRS_E2E_00495]
void profile_07_checker::check(const e2e_buffer &_buffer, instance_t _instance,
        e2e::profile_interface::check_status_t &_generic_check_status) {

    std::lock_guard<std::mutex> lock(check_mutex_);
    _generic_check_status = e2e::profile_interface::generic_check_status::E2E_ERROR;

    /** @req [PRS_E2E_00496] */
    if (verify_input(_buffer)) {
        /** @req [PRS_E2E_00497] */
        uint32_t its_received_length;
        if (read_32(_buffer, its_received_length, config_.offset_ + 8)) {
            /** @req [PRS_E2E_00498] */
            uint32_t its_received_counter;
            if (read_32(_buffer, its_received_counter, config_.offset_ + 12)) {
                /** @req [PRS_E2E_00499] */
                uint32_t its_received_data_id;
                if (read_32(_buffer, its_received_data_id, config_.offset_ + 16)) {
                    /** @req [PRS_E2E_00500] */
                    uint64_t its_received_crc;
                    if (read_64(_buffer, its_received_crc, config_.offset_)) {
                        uint64_t its_crc = profile_07::compute_crc(config_, _buffer);
                        /** @req [PRS_E2E_00501] */
                        if (its_received_crc != its_crc) {
                            _generic_check_status = e2e::profile_interface::generic_check_status::E2E_WRONG_CRC;
                            VSOMEIP_ERROR << std::hex << "E2E P07 protection: CRC64 does not match: calculated CRC: "
                                    << its_crc << " received CRC: " << its_received_crc;
                        } else {
                            uint32_t its_data_id(uint32_t(_instance) << 24 | config_.data_id_);
                            if (its_received_data_id == its_data_id
                                    && static_cast<uint32_t>(its_received_length) == _buffer.size()
                                    && verify_counter(its_received_counter)) {
                            _generic_check_status = e2e::profile_interface::generic_check_status::E2E_OK;
                            }
                            counter_ = its_received_counter;
                        }
                    }
                }
            }
        }
    }
}

bool
profile_07_checker::verify_input(const e2e_buffer &_buffer) const {

    auto its_length = _buffer.size();
    return (its_length >= config_.min_data_length_
            && its_length <= config_.max_data_length_);
}

bool
profile_07_checker::verify_counter(uint32_t _received_counter) const {

    static bool has_counter(false);
    uint32_t its_delta(0);

    if (has_counter) {
        if (counter_ < _received_counter)
            its_delta = uint32_t(_received_counter - counter_);
        else
            its_delta = uint32_t(uint32_t(0xFFFFFFFF) - counter_ + _received_counter);
    } else {
        has_counter = true;
    }

    return (its_delta <= config_.max_delta_counter_);
}

bool
profile_07_checker::read_32(const e2e_buffer &_buffer,
        uint32_t &_data, size_t _index) const {

    _data = VSOMEIP_BYTES_TO_LONG(_buffer[config_.offset_ + _index],
                                  _buffer[config_.offset_ + _index + 1],
                                  _buffer[config_.offset_ + _index + 2],
                                  _buffer[config_.offset_ + _index + 3]);
    return (true);
}


bool
profile_07_checker::read_64(const e2e_buffer &_buffer,
        uint64_t &_data, size_t _index) const {

    _data = VSOMEIP_BYTES_TO_LONG_LONG(_buffer[config_.offset_ + _index],
                                  _buffer[config_.offset_ + _index + 1],
                                  _buffer[config_.offset_ + _index + 2],
                                  _buffer[config_.offset_ + _index + 3],
                                  _buffer[config_.offset_ + _index + 4],
                                  _buffer[config_.offset_ + _index + 5],
                                  _buffer[config_.offset_ + _index + 6],
                                  _buffer[config_.offset_ + _index + 7]);
    return (true);
}

} // namespace profile07
} // namespace e2e
} // namespace vsomeip_v3
