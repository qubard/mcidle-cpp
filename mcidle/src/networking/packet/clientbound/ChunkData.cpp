#include <networking/packet/clientbound/ChunkData.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Packet& ChunkData::Serialize()
{
	return *this;
}

inline void ChunkData::ReadSection(mcidle::ByteBuffer& buf, int ChunkX, int ChunkZ, int section)
{
	u8 bits_per_block;
	buf >> bits_per_block;

	mcidle::VarInt palette_len;
	buf >> palette_len;

	if (bits_per_block < 4)
	{
		bits_per_block = 4;
	}

	if (bits_per_block > 8)
	{
		bits_per_block = 13;
	}

	// If bits_per_block >= 13 we use direct format (no palette)
	// And palette_len.Value() == 0

	// Read the palette if it exists
	std::vector<mcidle::VarInt> palette;
	if (palette_len.Value() > 0)
	{
		palette.reserve(palette_len.Value());
		for (u32 i = 0; i < palette_len.Value(); i++)
		{
			palette.push_back(buf.Read<mcidle::VarInt>());
		}
	}

	int mask = (1 << bits_per_block) - 1;

	std::vector<u64> data;
	buf >> data;

	m_ChunkMap[section] = std::vector<s32>(SECTION_SIZE * SECTION_SIZE * SECTION_SIZE);

	if (data.size() > 0)
	{
		for (int y = 0; y < SECTION_SIZE; y++)
		{
			for (int z = 0; z < SECTION_SIZE; z++)
			{
				for (int x = 0; x < SECTION_SIZE; x++)
				{
					auto block_number = ((y * SECTION_SIZE) + z) * SECTION_SIZE + x;
					auto start_long = (block_number * bits_per_block) / 64;
					auto start_offset = (block_number * bits_per_block) % 64;
					auto end_long = ((block_number + 1) * bits_per_block - 1) / 64;

					auto val = 0;
					if (start_long == end_long)
					{
						val = data[start_long] >> start_offset;
					}
					else
					{
						auto end_offset = 64 - start_offset;
						val = (data[start_long] >> start_offset) | (data[end_long] << end_offset);
					}
					val &= mask;

					auto paletteId = palette[val].Value();

					/*auto id = paletteId >> 4;
					auto meta = paletteId & 0xF;*/

					m_ChunkMap[section][block_number] = paletteId;

					/*printf("Pos: %d, %d, %d, Block id: %d, Meta: %d\n", x + ChunkX * SECTION_SIZE,
						y + section * SECTION_SIZE, z + ChunkZ * SECTION_SIZE,
						id, meta);*/
				}
			}
		}
	}

	std::vector<u8> blockLight;
	blockLight.resize(2048);
	buf.Read(blockLight.data(), blockLight.size());

	// Only exists in the overworld
	/*std::vector<u8> skyLight;
	skyLight.resize(4096 / 2);
	buf.Read(skyLight.data(), skyLight.size());*/
}


void ChunkData::Deserialize(ByteBuffer& buf)
{
	mcidle::VarInt BitMask;
	bool groundUp;
	int ChunkX, ChunkZ;
	buf >> ChunkX;
	buf >> ChunkZ;
	buf >> groundUp;
	buf >> BitMask;

	u32 numSections = 0;
	u32 mask = BitMask.Value();

	std::vector<u8> data;
	buf >> data;

	mcidle::ByteBuffer dataBuf(data);

	s32 section = 0;
	while (mask > 0)
	{
		if (mask & 1)
			ReadSection(dataBuf, ChunkX, ChunkZ, section);
		mask >>= 1;
		section++;
	}
}

}
}
}


