#include "gmock/gmock.h"
#include "kiwer.cpp"
#include "nemo.cpp"

using namespace testing;

TEST(TradingSystem, Sample)
{
    EXPECT_EQ(1, 1);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
