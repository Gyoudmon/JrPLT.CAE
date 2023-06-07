#include "../../digitama/big_bang/wormhole/checksum/crc32.hpp"
#include "../../digitama/big_bang/datum/string.hpp"

using namespace WarGrey::STEM;

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
}

/*
	private class IPv4 : public TestClass<IPv4> {
	public:
		TEST_METHOD(Break) {
			uint8 vector[] = { 0x00, 0x01, 0xf2, 0x03, 0xf4, 0xf5, 0xf6, 0xf7 };
			size_t count = sizeof(vector) / sizeof(uint8);
			unsigned short checksum = checksum_ipv4(vector, 0, count);

			for (size_t bidx = 0; bidx < count; bidx++) {
				Platform::String^ message = make_wstring(L"break@%d", bidx);
				unsigned short acc_sum = 0;

				if ((bidx & 0x01) == 1) {
					checksum_ipv4(&acc_sum, vector, bidx, count);
					acc_sum = ((acc_sum & 0xFF) << 8) ^ (acc_sum >> 8); // swap 2nd part checksum
					checksum_ipv4(&acc_sum, vector, 0, bidx);
				} else {
					checksum_ipv4(&acc_sum, vector, 0, bidx);
					checksum_ipv4(&acc_sum, vector, bidx, count);
				}

				Assert::AreEqual(int(checksum), int(acc_sum), message->Data());
			}
		}

		TEST_METHOD(Head) {
			uint8 header[] = {
				0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11,
				0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01, 0xc0, 0xa8, 0x00, 0xc7,
				0x00 // termination
			};
			size_t checksum_idx = sizeof(header) / sizeof(uint8) / 2;
			unsigned short sum = 0U;

			checksum_ipv4(&sum, header, 0, checksum_idx);
			checksum_ipv4(&sum, header, checksum_idx + 2, checksum_idx * 2);
			
			header[checksum_idx + 0] = sum >> 8U;
			header[checksum_idx + 1] = sum & 0xFFU;

			Assert::AreEqual(0x0, int(checksum_ipv4(header)), L"verify the header's checksum");
		}
	};
*/
