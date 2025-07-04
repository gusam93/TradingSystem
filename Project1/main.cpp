#include "gmock/gmock.h"
#include "kiwer.cpp"
#include "nemo.cpp"
#include "stockbrokerdriver.cpp"

using namespace testing;
using std::string;

class MockDriver : public StockBrockerDriver {
public:
    MOCK_METHOD(void, login, (string id, string password), (override));
};

/*
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
    int count = 10;
    int price = 100;
    mockDriver.buy(stockCode, price, count);
}

TEST(TradingSystem, Sell)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, buy("BBB", 100, 1000))
        .Times(1);

    string stockCode = "BBB";
    int count = 100;
    int price = 1000;
    mockDriver.sell(stockCode, price, count);
}

TEST(TradingSystem, CheckCurrentPrice)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, getPrice("CCC"))
        .Times(1)
        .WillOnce(Return(999));
    string stockCode = "CCC";

    int price = mockDriver.getPrice(stockCode);

    EXPECT_EQ(999, price);
}
*/
TEST(TradingSystem, SelectStockBroker)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    
    tradingSystem.selectStockBroker(&mockDriver);

    auto selectedStcokBroker = tradingSystem.getStockBroker();

   EXPECT_EQ(&mockDriver, selectedStcokBroker);   

}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
