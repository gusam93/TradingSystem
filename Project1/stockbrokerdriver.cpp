#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
	virtual int getPrice(string& stockCode, int msDelay) = 0;
};

class kiwerDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return api->login(id, password);
	}
	int getPrice(string& stockCode, int msDelay) override {
		return api->currentPrice(stockCode);
	}
private:
	KiwerAPI* api;
};

class nemoDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return api->certification(id, password);
	}
	int getPrice(string& stockCode, int msDelay) override {
		return api->getMarketPrice(stockCode, msDelay);
	}
private:
	NemoAPI* api;
};
