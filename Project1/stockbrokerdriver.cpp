#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrokerDriver {
public:
	virtual void login(string id, string password) = 0;
	virtual void sell(string stockCode, int price, int count) = 0;
	virtual void buy(string stockCode, int price, int count) = 0;
	virtual int getPrice(string stockCode, int msDelay) = 0;
};

class KiwerDriver : public StockBrokerDriver {
public:
	void login(string id, string password) override {
		return api.login(id, password);
	}
	void sell(string stockCode, int price, int count) override {
		return api.sell(stockCode, count, price);
	}
	void buy(string stockCode, int price, int count) override {
		return api.buy(stockCode, count, price);
    }
	int getPrice(string stockCode, int msDelay) override {
		Sleep(msDelay);
		return api.currentPrice(stockCode);
	}
private:
	KiwerAPI api;
};

class NemoDriver : public StockBrokerDriver {
public:
	void login(string id, string password) override {
		return api.certification(id, password);
	}
	void sell(string stockCode, int price, int count) override {
		return api.sellingStock(stockCode, price, count);
	}
	void buy(string stockCode, int price, int count) override {
		return api.purchasingStock(stockCode, price, count);
    }
	int getPrice(string stockCode, int msDelay) override {
		return api.getMarketPrice(stockCode, msDelay);
	}
private:
	NemoAPI api;
};

class AutoTradingSystem {
public:
	void selectStockBroker(StockBrokerDriver *driver) {
		broker = driver;
	}

	StockBrokerDriver* getStockBroker() {
		return broker;
	}

	bool buyNiceTiming(string stockCode, int maxPrice) {
		int finalPrice = checkPriceIncreasing(stockCode);
		if (finalPrice == BUY_PRICE_NOT_NICE) return false;
		int buyStockCount = maxPrice / finalPrice;
		if (buyStockCount <= 0) return false;
		broker->buy(stockCode, finalPrice, buyStockCount);
		return true;
	}

	bool sellNiceTiming(string stockCode, int count) {
		int finalPrice = checkPriceFalling(stockCode);
		if (finalPrice == SELL_PRICE_NOT_NICE) return false;
		broker->sell(stockCode, finalPrice, count);
		return true;
	}

private:
	int checkPriceFalling(string stockCode) {
		int price = broker->getPrice(stockCode, GET_PRICE_DELAY);
		for (int getPriceCount = 0; getPriceCount < GET_PRICE_COUNT - 1; getPriceCount++) {
			int newPrice = broker->getPrice(stockCode, GET_PRICE_DELAY);
			if (price <= newPrice) return SELL_PRICE_NOT_NICE;
			price = newPrice;
		}

		return price;
	}

	int checkPriceIncreasing(string stockCode) {
		int price = broker->getPrice(stockCode, GET_PRICE_DELAY);
		for (int getPriceCount = 0; getPriceCount < GET_PRICE_COUNT - 1; getPriceCount++) {
			int newPrice = broker->getPrice(stockCode, GET_PRICE_DELAY);
			if (price >= newPrice) return BUY_PRICE_NOT_NICE;
			price = newPrice;
		}
		if (price <= 0) return BUY_PRICE_NOT_NICE;
		return price;
	}

	const int GET_PRICE_DELAY = 200;
	const int GET_PRICE_COUNT = 3;
	const int BUY_PRICE_NOT_NICE = -1;
	const int SELL_PRICE_NOT_NICE = -1;
	
	StockBrokerDriver* broker;
};