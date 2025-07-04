#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
    virtual void buy(string stockCode, int count, int price) {}
};

class KiwerDriver : public StockBrockerDriver {
public:
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}

    void buy(string stockCode, int count, int price) override {
		kiwerapi->buy(stockCode, count, price);
    }
private:
	KiwerAPI* kiwerapi;
};

class NemoDriver : public StockBrockerDriver {
public:
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}

    void buy(string stockCode, int count, int price) override {
		nemoapi->purchasingStock(stockCode, count, price);
    }
private:
	NemoAPI* nemoapi;
};