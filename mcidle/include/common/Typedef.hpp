#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

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

} // ns dimension
} // ns mcidle
