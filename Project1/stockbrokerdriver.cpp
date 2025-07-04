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

	bool buyNiceTiming(string stockCode, int maxCash) {
		if (isBrockerNull()) throw std::exception("Stock broker not selected");
		if (isNiceTimingToBuy(stockCode) == false) return false;
		if (isInvalidPrice()) return false;

		int buyStockCount = getStockCount(maxCash, currentPrice);
		broker->buy(stockCode, currentPrice, buyStockCount);
		return true;
	}

	bool sellNiceTiming(string stockCode, int count) {
		if (isBrockerNull()) throw std::exception("Stock broker not selected");
		if (isNiceTimingToSell(stockCode) == false) return false;
		if (isInvalidPrice()) return false;

		broker->sell(stockCode, currentPrice, count);
		return true;
	}

private:
	bool isBrockerNull()
	{
		return broker == nullptr;
	}

	bool isInvalidPrice()
	{
		return currentPrice == 0;
	}

	int getStockCount(int& maxCash, int& currentPrice)
	{
		return maxCash / currentPrice;
	}


	int isNiceTimingToSell(string stockCode) {
		int price = broker->getPrice(stockCode, 0);
		for (int getPriceCount = 0; getPriceCount < GET_PRICE_COUNT - 1; getPriceCount++) {
			int newPrice = broker->getPrice(stockCode, GET_PRICE_DELAY);
			if (price <= newPrice) return false;
			price = newPrice;
		}
		currentPrice = price;
		return price;
	}

	bool isNiceTimingToBuy(string stockCode) {
		int price = broker->getPrice(stockCode, 0);
		for (int getPriceCount = 0; getPriceCount < GET_PRICE_COUNT - 1; getPriceCount++) {
			int newPrice = broker->getPrice(stockCode, GET_PRICE_DELAY);
			if (price >= newPrice) return false;
			price = newPrice;
		}
		if (price <= 0) return false;
		currentPrice = price;
		return true;
	}

	const int GET_PRICE_DELAY = 200;
	const int GET_PRICE_COUNT = 3;

	int currentPrice = 0;
	
	StockBrokerDriver* broker;
};