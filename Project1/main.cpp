#include "gmock/gmock.h"
#include "kiwer.cpp"
#include "nemo.cpp"
#include "stockbrokerdriver.cpp"

using namespace testing;
using std::string;

class MockDriver : public StockBrokerDriver {
public:
    MOCK_METHOD(void, login, (string id, string password), (override));
    MOCK_METHOD(void, sell, (string stockCode, int price, int count), (override));
    MOCK_METHOD(void, buy, (string stockCode, int price, int count), (override));
    MOCK_METHOD(int, getPrice, (string stockCode, int msDelay), (override));
};

class TradingSystemFixture : public Test {
public:
    NiceMock<MockDriver> mockDriver;
    AutoTradingSystem tradingSystem;

    const string STOCK_ID = "SEC_CRA";
    const int STOCK_PRICE = 100;
    const int STOCK_COUNT = 1000;
    const int STOCK_MAX_PRICE = 9999;
    const int DELAY1000 = 1000;

protected:
    void SetUp() override {
        //tradingSystem.selectStockBroker(&mockDriver);
    }
};


TEST_F(TradingSystemFixture, Login)
{
    EXPECT_CALL(mockDriver, login("ABC", "1234"))
        .Times(1);

    string id = "ABC";
    string password = "1234";   
    mockDriver.login(id, password);
}

TEST_F(TradingSystemFixture, Buy)
{
    EXPECT_CALL(mockDriver, buy(STOCK_ID, STOCK_PRICE, STOCK_COUNT))
        .Times(1);

    mockDriver.buy(STOCK_ID, STOCK_PRICE, STOCK_COUNT);
}

TEST_F(TradingSystemFixture, Sell)
{
    EXPECT_CALL(mockDriver, sell(STOCK_ID, STOCK_PRICE, STOCK_COUNT))
        .Times(1);

    mockDriver.sell(STOCK_ID, STOCK_PRICE, STOCK_COUNT);
}

TEST_F(TradingSystemFixture, CheckCurrentPrice)
{
    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, DELAY1000))
        .Times(1)
        .WillOnce(Return(999));

    int price = mockDriver.getPrice(STOCK_ID, DELAY1000);

    EXPECT_EQ(999, price);
}

TEST_F(TradingSystemFixture, SelectStockBroker)
{
    tradingSystem.selectStockBroker(&mockDriver);

    auto selectedStcokBroker = tradingSystem.getStockBroker();

   EXPECT_EQ(&mockDriver, selectedStcokBroker);   

}

TEST_F(TradingSystemFixture, BuyNiceTimingWithFail)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000));

    bool isSuccess = tradingSystem.buyNiceTiming(STOCK_ID, STOCK_MAX_PRICE);
    EXPECT_FALSE(isSuccess);
}

TEST_F(TradingSystemFixture, BuyNiceTimingFailWithNotEnoughPrice)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000))
        .WillOnce(Return(1000));

    int maxPrice = 100;
    bool isSuccess = tradingSystem.buyNiceTiming(STOCK_ID, maxPrice);
    EXPECT_FALSE(isSuccess);
}

TEST_F(TradingSystemFixture, BuyNiceTimingWithSuccess)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(1100))
        .WillOnce(Return(1200));

    EXPECT_CALL(mockDriver, buy(STOCK_ID, _, _))
		.Times(AtLeast(1));

    bool isSuccess = tradingSystem.buyNiceTiming(STOCK_ID, 99999);
    EXPECT_TRUE(isSuccess);
}

TEST_F(TradingSystemFixture, SellNiceTimingWithFail)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillOnce(Return(1000))
        .WillRepeatedly(Return(1100));

     int count = 100;

    bool isSuccess = tradingSystem.sellNiceTiming(STOCK_ID, count);
    EXPECT_FALSE(isSuccess);
}

TEST_F(TradingSystemFixture, SellNiceTimingWithSuccess)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(900))
        .WillOnce(Return(800));

    int count = 100;

    bool isSuccess = tradingSystem.sellNiceTiming(STOCK_ID, count);
    EXPECT_TRUE(isSuccess);
}

TEST_F(TradingSystemFixture, BuyNiceTimingExceptionWithoutSelectBroker)
{
    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(900))
        .WillOnce(Return(800));

    EXPECT_THROW(tradingSystem.buyNiceTiming(STOCK_ID, STOCK_MAX_PRICE), std::exception);
}

TEST_F(TradingSystemFixture, SellNiceTimingExceptionWithoutSelectBroker)
{
    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(1000))
        .WillOnce(Return(900))
        .WillOnce(Return(800));

    int count = 100;
    EXPECT_THROW(tradingSystem.sellNiceTiming(STOCK_ID, count), std::exception);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
