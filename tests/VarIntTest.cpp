#define BOOST_TEST_MODULE VarIntTest
#include <boost/test/included/unit_test.hpp>

#include "../mcidle/include/networking/types/VarInt.hpp"
#include "../mcidle/include/networking/ByteBuffer.hpp"

BOOST_AUTO_TEST_CASE(decode_test)
{
	mcidle::ByteBuffer buf;
	buf << mcidle::VarInt((s32)9);
	buf << mcidle::VarInt((s32)127);
	buf << mcidle::VarInt((s32)128);
	buf << mcidle::VarInt(255);
	buf << mcidle::VarInt(-2147483648);
	buf << mcidle::VarInt(2147483647);
	buf << mcidle::VarInt(-1);
	buf << mcidle::VarInt(0);
	buf << mcidle::VarInt(2147483647);
	buf << mcidle::VarInt(-1);
	buf << (u8)0;

	mcidle::VarInt x;

	BOOST_TEST((buf.Read<mcidle::VarInt>() == 9));
	buf >> x;
	BOOST_TEST((x == 127));
	buf >> x;
	BOOST_TEST((x == 128));
	buf >> x;
	BOOST_TEST((x == 255));
	buf >> x;
	BOOST_TEST((x == -2147483648));
	buf >> x;
	BOOST_TEST((x == 2147483647));
	buf >> x;
	BOOST_TEST((x == -1));
	buf >> x;
	BOOST_TEST((x == 0));
	buf >> x;
	BOOST_TEST((x == 2147483647));
	buf >> x;
	BOOST_TEST((x == -1));
	buf >> x;
	BOOST_TEST((x == 0));

	buf << mcidle::VarInt(-1);
	buf >> x;
	BOOST_TEST((x == -1));
}

BOOST_AUTO_TEST_CASE(encode_test)
{
	mcidle::ByteBuffer buf;
	auto t = mcidle::VarInt(9);
	buf << t; // lvalue

	BOOST_CHECK_EQUAL(buf.Hex(), "09");

	buf.Clear();
	buf << mcidle::VarInt((s64)5);
	BOOST_CHECK_EQUAL(buf.Hex(), "05");

	buf.Clear();
	buf << mcidle::VarInt(9); // rvalue
	BOOST_CHECK_EQUAL(buf.Hex(), "09");
	
	buf.Clear();
	buf << mcidle::VarInt(127);
	BOOST_CHECK_EQUAL(buf.Hex(), "7F");

	buf.Clear();
	buf << mcidle::VarInt(128);
	BOOST_CHECK_EQUAL(buf.Hex(), "80 01");
	
	buf.Clear();
	buf << mcidle::VarInt(255);
	BOOST_CHECK_EQUAL(buf.Hex(), "FF 01");

	buf.Clear();
	buf << mcidle::VarInt(-2147483648);
	BOOST_CHECK_EQUAL(buf.Hex(), "80 80 80 80 08");

	buf.Clear();
	buf << mcidle::VarInt(2147483647);
	BOOST_CHECK_EQUAL(buf.Hex(), "FF FF FF FF 07");

	buf.Clear();
	buf << mcidle::VarInt(-1);
	BOOST_CHECK_EQUAL(buf.Hex(), "FF FF FF FF 0F");

	buf.Clear();
	buf << mcidle::VarInt(0);
	BOOST_CHECK_EQUAL(buf.Hex(), "00");


	buf.Clear();
	buf << mcidle::VarInt(2147483647);
	buf << mcidle::VarInt(-1);
	buf << (u8) 0;

	BOOST_CHECK_EQUAL(buf.Hex(), "FF FF FF FF 07 FF FF FF FF 0F 00");
}
