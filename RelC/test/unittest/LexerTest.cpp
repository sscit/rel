#include "gtest/gtest.h"
#include "RelC/src/Lexer.h"

class LexerT : public Lexer
{
	public:
		LexerT(Logger const& l) : Lexer(l)  {}
		virtual ~LexerT() {}

		bool TestIsInteger(std::string const &s)
		{
			return IsInteger(s);
		}

		bool TestIsIdentifier(std::string const &s)
		{
			return IsIdentifier(s);
		}

		bool TestIsString(std::string const &s)
		{
			return IsString(s);
		}

};

TEST(LexerTest, IsIdentifier) {
	Logger tmp;
	LexerT l(tmp);

	EXPECT_TRUE ( l.TestIsIdentifier("asbd") );
	EXPECT_TRUE ( l.TestIsIdentifier("Xhdsa") );
	EXPECT_TRUE ( l.TestIsIdentifier("JDASU") );
	EXPECT_TRUE ( l.TestIsIdentifier("ash_dhuzd") );
	EXPECT_TRUE ( l.TestIsIdentifier("XB_dhs_ASdzz") );

	EXPECT_FALSE ( l.TestIsIdentifier("1abs") );
	EXPECT_FALSE ( l.TestIsIdentifier("/§&dasdas") );
	EXPECT_FALSE ( l.TestIsIdentifier("asdhdja_&dhas") );
	EXPECT_FALSE ( l.TestIsIdentifier("-1273t67") );
	EXPECT_FALSE ( l.TestIsIdentifier("1237") );
	EXPECT_FALSE ( l.TestIsIdentifier(" ") );
}

TEST(LexerTest, IsString)
{
	Logger tmp;
	LexerT l(tmp);

	EXPECT_TRUE ( l.TestIsString("asbd") );
	EXPECT_TRUE ( l.TestIsString("A") );
	EXPECT_TRUE ( l.TestIsString("Xh---dsa") );
	EXPECT_FALSE ( l.TestIsString("Xh-\r-dsa") );
	EXPECT_FALSE ( l.TestIsString("Xh-\n-dsa") );
	EXPECT_FALSE ( l.TestIsString("Xh-\r\n-dsa") );
	EXPECT_FALSE ( l.TestIsString("\r") );
	EXPECT_FALSE ( l.TestIsString("\n") );
	EXPECT_FALSE ( l.TestIsString(" ") );
	EXPECT_FALSE ( l.TestIsString("     xx  ") );
}


TEST(LexerTest, IsInteger) {
	Logger tmp;
	LexerT l(tmp);

	EXPECT_TRUE ( l.TestIsInteger("-1") );
	EXPECT_TRUE ( l.TestIsInteger("0") );
	EXPECT_TRUE ( l.TestIsInteger("1") );
	EXPECT_TRUE ( l.TestIsInteger("17262") );
	EXPECT_TRUE ( l.TestIsInteger("-14726") );

	EXPECT_FALSE ( l.TestIsInteger("-1x4726") );
	EXPECT_FALSE ( l.TestIsInteger("1x4726") );
	EXPECT_FALSE ( l.TestIsInteger("$1x4726") );
	EXPECT_FALSE ( l.TestIsInteger("a-1x4726") );
	EXPECT_FALSE ( l.TestIsInteger("X") );
	EXPECT_FALSE ( l.TestIsInteger("123sa") );
}
