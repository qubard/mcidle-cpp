#define BOOST_TEST_MODULE VarLongTest
#include <boost/test/included/unit_test.hpp>

#include "../mcidle/include/networking/types/VarLong.hpp"
#include "../mcidle/include/networking/ByteBuffer.hpp"

BOOST_AUTO_TEST_CASE(encode_decode_test)
{
	s64 slng = 9223372036854775807;
	mcidle::ByteBuffer buf;
	buf << mcidle::VarLong(slng);
	BOOST_CHECK_EQUAL(buf.Hex(), "FF FF FF FF FF FF FF FF 7F");

	mcidle::VarLong lng;
	buf >> lng;
	BOOST_CHECK_EQUAL(lng.Value(), slng);

	slng = -9223372036854775808;

	buf.Clear();
	buf << mcidle::VarLong(slng);
	BOOST_CHECK_EQUAL(buf.Hex(), "80 80 80 80 80 80 80 80 80 01");

	buf >> lng;
	BOOST_CHECK_EQUAL(lng.Value(), slng);

	buf << mcidle::VarLong(9223372036854775807);
	buf << mcidle::VarLong(-972036854775808);

	mcidle::VarLong a;
	mcidle::VarLong b;
	buf >> a;
	buf >> b;
	BOOST_CHECK_EQUAL(a.Value(), 9223372036854775807);
	BOOST_CHECK_EQUAL(b.Value(), -972036854775808);
}
