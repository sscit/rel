#include "gtest/gtest.h"
#include "rel-lib/src/FileEngine.h"

class FileEngineFixture : public ::testing::Test, public FileEngine {
protected:
  FileEngineFixture() : FileEngine(logger) {

  }

  void SetUp() override {
      //logger.SetLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override {

  }

  Logger logger;
};

TEST_F(FileEngineFixture, ParseDirectoriesRecursive) {
    SetSearchRecursive(true);
    EXPECT_TRUE(GetSearchRecursive());

    SetSearchRecursive(false);
    EXPECT_FALSE(GetSearchRecursive());
}


TEST_F(FileEngineFixture, DirectoriesToSearchFor1) {
    SetDirectory("/bla");
    SetDirectory("/tmp");
    EXPECT_EQ(directories.size(), 2);
}

TEST_F(FileEngineFixture, SearchFileNonRecursive1) {
    SetDirectory("./test/small");
    SetSearchRecursive(false);

    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 2);
}

TEST_F(FileEngineFixture, SearchFileNonRecursive2) {
    SetDirectory("./test/big");
    SetSearchRecursive(false);

    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 2);
}

TEST_F(FileEngineFixture, SearchFileRecursive1) {
    SetDirectory("./test/big");
    SetSearchRecursive(true);

    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 7);
}

TEST_F(FileEngineFixture, NoDirectorySpecified) {
    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 0);
}

TEST_F(FileEngineFixture, SearchMultipleDirs) {
    SetDirectory("./test/small");
    SetDirectory("./test/big");
    SetSearchRecursive(false);

    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 4);
}

TEST_F(FileEngineFixture, SearchMultipleDirsRecursively) {
    SetDirectory("./test/small");
    SetDirectory("./test/big");
    SetSearchRecursive(true);

    std::vector<FileTokenData> res = GetListOfFiles();

    EXPECT_EQ(res.size(), 9);
}