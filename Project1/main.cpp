#include "gmock/gmock.h"
#include "kiwer.cpp"
#include "nemo.cpp"

using namespace testing;
using std::string;

class MockDriver {
public:
    MOCK_METHOD(void, login, (std::string id, std::string password), ());
    MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), ());
    MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), ());
};

TEST(TradingSystem, Login)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, login("ABC", "1234"))
        .Times(1);

    string id = "ABC";
    string password = "1234";   
    mockDriver.login(id, password);
}

TEST(TradingSystem, Buy)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, buy("AAA", 10,100))
        .Times(1);

    string stockCode = "AAA";
    int price = 10;
    int count = 100;
    mockDriver.buy(stockCode, price, count);
}

TEST(TradingSystem, Sell)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, sell("BBB", 100, 1000))
        .Times(1);

    string stockCode = "BBB";
    int price = 100;
    int count = 1000;
    mockDriver.sell(stockCode, price, count);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
