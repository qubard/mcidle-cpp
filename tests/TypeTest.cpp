#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include <string>

#include "../mcidle/include/networking/types/VarInt.hpp"
#include "../mcidle/include/networking/ByteBuffer.hpp"

BOOST_AUTO_TEST_CASE(encode_test)
{
	mcidle::ByteBuffer buf;
	buf << (int)55;
	buf << (double)55.71321;
	// "Long" is s64 (8 bytes) or unsigned long
	// By default long is 4 bytes in C++
	buf << (s64)55934891;
	buf << (float)55.3;
	buf << false;
	buf << (short)2249;
	buf << mcidle::VarInt(59453);
	buf << (s64)-9223372036854775808;
	buf << (s64)9223372036854775807;
	buf << (s8)-128;
	buf << (s8)127;
	buf << (u8)0; // 0x0
	buf << (u8)255;
	buf << (u64)255;
	u8 lval = 255;
	buf << lval;

	BOOST_CHECK_EQUAL(buf.Hex(), "00 00 00 37 40 4B DB 4A 77 1C 97 0F 00 00 00 00 03 55 7F AB 42 5D 33 33 00 08 C9 BD D0 03 80 00 00 00 00 00 00 00 7F FF FF FF FF FF FF FF 80 7F 00 FF 00 00 00 00 00 00 00 FF FF");
}

BOOST_AUTO_TEST_CASE(vector_test)
{
	mcidle::ByteBuffer buf;
	std::vector<u8> b;
	b.push_back(0xFF);
	b.push_back(0x01);

	buf << b;
	buf << b;
	
	std::vector<u8> b2;
	buf >> b2;

	BOOST_CHECK_EQUAL(b2[0], 0xFF);
	BOOST_CHECK_EQUAL(b2[1], 0x01);

	buf >> b2;
	BOOST_CHECK_EQUAL(b2[0], 0xFF);
	BOOST_CHECK_EQUAL(b2[1], 0x01);
	BOOST_CHECK_EQUAL(b2[2], 0xFF);
	BOOST_CHECK_EQUAL(b2[3], 0x01);

	buf << mcidle::VarInt(543985);
	mcidle::VarInt out;
	buf >> out;
	BOOST_CHECK_EQUAL(out.Value(), 543985);

	buf << b2;
	buf >> b2;
	BOOST_CHECK_EQUAL(b2.size(), 8);
	buf << b2;
	buf >> b2;
	BOOST_CHECK_EQUAL(b2.size(), 16);
}

BOOST_AUTO_TEST_CASE(string_test)
{
	mcidle::ByteBuffer buf;
	std::string hello = "hello";
	buf << hello;
	std::string str;
	buf >> str;
	BOOST_CHECK_EQUAL(buf.Hex(), "05 68 65 6C 6C 6F");
	BOOST_CHECK_EQUAL(str, hello);
	buf << std::string("hello");
	BOOST_CHECK_EQUAL(buf.Hex(), "05 68 65 6C 6C 6F 05 68 65 6C 6C 6F");
	buf >> str;
	BOOST_CHECK_EQUAL(str, hello);

	// Test const char*
	buf.Clear();
	buf << "hello";
	BOOST_CHECK_EQUAL(buf.Hex(), "05 68 65 6C 6C 6F");
	buf >> str;
	BOOST_CHECK_EQUAL(str, hello);
}

BOOST_AUTO_TEST_CASE(decode_test) 
{
	mcidle::ByteBuffer buf;

	buf << (int)55;
	BOOST_CHECK_EQUAL(buf.Read<int>(), 55);

	buf << (double)55.71321;
	BOOST_CHECK_EQUAL(buf.Read<double>(), (double)55.71321);

	buf << (s64)55934891;
	BOOST_CHECK_EQUAL(buf.Read<s64>(), (s64)55934891);

	buf << (float)55.3;
	BOOST_CHECK_EQUAL(buf.Read<float>(), (float)55.3);

	buf << false;
	BOOST_CHECK_EQUAL(buf.Read<bool>(), false);

	buf << true;
	BOOST_CHECK_EQUAL(buf.Read<bool>(), true);

	buf << (short)2249;
	BOOST_CHECK_EQUAL(buf.Read<short>(), (short)2249);

	buf << mcidle::VarInt(59453);
	mcidle::VarInt t;
	buf >> t;
	BOOST_CHECK_EQUAL(t.Value(), 59453);

	s64 s = -9223372036854775808;
	buf << s;
	BOOST_CHECK_EQUAL(buf.Read<s64>(), s);

	s = 9223372036783548954;
	buf << s;
	BOOST_CHECK_EQUAL(buf.Read<s64>(), s);

	buf << (s8)-128;
	BOOST_CHECK_EQUAL(buf.Read<s8>(), (s8)-128);

	buf << (s8)127;
	BOOST_CHECK_EQUAL(buf.Read<s8>(), (s8)127);

	buf << (u8)0;
	BOOST_CHECK_EQUAL(buf.Read<u8>(), (u8)0);

	buf << (u8)255;
	BOOST_CHECK_EQUAL(buf.Read<u8>(), (u8)255);

	buf << (u64)1329854;
	BOOST_CHECK_EQUAL(buf.Read<u64>(), (u64)1329854);
}

template <typename T>
void GenericSizeTest(u32 count) 
{
	mcidle::ByteBuffer buf;

	for (T i = 0; i < count; i++) 
	{
		buf << i;
	}

	BOOST_CHECK_EQUAL(buf.Size(), count * sizeof(T));
}

BOOST_AUTO_TEST_CASE(size_test)
{	
	GenericSizeTest<s64>(300);
	GenericSizeTest<float>(300);
	GenericSizeTest<u32>(300);
	GenericSizeTest<s32>(300);
	GenericSizeTest<u64>(300);
	GenericSizeTest<double>(300);
}
