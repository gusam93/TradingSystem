#include <iostream>
#include <string>
#include "kiwer.cpp"
#include "nemo.cpp"

using std::string;

class StockBrockerDriver {
public:
	virtual void login(string id, string password) = 0;
};

class kiwerDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return kiwerapi->login(id, password);
	}
private:
	KiwerAPI* kiwerapi;
};

class nemoDriver : public StockBrockerDriver {
	void login(string id, string password) override {
		return nemoapi->certification(id, password);
	}
private:
	NemoAPI* nemoapi;
};