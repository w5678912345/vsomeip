#include <iomanip>

#include "../../../../../e2e_protection/include/e2e/profile/profile07/protector.hpp"

#include <vsomeip/internal/logger.hpp>
#include "../../utility/include/byteorder.hpp"

namespace vsomeip_v3 {
namespace e2e {
namespace profile07 {

/** @req [PRS_E2E_00486] */
void
protector::protect(e2e_buffer &_buffer, instance_t _instance) {
    std::lock_guard<std::mutex> lock(protect_mutex_);

    /** @req: [PRS_E2E_00487] */
    if (verify_inputs(_buffer)) {

        /** @req [PRS_E2E_00489] */
        /** write length */
        write_32(_buffer, static_cast<uint32_t>(_buffer.size()), config_.offset_ + 8);

        /** @req [PRS_E2E_00490] */
        /** write counter */
        write_32(_buffer, counter_, config_.offset_ + 12);

        /** @req [PRS_E2E_00491] */
        /** write data id */
        uint32_t its_data_id(uint32_t(_instance) << 24 | config_.data_id_);
        write_32(_buffer, its_data_id, config_.offset_ + 16);

        /** @req [PRS_E2E_00492] */
        /** compute_crc */
        uint64_t its_crc = profile_07::compute_crc(config_, _buffer);
        /** @req [PRS_E2E_00493] */
        /** write crc */
        write_64(_buffer, its_crc, config_.offset_);

        /** @req [PRS_E2E_00494] */
        increment_counter();
    }
}

bool
protector::verify_inputs(e2e_buffer &_buffer) {

    return (_buffer.size() >= config_.min_data_length_
            && _buffer.size() <= config_.max_data_length_);
}

void
protector::write_32(e2e_buffer &_buffer, uint32_t _data, size_t _index) {

    _buffer[config_.offset_ + _index] = VSOMEIP_LONG_BYTE3(_data);
    _buffer[config_.offset_ + _index + 1] = VSOMEIP_LONG_BYTE2(_data);
    _buffer[config_.offset_ + _index + 2] = VSOMEIP_LONG_BYTE1(_data);
    _buffer[config_.offset_ + _index + 3] = VSOMEIP_LONG_BYTE0(_data);
}

void
protector::write_64(e2e_buffer &_buffer, uint64_t _data, size_t _index) {

    _buffer[config_.offset_ + _index] = VSOMEIP_LONG_LONG_BYTE7(_data);
    _buffer[config_.offset_ + _index + 1] = VSOMEIP_LONG_LONG_BYTE6(_data);
    _buffer[config_.offset_ + _index + 2] = VSOMEIP_LONG_LONG_BYTE5(_data);
    _buffer[config_.offset_ + _index + 3] = VSOMEIP_LONG_LONG_BYTE4(_data);
    _buffer[config_.offset_ + _index + 4] = VSOMEIP_LONG_LONG_BYTE3(_data);
    _buffer[config_.offset_ + _index + 5] = VSOMEIP_LONG_LONG_BYTE2(_data);
    _buffer[config_.offset_ + _index + 6] = VSOMEIP_LONG_LONG_BYTE1(_data);
    _buffer[config_.offset_ + _index + 7] = VSOMEIP_LONG_LONG_BYTE0(_data);
}

void
protector::increment_counter() {
    counter_++;
}

} // namespace profile07
} // namespace e2e
} // namespace vsomeip_v3
