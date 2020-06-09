#include <networking/packet/clientbound/ChunkData.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

void ChunkData::WriteSection(s32 section, u8 bitsPerBlock)
{
	*m_FieldBuf << bitsPerBlock;

	std::vector<u64> data;
	auto dataSize = (SECTION_SIZE * SECTION_SIZE * SECTION_SIZE) * bitsPerBlock / 64;
	data.resize(dataSize);

	// Direct format, don't write palette
	// **NOTE** this is not written in later protocols!
	*m_FieldBuf << VarInt(0);

	u32 valueMask = (1 << bitsPerBlock) - 1;
	for (int x = 0; x < SECTION_SIZE; x++)
	{
		for (int z = 0; z < SECTION_SIZE; z++)
		{
			for (int y = 0; y < SECTION_SIZE; y++)
			{
				auto blockNumber = (((y * SECTION_SIZE) + z) * SECTION_SIZE) + x;
				auto startLong = (blockNumber * bitsPerBlock) / 64;
				auto startOffset = (blockNumber * bitsPerBlock) % 64;
				auto endLong = ((blockNumber + 1) * bitsPerBlock - 1) / 64;

				u64 value = m_ChunkMap[section][blockNumber];
				value &= valueMask;

				data[startLong] |= (value << startOffset);

				if (startLong != endLong)
				{
					data[endLong] = (value >> (64 - startOffset));
				}
			}
		}
	}

	if (m_LightMap.find(section) != m_LightMap.end())
	{
		// Half a byte block of light data per block, 
		// 0.5 bytes per block, 2048 bytes -> 256 u64s
		data.resize(data.size() + 256);
		std::vector<u8> light = m_LightMap[section];
		std::copy(&light.front(), &light.back(), &data[data.size() - 256]);
	}
	else
	{
	}

	// Write overworld light if in overworld

	*m_FieldBuf << data;
}

Packet& ChunkData::Serialize()
{
	*m_FieldBuf << m_ChunkX;
	*m_FieldBuf << m_ChunkZ;

	*m_FieldBuf << m_GroundUp;

	u8 bitsPerBlock = 13;

	s32 mask = 0;
	for (int section = 0; section < SECTION_SIZE; section++)
	{
		// We could alternatively use the primary bit mask here
		// but it's less flexible
		if (m_ChunkMap.find(section) != m_ChunkMap.end())
		{
			// Write the sections to the data array
			WriteSection(section, bitsPerBlock);
			mask |= 1 << section;
		}
	}

	// Write 256 bytes of garbage biome information for now
	if (m_GroundUp)
	{
		// Write 256 bytes of biome information
		m_FieldBuf->Write(m_Biomes.data(), m_Biomes.size());
	}

	// Write NBT information for block entities
	// For now, keep this 0
	*m_FieldBuf << VarInt(0);


	return *this;
}

std::unordered_map<s32, Section>& ChunkData::ChunkMap()
{
	return m_ChunkMap;
}

inline void ChunkData::ReadSection(ByteBuffer& buf, int ChunkX, int ChunkZ, int section)
{
	u8 bits_per_block;
	buf >> bits_per_block;

	VarInt palette_len;
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

	// Pretty sure this is always 0 but documentation suggests
	// there is no palette length field in the latest protocol

	// Read the palette if it exists
	std::vector<VarInt> palette;
	if (palette_len.Value() > 0)
	{
		palette.reserve(palette_len.Value());
		for (u32 i = 0; i < palette_len.Value(); i++)
		{
			palette.push_back(buf.Read<VarInt>());
		}
	}

	int mask = (1 << bits_per_block) - 1;

	std::vector<u64> data;
	buf >> data;

	m_ChunkMap[section] = std::vector<s32>(SECTION_SIZE * SECTION_SIZE * SECTION_SIZE);

	if (data.size() > 0)
	{
		for (int block_number = 0; block_number < 4096; block_number++)
		{
			int y = block_number / (SECTION_SIZE * SECTION_SIZE);
			int x = block_number % SECTION_SIZE;
			int z = (block_number % (SECTION_SIZE * SECTION_SIZE)) / SECTION_SIZE;

			auto startLong = (block_number * bits_per_block) / 64;
			auto startOffset = (block_number * bits_per_block) % 64;
			auto endLong = ((block_number + 1) * bits_per_block - 1) / 64;

			auto val = 0;

			if (startLong == endLong)
			{
				val = data[startLong] >> startOffset;
			}
			else
			{
				auto end_offset = 64 - startOffset;
				val = (data[startLong] >> startOffset) | (data[endLong] << end_offset);
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

	m_LightMap[section] = std::vector<u8>();
	m_LightMap[section].resize(2048);

	// Read half a byte per block of block light
	buf.Read(m_LightMap[section].data(), m_LightMap[section].size());

	// Only exists in the overworld
	/*std::vector<u8> skyLight;
	skyLight.resize(4096 / 2);
	buf.Read(skyLight.data(), skyLight.size());*/
}


void ChunkData::Deserialize(ByteBuffer& buf)
{
	buf >> m_ChunkX;
	buf >> m_ChunkZ;
	buf >> m_GroundUp;
	buf >> m_PrimaryBitMask;

	u32 numSections = 0;
	u32 mask = m_PrimaryBitMask.Value();

	std::vector<u8> data;
	buf >> data;

	ByteBuffer dataBuf(data);

	s32 section = 0;
	while (mask > 0)
	{
		if (mask & 1)
			ReadSection(dataBuf, m_ChunkX, m_ChunkZ, section);
		mask >>= 1;
		section++;
	}

	if (m_GroundUp)
	{
		m_Biomes.resize(256);
		dataBuf.Read(m_Biomes.data(), 256);
	}

	// Read block entities
	//VarInt numBlockEntities;
	//buf >> numBlockEntities;
}

} // ns clientbound
} // ns packet
} // ns mcidle

