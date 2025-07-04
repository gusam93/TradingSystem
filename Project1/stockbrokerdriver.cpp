#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
	virtual void sell(string stockCode, int price, int count) = 0;
	virtual void buy(string stockCode, int price, int count) = 0;
	virtual int getPrice(string stockCode, int msDelay) = 0;
};

class KiwerDriver : public StockBrockerDriver {
public:
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return kiwerapi->sell(stockCode, count, price);
	}
	void buy(string stockCode, int price, int count) override {
		kiwerapi->buy(stockCode, count, price);
    }
	int getPrice(string stockCode, int msDelay) {
		Sleep(msDelay);
		return kiwerapi->currentPrice(stockCode);
	}
private:
	KiwerAPI* kiwerapi;
};

class NemoDriver : public StockBrockerDriver {
public:
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return nemoapi->sellingStock(stockCode, price, count);
	}
	void buy(string stockCode, int price, int count) override {
		nemoapi->purchasingStock(stockCode, price, count);
    }
	int getPrice(string stockCode, int msDelay) {
		return nemoapi->getMarketPrice(stockCode, msDelay);
	}
private:
	NemoAPI* nemoapi;
};

class AutoTradingSystem {
public:
	void selectStockBroker(StockBrockerDriver *driver) {
		brocker = driver;
	}

	StockBrockerDriver* getStockBroker() {
		return brocker;
	}

	bool buyNiceTiming(string stockCode, int maxPrice) {
		int previousPrice = 0;
		int currentStockPrice = 0;

		for (int getPriceCount = 0 ; getPriceCount < GET_PRICE_COUNT; getPriceCount++) {
			currentStockPrice = brocker->getPrice(stockCode, GET_PRICE_DELAY);
			if (previousPrice >= currentStockPrice) {
				return false;
			}
			previousPrice = currentStockPrice;
		}

		int buyStockCount = maxPrice / currentStockPrice;
		if (buyStockCount > 0) {
			brocker->buy(stockCode, currentStockPrice, buyStockCount);
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	const int GET_PRICE_COUNT = 3;
	const int GET_PRICE_DELAY = 200;
	StockBrockerDriver* brocker;
	
};