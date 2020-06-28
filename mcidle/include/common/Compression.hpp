#pragma once

#include <zlib.h>

#include <networking/ByteBuffer.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {

inline std::shared_ptr<ByteBuffer> Decompress(std::shared_ptr<ByteBuffer>& buf)
{
	VarInt compressLen;
	*buf >> compressLen;
	// Uncompressed length
	uLongf len = compressLen.Value();

	// Is the buffer compressed?
	if (len > 0)
	{
		auto uncompressed = std::make_shared<ByteBuffer>();
		uncompressed->Resize(len);

		s32 succ = uncompress((Bytef*)uncompressed->Front(), (uLongf*)&len,
			(const Bytef*)&buf->Peek(), (uLong)(buf->Size() - buf->ReadOffset()));

        // Invalid decompression
        if (succ != Z_OK) 
            return nullptr;

		// Set the write offset to the end of the buffer just to be safe
		// if we ever use this buffer again for writing
		uncompressed->SeekWrite(len);
		return std::move(uncompressed);
	} 

	return std::move(buf);
}

inline std::shared_ptr<ByteBuffer> Compress(ByteBuffer& buf)
{
	auto out = std::make_shared<ByteBuffer>();
	out->Resize(buf.WriteSize());
	auto len = out->Size();
	compress(out->Front(), (uLongf*)&len, buf.Front(), buf.WriteSize());
	out->SeekWrite(len);
	// The buffer has been compressed to len bytes
	out->Resize(len);
	return out;
}

} // ns mcidle
