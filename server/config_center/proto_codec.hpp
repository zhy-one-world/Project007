#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config_center.pb.h"

namespace faith
{
	namespace config_center
	{
		// Application frame: uint32 little-endian payload_len + CcMessage bytes.
		class proto_codec
		{
		public:
			static constexpr std::uint32_t max_payload_size = 1024 * 1024;

			static bool encode(const CcMessage& message, std::vector<std::uint8_t>& out);
			static bool decode(const void* data, std::size_t data_len, CcMessage& message);
		};
	}
}
