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

TEST(TradingSystem, Login)
{
    NiceMock<MockDriver> mockDriver;
    EXPECT_CALL(mockDriver, login("ABC", "1234"))
        .Times(1);

    string id = "ABC";
    string password = "1234";   
    mockDriver.login(id, password);
}

int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
