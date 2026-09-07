#include "proto_codec.hpp"

#include <cstring>

namespace faith
{
	namespace config_center
	{
		bool proto_codec::encode(const CcMessage& message, std::vector<std::uint8_t>& out)
		{
			const std::size_t payload_size = message.ByteSizeLong();
			if (payload_size == 0 || payload_size > max_payload_size)
			{
				return false;
			}

			out.resize(sizeof(std::uint32_t) + payload_size);
			const std::uint32_t len = static_cast<std::uint32_t>(payload_size);
			std::memcpy(out.data(), &len, sizeof(len));
			if (!message.SerializeToArray(out.data() + sizeof(std::uint32_t), static_cast<int>(payload_size)))
			{
				out.clear();
				return false;
			}
			return true;
		}

		bool proto_codec::decode(const void* data, std::size_t data_len, CcMessage& message)
		{
			if (data == nullptr || data_len < sizeof(std::uint32_t))
			{
				return false;
			}

			std::uint32_t payload_len = 0;
			std::memcpy(&payload_len, data, sizeof(payload_len));
			if (payload_len == 0 ||
				payload_len > max_payload_size ||
				data_len < sizeof(std::uint32_t) + payload_len)
			{
				return false;
			}

			const auto* payload = static_cast<const std::uint8_t*>(data) + sizeof(std::uint32_t);
			return message.ParseFromArray(payload, static_cast<int>(payload_len));
		}
	}
}
