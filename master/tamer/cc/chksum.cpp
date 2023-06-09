#include "../../digitama/big_bang/wormhole/checksum/crc32.hpp"
#include "../../digitama/big_bang/wormhole/checksum/ipv4.hpp"

#include "../../digitama/big_bang/datum/string.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static void checksum_ipv4_set(uint8_t* header, size_t size, size_t chksum_idx) {
	size_t post_chksum_idx = chksum_idx + 2;
	uint8_t* fields_before_checksum = header;
	uint8_t* fields_after_checksum = &header[post_chksum_idx];
	uint16_t sum = 0U;

	checksum_ipv4(&sum, fields_before_checksum, 0, chksum_idx);
	checksum_ipv4(&sum, fields_after_checksum, 0, size - post_chksum_idx);
		
	// set the checksum
	header[chksum_idx + 0] = sum >> 8U;
	header[chksum_idx + 1] = sum & 0xFFU;
}

/*************************************************************************************************/
extern "C" {
	__ffi__ uint32_t hex_chksum_crc32(const char* message, size_t size) {
		return checksum_crc32(reinterpret_cast<const uint8_t*>(message), 0, size);
	}
	
	__ffi__ const char* str_chksum_crc32(const char* message, size_t size) {
		static char sum[9];
		uint32_t crc = checksum_crc32(reinterpret_cast<const uint8_t*>(message), 0, size);
		std::string hex = hexnumber(crc, 4);

		return strncpy(sum, hex.c_str(), sizeof(sum) / sizeof(char));
	}

	__ffi__ uint32_t acc_chksum_crc32(const char* part1, size_t psz1, const char* part2, size_t psz2, const char* part3, size_t psz3) {
		uint32_t acc_crc = checksum_crc32(reinterpret_cast<const uint8_t*>(part1), 0, psz1);
		
		checksum_crc32(&acc_crc, reinterpret_cast<const uint8_t*>(part2), 0, psz2);
		
		return checksum_crc32(acc_crc, reinterpret_cast<const uint8_t*>(part3), 0, psz3);
	}
	
	__ffi__ uint16_t hex_chksum_ipv4(const char* message, size_t size) {
		return checksum_ipv4(reinterpret_cast<const uint8_t*>(message), 0, size);
	}

	__ffi__ uint16_t acc_chksum_ipv4(const char* message, size_t size, size_t bidx) {
		uint16_t acc_sum = 0U;

		if ((bidx & 0x01) == 1) { // odd
			checksum_ipv4(&acc_sum, reinterpret_cast<const uint8_t*>(message), bidx, size);
			acc_sum = ((acc_sum & 0xFF) << 8) ^ (acc_sum >> 8); // swap 2nd part checksum
			checksum_ipv4(&acc_sum, reinterpret_cast<const uint8_t*>(message), 0, bidx);
		} else {
			checksum_ipv4(&acc_sum, reinterpret_cast<const uint8_t*>(message), 0, bidx);
			checksum_ipv4(&acc_sum, reinterpret_cast<const uint8_t*>(message), bidx, size);
		}

		return acc_sum;
	}

	__ffi__ uint16_t chksum_ipv4_verify(char* header, size_t size) {
		static const size_t checksum_idx = 10;

		if (size > checksum_idx + 2) {
			checksum_ipv4_set(reinterpret_cast<uint8_t*>(header), size, checksum_idx);
		}

		return checksum_ipv4(reinterpret_cast<uint8_t*>(header), 0, size);
	}

	__ffi__ uint16_t chksum_ipv4_true_form(char* header, size_t size) {
		static const size_t checksum_idx = 10;
		uint8_t* uheader = reinterpret_cast<uint8_t*>(header);
		uint32_t Hsum = 0U;
		uint32_t Lsum = 0U;
		uint32_t sum = 0U;

		if (size > checksum_idx + 2) {
			checksum_ipv4_set(uheader, size, checksum_idx);
		}

		if ((size & 0x01) == 0x01) {
			Hsum += uheader[--size];
		}

		for (size_t idx = 0; idx < size; idx += 2) {
			Hsum += uheader[idx];
			Lsum += uheader[idx + 1];
		}

		sum = (Hsum << 8U) ^ Lsum;

		return (sum >> 16U) ^ (sum & 0xFFFFU);
	}
}
