#include "gtest/gtest.h"
#include "rel-ls/src/Uri.h"

class UriTestFixture : public ::testing::Test {
protected:
  UriTestFixture() { }
  void SetUp() override { }
  void TearDown() override { }
};

TEST_F(UriTestFixture, CheckUriElements)
{
    std::string sample_uri = "file:///home/myuser/tmp/reltest";
    Uri my_uri(sample_uri);

    EXPECT_EQ(sample_uri, my_uri.GetUri());
    EXPECT_EQ("file", my_uri.GetScheme());
    EXPECT_EQ("/home/myuser/tmp/reltest", my_uri.GetPath());
    EXPECT_EQ(DataType::Unknown, my_uri.GetDataType());
}

TEST_F(UriTestFixture, CheckUriElements2)
{
    std::string sample_uri = "file://example.com:1234/home/myuser/tmp/reltest";
    Uri my_uri(sample_uri);

    EXPECT_EQ(sample_uri, my_uri.GetUri());
    EXPECT_EQ("file", my_uri.GetScheme());
    EXPECT_EQ("/home/myuser/tmp/reltest", my_uri.GetPath());
}

TEST_F(UriTestFixture, filetype1)
{
    std::string sample_uri = "file://example.com:1234/home/myuser/tmp/spec.rs";
    Uri my_uri(sample_uri);

    EXPECT_TRUE(my_uri.IsRequirementsSpecification());
    EXPECT_FALSE(my_uri.IsRequirementsData());
    EXPECT_EQ(DataType::RequirementsSpecification, my_uri.GetDataType());
}

TEST_F(UriTestFixture, filetype2)
{
    std::string sample_uri = "file://example.com:1234/home/myuser/tmp/data.rd";
    Uri my_uri(sample_uri);

    EXPECT_FALSE(my_uri.IsRequirementsSpecification());
    EXPECT_TRUE(my_uri.IsRequirementsData());
    EXPECT_EQ(DataType::RequirementsData, my_uri.GetDataType());
}

TEST_F(UriTestFixture, Factory)
{
    std::string const path = "/home/myuser/tmp/data.rd";
    Uri my_uri = Uri::CreateFileUriFromPath(path);

    EXPECT_EQ(path, my_uri.GetPath());
    EXPECT_EQ("file://"+path, my_uri.GetUri());
}