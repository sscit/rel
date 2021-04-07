#include "gtest/gtest.h"
#include "rel-cli/src/Cli.h"

class CliTestFixture : public ::testing::Test {
   protected:
    CliTestFixture() {}

    void SetUp() override {}

    void TearDown() override {}

    Cli rel_cli;
};

TEST_F(CliTestFixture, ReturnValueValidProject) {
    int argc = 2;
    const char* tmp[] = {"rel_cli", "./test/small"};
    char** argv = (char**)tmp;

    EXPECT_EQ(0, rel_cli.Run(argc, argv));
}

TEST_F(CliTestFixture, ReturnValueInvalidProject) {
    int argc = 2;
    const char* tmp[] = {"rel_cli", "./test/invalid"};
    char** argv = (char**)tmp;

    EXPECT_EQ(1, rel_cli.Run(argc, argv));
}