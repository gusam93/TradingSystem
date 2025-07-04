#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
};

class KiwerDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}
	void sell(std::string stockCode, int price, int count) {
		return kiwerapi->sell(stockCode, count, price);
	}
private:
	KiwerAPI* kiwerapi;
};

class NemoDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}
	void sell(std::string stockCode, int price, int count) {
		return nemoapi->sellingStock(stockCode, price, count);
	}
private:
	NemoAPI* nemoapi;
};