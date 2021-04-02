#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

namespace mcidle
{
namespace game
{

namespace dimension
{

const s32 NETHER = -1;
const s32 OVERWORLD = 0;
const s32 END = 1;

}  // namespace dimension

namespace gamemode
{

const u8 SURVIVAL = 0;
const u8 CREATIVE = 1;
const u8 ADVENTURE = 2;

}  // namespace gamemode

// Each element in this array holds the encoded
// block ID (typically bits_per_block <= 13 so we round
// to a short)
using Section = std::vector<u16>;

typedef s64 ChunkPos;

const s32 SECTION_SIZE = 16;
const s32 SECTION_HEIGHT = SECTION_SIZE;
const s32 SECTION_WIDTH = SECTION_SIZE;
const s32 BLOCK_COUNT = SECTION_WIDTH * SECTION_HEIGHT * SECTION_SIZE;

inline game::ChunkPos CalcChunkPos(s32 x, s32 z)
{
	return (static_cast<u64>(x) << 32) | static_cast<u32>(z & 0xFFFFFFFF);
}

inline s32 ChunkPosToBlockNumLight(s32 x, s32 y, s32 z)
{
	// return y * (8 * 16) + (z * 8) + x;
	return ((y * 16) + z) * 8 + static_cast<s32>(x / 2);
}

inline s32 ChunkPosToBlockNum(s32 x, s32 y, s32 z)
{
	return (((y * SECTION_HEIGHT) + z) * SECTION_WIDTH) + x;
}

inline u8 LightAt(
	std::shared_ptr<std::unordered_map<s32, std::vector<u8>>> & lightMap, s32 x,
	s32 y, s32 z)
{
	auto pos = ChunkPosToBlockNumLight(x & 0xF, y & 0xF, z & 0xF);
	s32 chunkY = y / SECTION_SIZE;

	auto val = (*lightMap)[chunkY][pos];
	u8 mask = 0xF;
	return x % 2 == 1 ? (val >> 4) & mask : val & mask;
}

inline void SetLightAt(
	std::shared_ptr<std::unordered_map<s32, std::vector<u8>>> & lightMap, s32 x,
	s32 y, s32 z, s8 val)
{
	auto pos = ChunkPosToBlockNumLight(x & 0xF, y & 0xF, z & 0xF);
	s32 chunkY = y / SECTION_SIZE;

	if (x % 2 == 1)
	{
		(*lightMap)[chunkY][pos] =
			(u8)((val << 4)) | ((*lightMap)[chunkY][pos] & 0xF);
	}
	else
	{
		(*lightMap)[chunkY][pos] = val | (u8)((*lightMap)[chunkY][pos] & 0xF0);
	}
}

// Intermediate/deserialized representation of a chunk
struct Chunk
{
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
	s32 Dimension;
};

inline void CreateNewSection(std::shared_ptr<Chunk> & chunk, s32 y)
{
	(*chunk->Sections)[y] = Section(BLOCK_COUNT);
	(*chunk->LightMap)[y] = std::vector<u8>(BLOCK_COUNT >> 1);

	// Only generate skylight for the section if in overworld
	if (chunk->Dimension == game::dimension::OVERWORLD)
		(*chunk->Skylight)[y] = std::vector<u8>(BLOCK_COUNT >> 1);

	chunk->PrimaryBitMask |= 1 << y;
}

// A mapping from the chunk position to an deserialized chunk representation
using ChunkMap = std::unordered_map<ChunkPos, std::shared_ptr<Chunk>>;

}  // namespace game
}  // namespace mcidle
