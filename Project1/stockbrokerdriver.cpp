#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
	virtual void sell(string stockCode, int price, int count) = 0;
	virtual int getPrice(string stockCode, int msDelay) = 0;
};

class KiwerDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return kiwerapi->sell(stockCode, count, price);
	}
	int getPrice(string stockCode, int msDelay) {
		return kiwerapi->currentPrice(stockCode);
	}
private:
	KiwerAPI* kiwerapi;
};

class NemoDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}
	void sell(string stockCode, int price, int count) {
		return nemoapi->sellingStock(stockCode, price, count);
	}
	int getPrice(string stockCode, int msDelay) {
		return nemoapi->getMarketPrice(stockCode, msDelay);
	}
private:
	NemoAPI* nemoapi;
};