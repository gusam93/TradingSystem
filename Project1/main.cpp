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
    const int STOCK_NUM_SELLING_COUNT = 100;
    const int DELAY1000 = 1000;

    const int CURRENT_PRICE = 1000;
    const int RISING_PRICE = 1100;
    const int DOUBLE_RISING_PRICE = 1200;
    const int FALLING_PRICE = 900;
    const int DOUBLE_FALLING_PRICE = 800;

    const int MAX_CASH = 99999;

protected:
    void SetUp() override {
        //tradingSystem.selectStockBroker(&mockDriver);
    }
};


TEST_F(TradingSystemFixture, Login)
{
    EXPECT_CALL(mockDriver, login("USER_ID", "1234"))
        .Times(1);

    string id = "USER_ID";
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
        .WillOnce(Return(STOCK_PRICE));

    EXPECT_EQ(STOCK_PRICE, mockDriver.getPrice(STOCK_ID, DELAY1000));
}

TEST_F(TradingSystemFixture, SelectStockBroker)
{
    tradingSystem.selectStockBroker(&mockDriver);

   EXPECT_EQ(&mockDriver, tradingSystem.getStockBroker());
}

TEST_F(TradingSystemFixture, BuyNiceTimingWithFail)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillRepeatedly(Return(CURRENT_PRICE));

    EXPECT_FALSE(tradingSystem.buyNiceTiming(STOCK_ID, STOCK_MAX_PRICE));
}

TEST_F(TradingSystemFixture, BuyNiceTimingFailWithNotEnoughPrice)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillRepeatedly(Return(CURRENT_PRICE));

    EXPECT_FALSE(tradingSystem.buyNiceTiming(STOCK_ID, MAX_CASH));
}

TEST_F(TradingSystemFixture, BuyNiceTimingWithSuccess)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(CURRENT_PRICE))
        .WillOnce(Return(RISING_PRICE))
        .WillOnce(Return(DOUBLE_RISING_PRICE));

    EXPECT_CALL(mockDriver, buy(STOCK_ID, _, _))
		.Times(AtLeast(1));

    bool isSuccess = tradingSystem.buyNiceTiming(STOCK_ID, MAX_CASH);
    EXPECT_TRUE(isSuccess);
}

TEST_F(TradingSystemFixture, SellNiceTimingWithFail)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(AtLeast(2))
        .WillOnce(Return(CURRENT_PRICE))
        .WillRepeatedly(Return(RISING_PRICE));

    EXPECT_FALSE(tradingSystem.sellNiceTiming(STOCK_ID, STOCK_NUM_SELLING_COUNT));
}

TEST_F(TradingSystemFixture, SellNiceTimingWithSuccess)
{
    tradingSystem.selectStockBroker(&mockDriver);

    EXPECT_CALL(mockDriver, getPrice(STOCK_ID, _))
        .Times(3)
        .WillOnce(Return(CURRENT_PRICE))
        .WillOnce(Return(FALLING_PRICE))
        .WillOnce(Return(DOUBLE_FALLING_PRICE));

    EXPECT_TRUE(tradingSystem.sellNiceTiming(STOCK_ID, STOCK_NUM_SELLING_COUNT));
}

TEST_F(TradingSystemFixture, BuyNiceTimingExceptionWithoutSelectBroker)
{
    EXPECT_THROW(tradingSystem.buyNiceTiming(STOCK_ID, STOCK_MAX_PRICE), std::exception);
}

TEST_F(TradingSystemFixture, SellNiceTimingExceptionWithoutSelectBroker)
{
    int count = 100;
    EXPECT_THROW(tradingSystem.sellNiceTiming(STOCK_ID, count), std::exception);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
