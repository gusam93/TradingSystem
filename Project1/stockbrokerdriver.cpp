#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class AutoTradingSystem {
public:
	void selectBrocker(StockBrockerDriver *driver) {
		brocker = driver;
	}
	StockBrockerDriver* getStockBroker() {
		return brocker;
	}
private:
	StockBrockerDriver * brocker;
};

class StockBrockerDriver {
	virtual void login(string id, string password) = 0;
};

class KiwerDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}
private:
	KiwerAPI* kiwerapi;
};

class NemoDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}
private:
	NemoAPI* nemoapi;
};