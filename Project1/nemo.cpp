// 이 소스코드는 네모증권에서 제공되는 소스코드이며, 읽기전용으로 제공됩니다.
// 소스코드를 임의로 변경을 할 수 없습니다.

#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>

class NemoAPI {
public:
	void certification(std::string ID, std::string pass) {
		std::cout << "[NEMO]" << ID << " login GOOD\n";
	}

	void purchasingStock(std::string stockCode, int price, int count) {
		std::cout << "[NEMO]" << stockCode << " buy stock ( price : " << price << " ) * ( count : " << count << ")\n";
	}

	void sellingStock(std::string stockCode, int price, int count) {
		std::cout << "[NEMO]" << stockCode << " sell stock ( price : " << price << " ) * ( count : " << count << ")\n";
	}

	int getMarketPrice(std::string stockCode, int minute) {
		//minute ms초 이후 구매가 되는 방식 (최소 1 ms)
		if (minute <= 0) minute = 1;
		Sleep(minute);
		std::srand(std::time(NULL));
		int ret = std::rand() % 10 * 100 + 5000;
		return ret;
	}
};