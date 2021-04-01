#include <networking/packet/clientbound/ChunkData.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        ChunkData::ChunkData()
        {
            m_Sections =
                std::make_shared<std::unordered_map<s32, game::Section>>();
            m_LightMap =
                std::make_shared<std::unordered_map<s32, std::vector<u8>>>();
            m_Skylight =
                std::make_shared<std::unordered_map<s32, std::vector<u8>>>();
            m_Biomes = std::make_shared<std::vector<u8>>();
        }

        ChunkData::ChunkData(s32 chunkX, s32 chunkZ, bool groundUp,
                             s32 primaryBitMask)
            : Packet()
            , m_ChunkX(chunkX)
            , m_ChunkZ(chunkZ)
            , m_GroundUp(groundUp)
            , m_PrimaryBitMask(primaryBitMask)
        {
        }

        ChunkData::ChunkData(game::Chunk &chunk)
            : m_ChunkX(chunk.ChunkX)
            , m_ChunkZ(chunk.ChunkZ)
            , m_Sections(chunk.Sections)
            , m_Skylight(chunk.Skylight)
            , m_Biomes(chunk.Biomes)
            , m_GroundUp(chunk.GroundUp)
            , m_PrimaryBitMask(chunk.PrimaryBitMask)
            , m_LightMap(chunk.LightMap)
            , m_Dimension(chunk.Dimension)
        {
        }

        // Move from another chunk packet, cheaper than calling copy ctor
        ChunkData::ChunkData(ChunkData &&o)
            : m_ChunkX(o.m_ChunkX)
            , m_ChunkZ(o.m_ChunkZ)
            , m_GroundUp(o.m_GroundUp)
            , m_PrimaryBitMask(o.m_PrimaryBitMask)
            , m_Biomes(std::move(o.m_Biomes))
            , m_Sections(std::move(o.m_Sections))
            , m_LightMap(std::move(o.m_LightMap))
            , m_Skylight(std::move(o.m_Skylight))
            , m_Dimension(o.m_Dimension)
        {
            // Share the raw buffer ptr in case we want to do forwarding
            SetRawBuffer(o.RawBuffer());
        }

        void ChunkData::Mutate(mcidle::game::GameState &state)
        {
            // dangerous, but necessary since we need to keep the
            // current pointer alive but avoid expensive copies
            auto p = std::make_shared<game::Chunk>();
            p->ChunkX = m_ChunkX;
            p->ChunkZ = m_ChunkZ;
            p->Sections = std::move(m_Sections);
            p->Skylight = std::move(m_Skylight);
            p->LightMap = std::move(m_LightMap);
            p->Biomes = std::move(m_Biomes);
            p->GroundUp = m_GroundUp;
            p->PrimaryBitMask = m_PrimaryBitMask.Value();
            p->Dimension = m_Dimension;

            state.LoadChunk(p);
            printf("Loaded chunk %d %d into state!\n", m_ChunkX, m_ChunkZ);
        }

        std::shared_ptr<Packet> ChunkData::Response(Protocol &protocol,
                                                    s32 compression)
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

        inline void ChunkData::WriteSection(ByteBuffer &buf, s32 section,
                                            u8 bitsPerBlock)
        {
            buf << bitsPerBlock;

            std::vector<u64> data;

            // Calculate how many longs we need
            auto dataSize = (game::BLOCK_COUNT)*bitsPerBlock / 64;
            data.resize(dataSize);

            // Direct format, don't write palette
            // **NOTE** this is not written in later protocols!
            buf << VarInt(0);

            u32 valueMask = (1 << bitsPerBlock) - 1;
            for (s32 blockNumber = 0; blockNumber < game::BLOCK_COUNT;
                 blockNumber++)
            {
                s32 startLong = (blockNumber * bitsPerBlock) / 64;

                s32 startOffset = (blockNumber * bitsPerBlock) % 64;
                s32 endLong = ((blockNumber + 1) * bitsPerBlock - 1) / 64;

                u64 value = (*m_Sections)[section][blockNumber];
                value &= valueMask;

                data[startLong] |= value << startOffset;

                if (startLong != endLong)
                {
                    data[endLong] |= value >> (64 - startOffset);
                }
            }

            // Convert each long back to big endian (byte ordering)
            for (auto &l : data)
                std::reverse((u8 *)&l, (u8 *)&l + 8);

            // Write from the temporary data buffer to the byte buffer
            buf << data;

            if (m_LightMap->find(section) != m_LightMap->end())
            {
                // Half a byte block of light data per block,
                // 0.5 bytes per block, 2048 bytes -> 256 u64s
                std::vector<u8> light = (*m_LightMap)[section];
                buf.Write(light.data(), light.size());
            }
            else
            {
                throw std::runtime_error("No section present for lightmap!");
            }

            // Write skylight if in overworld
            if (m_Dimension == game::dimension::OVERWORLD)
            {
                buf.Write((*m_Skylight)[section].data(),
                          (*m_Skylight)[section].size());
            }
        }

        Packet &ChunkData::Serialize()
        {
            *m_FieldBuf << m_ChunkX;
            *m_FieldBuf << m_ChunkZ;
            *m_FieldBuf << m_GroundUp;

            u8 bitsPerBlock = 13;

            s32 primaryMask = m_PrimaryBitMask.Value();

            // Write the primary bit mask
            *m_FieldBuf << m_PrimaryBitMask;

            ByteBuffer dataBuf;

            for (s32 section = 0; section < game::SECTION_SIZE; section++)
            {
                // Is the section not empty?
                if (primaryMask & (1 << section))
                {
                    WriteSection(dataBuf, section, bitsPerBlock);
                }
            }

            if (m_GroundUp)
            {
                dataBuf.Write(m_Biomes->data(), m_Biomes->size());
            }

            *m_FieldBuf << VarInt(dataBuf.Size());
            *m_FieldBuf << dataBuf;

            // Write NBT information for block entities
            // For now, keep this 0
            *m_FieldBuf << VarInt(0);

            return *this;
        }

        inline void ChunkData::ReadSection(ByteBuffer &buf, int ChunkX,
                                           int ChunkZ, int section)
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

            // Reads 256 longs or 2048 bytes, 4 bits per block -> 2048 bytes
            std::vector<u64> data;
            buf >> data;

            // Convert the data array to big endian (byte ordering)
            for (u64 &l : data)
                std::reverse((u8 *)&l, (u8 *)&l + 8);

            // Initialize the section
            (*m_Sections)[section] = game::Section(game::BLOCK_COUNT);

            for (s32 blockNumber = 0; blockNumber < game::BLOCK_COUNT;
                 blockNumber++)
            {
                s32 startLong = (blockNumber * bits_per_block) / 64;
                s32 startOffset = (blockNumber * bits_per_block) % 64;
                s32 endLong = ((blockNumber + 1) * bits_per_block - 1) / 64;

                u32 val = 0;

                if (startLong == endLong)
                {
                    val = (u32)(data[startLong] >> startOffset);
                }
                else
                {
                    s32 end_offset = 64 - startOffset;
                    val = (u32)((data[startLong] >> startOffset) |
                                (data[endLong] << end_offset));
                }
                val &= mask;

                // Take into account direct format which has no palette
                u64 paletteId = bits_per_block >= 13   ? val
                                : val < palette.size() ? palette[val].Value()
                                                       : 0;

                // Store the block
                (*m_Sections)[section][blockNumber] = paletteId;
            }

            (*m_LightMap)[section] = std::vector<u8>();
            (*m_LightMap)[section].resize(game::BLOCK_COUNT >> 1);

            // Read half a byte per block of block light
            buf.Read((*m_LightMap)[section].data(),
                     (*m_LightMap)[section].size());

            s32 dim = m_State->Dimension();

            // Set the dimension after deserializing which avoids
            // lookup on state later when serializing
            m_Dimension = dim;

            // Only exists in the overworld
            if (dim == mcidle::game::dimension::OVERWORLD)
            {
                (*m_Skylight)[section] = std::vector<u8>();
                // Half a byte of skylight data per block
                (*m_Skylight)[section].resize(game::BLOCK_COUNT >> 1);

                buf.Read((*m_Skylight)[section].data(),
                         (*m_Skylight)[section].size());
            }
        }

        void ChunkData::Deserialize(ByteBuffer &buf)
        {
            buf >> m_ChunkX;
            buf >> m_ChunkZ;

            buf >> m_GroundUp;
            buf >> m_PrimaryBitMask;

            s32 mask = m_PrimaryBitMask.Value();

            // Create a temporary byte storage
            // for the data array
            std::vector<u8> data;
            buf >> data;

            // Use the data array as a byte buffer
            ByteBuffer dataBuf(data);

            s32 section = 0;
            while (mask > 0)
            {
                if (mask & 1)
                {
                    ReadSection(dataBuf, m_ChunkX, m_ChunkZ, section);
                }
                mask >>= 1;
                section++;
            }

            if (m_GroundUp)
            {
                m_Biomes->resize(256);
                dataBuf.Read(m_Biomes->data(), 256);
            }

            // Read block entities
            VarInt numBlockEntities;
            buf >> numBlockEntities;

            if (numBlockEntities.Value() != 0)
            {
                printf("Num block entities: %d\n", numBlockEntities.Value());

                s32 len = numBlockEntities.Value();
                while (len > 0)
                {
                    nbt::TagCompound tag;
                    buf >> tag;
                    len--;
                }
            }
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle
