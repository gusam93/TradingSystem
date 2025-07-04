#include "gmock/gmock.h"
#include "kiwer.cpp"
#include "nemo.cpp"
#include "stockbrokerdriver.cpp"

using namespace testing;
using std::string;

class MockDriver : public StockBrockerDriver {
public:
    MOCK_METHOD(void, login, (string id, string password), (override));
    MOCK_METHOD(void, sell, (string stockCode, int price, int count), (override));
    MOCK_METHOD(void, buy, (string stockCode, int price, int count), (override));
    MOCK_METHOD(int, getPrice, (string stockCode, int msDelay), (override));
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
    EXPECT_CALL(mockDriver, buy("AAA", 10, 100))
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

TEST(TradingSystem, CheckCurrentPrice)
{
    NiceMock<MockDriver> mockDriver;
	string stockCode = "CCC";
    EXPECT_CALL(mockDriver, getPrice(stockCode, 1000))
        .Times(1)
        .WillOnce(Return(999));

    int price = mockDriver.getPrice(stockCode, 1000);

    EXPECT_EQ(999, price);
}

TEST(TradingSystem, SelectStockBroker)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    
    tradingSystem.selectStockBroker(&mockDriver);

    auto selectedStcokBroker = tradingSystem.getStockBroker();

   EXPECT_EQ(&mockDriver, selectedStcokBroker);   

}

TEST(TradingSystem, BuyNiceTimingWithFail)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice("DDD", _))
        .Times(AtLeast(2))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000));

    string stockCode = "DDD";
    int maxPrice = 9999;

    bool isSuccess = tradingSystem.buyNiceTiming(stockCode, maxPrice);
    EXPECT_FALSE(isSuccess);
}

TEST(TradingSystem, BuyNiceTimingFailWithNotEnoughPrice)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice("DDD", _))
        .Times(AtLeast(2))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000));

    string stockCode = "DDD";
    int maxPrice = 100;

    bool isSuccess = tradingSystem.buyNiceTiming(stockCode, maxPrice);
    EXPECT_FALSE(isSuccess);
}

TEST(TradingSystem, BuyNiceTimingWithSuccess)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice("EEE", _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(1100))
        .WillOnce(Return(1200));

    EXPECT_CALL(mockDriver, buy("EEE", _, _))
		.Times(AtLeast(1));

    string stockCode = "EEE";
    int maxPrice = 99999;

    bool isSuccess = tradingSystem.buyNiceTiming(stockCode, maxPrice);
    EXPECT_TRUE(isSuccess);
}

TEST(TradingSystem, SellNiceTimingWithFail)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice("FFF", _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(1100))
        .WillOnce(Return(1000));

    string stockCode = "FFF";
    int count = 100;

    bool isSuccess = tradingSystem.sellNiceTiming(stockCode, count);
    EXPECT_FALSE(isSuccess);
}
TEST(TradingSystem, SellNiceTimingWithSuccess)
{
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice("GGG", _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(900))
        .WillOnce(Return(800));

    string stockCode = "GGG";
    int count = 100;

    bool isSuccess = tradingSystem.sellNiceTiming(stockCode, count);
    EXPECT_TRUE(isSuccess);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
