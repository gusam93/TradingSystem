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
		return api.login(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return api.sell(stockCode, count, price);
	}
	void buy(string stockCode, int price, int count) override {
		api.buy(stockCode, count, price);
    }
	int getPrice(string stockCode, int msDelay) {
		Sleep(msDelay);
		return api.currentPrice(stockCode);
	}
private:
	KiwerAPI api;
};

class NemoDriver : public StockBrockerDriver {
public:
	void login(string id, string password) override {
		return api.certification(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return api.sellingStock(stockCode, price, count);
	}
	void buy(string stockCode, int price, int count) override {
		api.purchasingStock(stockCode, price, count);
    }
	int getPrice(string stockCode, int msDelay) {
		return api.getMarketPrice(stockCode, msDelay);
	}
private:
	NemoAPI api;
};

class AutoTradingSystem {
public:
	void selectStockBroker(StockBrockerDriver *driver) {
		brocker = driver;
	}

	StockBrockerDriver* getStockBroker() {
		return brocker;
	}

private:
	StockBrockerDriver* brocker;
	
};