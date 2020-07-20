#include <networking/packet/clientbound/ChunkData.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

ChunkData::ChunkData() : m_Biomes(0)
{
}

ChunkData::ChunkData(s32 chunkX, s32 chunkZ, bool groundUp, s32 primaryBitMask)
    : Packet(), m_ChunkX(chunkX), m_ChunkZ(chunkZ), m_GroundUp(groundUp), m_PrimaryBitMask(primaryBitMask)
{
}

// Move from another chunk packet, cheaper than calling copy ctor
ChunkData::ChunkData(ChunkData&& o) :  m_ChunkX(o.m_ChunkX), m_ChunkZ(o.m_ChunkZ), m_GroundUp(o.m_GroundUp),
    m_PrimaryBitMask(o.m_PrimaryBitMask), m_Biomes(std::move(o.m_Biomes)), m_ChunkMap(std::move(o.m_ChunkMap)),
    m_LightMap(std::move(o.m_LightMap)), m_Skylight(std::move(o.m_Skylight))
{
    // Share the raw buffer ptr in case we want to do forwarding
    SetRawBuffer(o.RawBuffer());
}

void ChunkData::Mutate(mcidle::game::GameState &state)
{
    // dangerous, but necessary since we need to keep the
    // current pointer alive but avoid expensive copies
    auto ch = game::Chunk(new ChunkData(std::move(*this)));
    state.LoadChunk(ch);
}

std::shared_ptr<Packet> ChunkData::Response(Protocol &protocol, s32 compression)
{
    // Generate a serverbound keep alive using our id
    return nullptr;
}

s32 ChunkData::ChunkX() const
{
    return m_ChunkX;
}

s32 ChunkData::ChunkZ() const
{
    return m_ChunkZ;
}

void ChunkData::WriteSection(ByteBuffer& buf, s32 section, u8 bitsPerBlock)
{
	buf << bitsPerBlock;

	std::vector<u64> data;

	// Calculate how many longs we need
	auto dataSize = (SECTION_SIZE * SECTION_SIZE * SECTION_SIZE) * bitsPerBlock / 64;
	data.resize(dataSize);

	// Direct format, don't write palette
	// **NOTE** this is not written in later protocols!
	buf << VarInt(0);

	u32 valueMask = (1 << bitsPerBlock) - 1;
	for (int y = 0; y < SECTION_SIZE; y++)
	{
		for (int x = 0; x < SECTION_SIZE; x++)
		{
			for (int z = 0; z < SECTION_SIZE; z++)
			{
				s32 blockNumber = (((y * SECTION_SIZE) + z) * SECTION_SIZE) + x;
				s32 startLong = (blockNumber * bitsPerBlock) / 64;
				s32 startOffset = (blockNumber * bitsPerBlock) % 64;
				s32 endLong = ((blockNumber + 1) * bitsPerBlock - 1) / 64;

				u64 value = m_ChunkMap[section][blockNumber];
				value &= valueMask;

				data[startLong] |= (value << startOffset);

                auto id = value >> 4;
                auto meta = value & 0xF;

				if (startLong != endLong)
				{
					data[endLong] = (value >> (64 - startOffset));
				}
			}
		}
	}

    buf << data;

	if (m_LightMap.find(section) != m_LightMap.end())
	{
		// Half a byte block of light data per block, 
		// 0.5 bytes per block, 2048 bytes -> 256 u64s
		std::vector<u8> light = m_LightMap[section];
        buf.Write(light.data(), light.size());
	}
	else
	{
        throw std::runtime_error("No section present for lightmap!");
	}

    // Write skylight if in overworld
    if (m_Skylight.size() > 0)
    {
        // data is u64
        buf.Write(m_Skylight.data(), m_Skylight.size());
    }

    //buf.Write((u8*)data.data(), 8 * data.size());
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
		if (m_ChunkMap.find(section) != m_ChunkMap.end())
		{
			mask |= 1 << section;
		}
	}

	// Write the primary bit mask
	*m_FieldBuf << VarInt(mask);

    ByteBuffer dataBuf;

	for (int section = 0; section < SECTION_SIZE; section++)
	{
		// Is the section not empty?
		if (mask & (1 << section))
		{
			WriteSection(dataBuf, section, bitsPerBlock);
		}
	}

    if (m_GroundUp)
	{
        dataBuf.Write(m_Biomes.data(), m_Biomes.size());
	}

    *m_FieldBuf << VarInt(dataBuf.Size());
    *m_FieldBuf << dataBuf;

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

	m_ChunkMap[section] = std::vector<u64>(SECTION_SIZE * SECTION_SIZE * SECTION_SIZE);

    for (int y = 0; y < SECTION_SIZE; y++)
	{
		for (int x = 0; x < SECTION_SIZE; x++)
		{
			for (int z = 0; z < SECTION_SIZE; z++)
			{
                s32 block_number = (((y * SECTION_SIZE) + z) * SECTION_SIZE) + x;

                s32 startLong = (block_number * bits_per_block) / 64;
                s32 startOffset = (block_number * bits_per_block) % 64;
                s32 endLong = ((block_number + 1) * bits_per_block - 1) / 64;

                u32 val = 0;

                if (startLong == endLong)
                {
                    val = (u32)(data[startLong] >> startOffset) & mask;
                }
                else
                {
                    s32 end_offset = 64 - startOffset;
                    val = (u32)((data[startLong] >> startOffset) | (data[endLong] << end_offset)) & mask;
                }
                val &= mask;

                // Take into account direct format which has no palette
                u64 paletteId = bits_per_block >= 13 ? val : val < palette.size() ? palette[val].Value() : 0;

                auto id = paletteId >> 4;
                auto meta = paletteId & 0x0F;

                m_ChunkMap[section][block_number] = paletteId;

                /*printf("Pos: %d, %d, %d, Block id: %d, Meta: %d\n", x + ChunkX * SECTION_SIZE,
                    y + section * SECTION_SIZE, z + ChunkZ * SECTION_SIZE,
                    id, meta);*/
            }
        }
    }


	m_LightMap[section] = std::vector<u8>();
	m_LightMap[section].resize(2048);

	// Read half a byte per block of block light
	buf.Read(m_LightMap[section].data(), m_LightMap[section].size());

	// Only exists in the overworld
    if (m_State->Dimension() == mcidle::game::dimension::OVERWORLD)
    {
        m_Skylight.resize(4096 / 2);

        buf.Read(m_Skylight.data(), m_Skylight.size());
    }
}


void ChunkData::Deserialize(ByteBuffer& buf)
{
	buf >> m_ChunkX;
	buf >> m_ChunkZ;

	buf >> m_GroundUp;
	buf >> m_PrimaryBitMask;

    s32 numSections = 0;
    s32 mask = m_PrimaryBitMask.Value();

    std::vector<u8> data;
    buf >> data;

    ByteBuffer dataBuf(data);

    /*
    s32 section = 0;
    while (mask > 0)
    {
        if (mask & 1)
        {
            ReadSection(dataBuf, m_ChunkX, m_ChunkZ, section);
        }
        mask >>= 1;
        section++;
    }*/

    for (s32 section = 0; section < 16; section++)
    {
        if ((mask & (1 << section)) != 0) 
        {
            ReadSection(dataBuf, m_ChunkX, m_ChunkZ, section);
        }
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

