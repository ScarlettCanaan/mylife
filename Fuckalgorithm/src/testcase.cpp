#include "MergeLogic.h"
#include <string>

void print(int8_t* binaryInfo, size_t x, size_t y) {
	if (binaryInfo == NULL)
		return;
	for (size_t i = 0; i < y; ++i) {
		for (size_t j = 0; j < x; ++j) {
			int temp = binaryInfo[i * x + j];
			std::cout << temp << " ";
		}
		std::cout << std::endl;
	}
}

void printInfo(std::vector<BlockInfo> _info) {
	std::for_each(_info.begin(), _info.end(), [&](BlockInfo _data) {
		std::cout << "isHeart:" << std::to_string(_data.isHeart) << " size:" << _data.size <<
			" leftup:(" << _data.leftup.x << ", " << _data.leftup.y <<
			") rightdown:(" << _data.rightdown.x << ", " << _data.rightdown.y << ")" << std::endl;
	});
}

void printToFile(int8_t* binaryInfo, size_t x, size_t y, std::ofstream& _outlog) {
	if (binaryInfo == NULL)
		return;
	for (size_t i = 0; i < y; ++i) {
		for (size_t j = 0; j < x; ++j) {
			_outlog << binaryInfo[i * y + x] << " ";
		}
		std::cout << std::endl;
	}
}

int main(int argc, char const *argv[])
{
	MergeLogic solution;
	//parsing input token stream to structure.
	int N;
	std::cin >> N;
	std::cout << "插入点测试:" << std::endl;
	while (N--) {
		int m, n;
		std::cin >> m >> n;
		int8_t* binaryInfo = new int8_t[m * n];
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				int temp;
				std::cin >> temp;
				binaryInfo[y * m + x] = temp;
				//binaryInfo[y * m + x] -= '0';
			}
		}
		int insX, insY;
		std::cin >> insX >> insY;
		std::cout << "input:" << std::endl;
		print(binaryInfo, m, n);
		std::vector<BlockInfo>* old_info = new std::vector<BlockInfo>();
		solution.merge(binaryInfo, m, n, std::vector<BlockInfo>(), old_info);
		std::cout << "(" << insX << ", " << insY << ")" << std::endl;
		printInfo(*old_info);

		//solution
		binaryInfo[m * (insY) + insX] = 1;
		std::vector<BlockInfo>* new_info = new std::vector<BlockInfo>();
		solution.merge(binaryInfo, m, n, *old_info, new_info);

		//output
		std::cout << "output:" << std::endl;
		print(binaryInfo, m, n);
		printInfo(*new_info);
		delete binaryInfo;
		delete old_info;
		delete new_info;
	}

	std::cout << "投出点测试" << std::endl;
	std::cin >> N;
	while (N--) {
		int m, n;
		std::cin >> m >> n;
		int8_t* binaryInfo = new int8_t[m * n];
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				int temp;
				std::cin >> temp;
				binaryInfo[y * m + x] = temp;
			}
		}
		int dir90, dir45;
		std::cin >> dir90 >> dir45;
		std::cout << "input:" << std::endl;
		print(binaryInfo, m, n);
		std::vector<BlockInfo>* old_info = new std::vector<BlockInfo>();
		solution.merge(binaryInfo, m, n, std::vector<BlockInfo>(), old_info);
		printInfo(*old_info);
		//solution

		//output
		std::vector<BlockInfo>* new_info = new std::vector<BlockInfo>();
		std::vector<Point>* retTakenPoints = new std::vector<Point>();
		solution.breakUp(*old_info, 1, 0.5, binaryInfo, m, n, new_info, retTakenPoints);
		std::cout << "output:" << std::endl;
		print(binaryInfo, m, n);
		printInfo(*new_info);
	}
	system("PAUSE");
	return 0;
}
