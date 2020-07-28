#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

#include <unordered_map>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

namespace mcidle {
namespace game {

namespace dimension {

const s32 NETHER = -1;
const s32 OVERWORLD = 0;
const s32 END = 1;

}

namespace gamemode {

const u8 SURVIVAL = 0;
const u8 CREATIVE = 1;
const u8 ADVENTURE = 2;

} // ns game

using Section = std::vector<u64>;

typedef s64 ChunkPos;

const s32 SECTION_SIZE = 16;
const s32 SECTION_HEIGHT = SECTION_SIZE;
const s32 SECTION_WIDTH = SECTION_SIZE;
const s32 BLOCK_COUNT = SECTION_WIDTH * SECTION_HEIGHT * SECTION_SIZE;

inline game::ChunkPos CalcChunkPos(s32 x, s32 z)
{
    return (static_cast<u64>(x) << 32) | static_cast<u32>(z & 0xFFFFFFFF);
}

inline s32 ChunkPosToBlockNum(s32 x, s32 y, s32 z)
{
    return (((y * SECTION_HEIGHT) + z) * SECTION_WIDTH) + x;
}

struct Chunk {
    s32 ChunkX;
    s32 ChunkZ;
    // Maps section-y (0-15) to Section data
    std::shared_ptr<std::unordered_map<s32, Section>> Sections;
    // Maps section-y to its light information
    std::shared_ptr<std::unordered_map<s32, std::vector<u8>>> LightMap;
    std::shared_ptr<std::unordered_map<s32, std::vector<u8>>> Skylight;
    // One biome array per chunk
    std::shared_ptr<std::vector<u8>> Biomes;
    bool GroundUp;
    s32 PrimaryBitMask;
};

using ChunkMap = std::unordered_map<ChunkPos, std::shared_ptr<Chunk>>;

} // ns dimension
} // ns mcidle
