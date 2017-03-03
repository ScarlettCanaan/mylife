#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <fstream>

enum Direct
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

class point {
public:
	int x, y;
	point(int _x, int _y) : x(_x), y(_y) {};
	point() : x(0), y(0) {};
};

//正方形的结构信息
class blockInfo {
public:
	int id;
	int size;
	point leftup;
	point rightdown;
	point central;
};

class Solution2 {
public:
	std::vector<blockInfo> insert(std::vector<std::vector<int>>& src_map,
		size_t						 targX,
		size_t						 targY,
		const std::vector<blockInfo>&	 old_info) {

		// 边界条件
		if (targX < 0 || targX >= src_map[0].size() ||
			targY < 0 || targY >= src_map.size())
			return old_info;
		if (src_map.empty() || src_map[0].empty())
			return old_info;
		// 若插入点已有标记，则结构不变
		if (src_map[targY][targX] > 0)
			return old_info;
		// 标记插入点
		src_map[targY][targX] = 1;
		std::vector<blockInfo> new_info;
		// 有core并且core没有成长到4*4的情况，优先构造core四周的结构
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1 && old_info[i].size < 4) {
				new_info.push_back(calcCoreStructure(src_map, old_info[i]));
				break;
			}
		}
		// 定义动态规划的状态
		std::vector<std::vector<int>> state(src_map.size(), std::vector<int>(src_map[0].size()));
		// 初始化最小状态
		for (size_t i = 0; i < state.size(); ++i) {
			for (size_t j = 0; j < state[0].size(); ++j) {
				if (src_map[i][j] > 0) {
					state[i][j] = 1;
					src_map[i][j] = 1;
				}
				else
					state[i][j] = 0;
			}
		}

		// 递归求每次结果的最大值
		return dfsHelper(src_map, state, old_info, new_info);
	};

	std::vector<blockInfo> cast(std::vector<std::vector<int>>& src_map,
		Direct direct90,
		Direct direct45,
		const std::vector<blockInfo>& old_info) {
		if (src_map.size() <= 1 && src_map[0].size() <= 1) {
			return old_info;
		}
		point throwpoint;
		bool canThrow = false;
		// 根据输入方向搜索能投出去的点
		switch (direct90) {
		case UP:
			switch (direct45) {
			case LEFT:
				canThrow = scanUPLEFT(src_map, throwpoint, old_info);
				break;
			case RIGHT:
				canThrow = scanUPRIGHT(src_map, throwpoint, old_info);
				break;
			}
			break;
		case DOWN:
			switch (direct45) {
			case LEFT:
				canThrow = scanDOWNLEFT(src_map, throwpoint, old_info);
				break;
			case RIGHT:
				canThrow = scanDOWNRIGHT(src_map, throwpoint, old_info);
				break;
			}
			break;
		case LEFT:
			switch (direct45) {
			case UP:
				canThrow = scanLEFTUP(src_map, throwpoint, old_info);
				break;
			case DOWN:
				canThrow = scanLEFTDOWN(src_map, throwpoint, old_info);
				break;
			}
			break;
		case RIGHT:
			switch (direct45) {
			case UP:
				canThrow = scanRIGHTUP(src_map, throwpoint, old_info);
				break;
			case DOWN:
				canThrow = scanRIGHTDOWN(src_map, throwpoint, old_info);
				break;
			}
			break;
		default:
			break;
		}
		if (!canThrow)
			return old_info;
		src_map[throwpoint.y][throwpoint.x] = 0;
		std::vector<blockInfo> new_info;
		// 有core并且core没有成长到4*4的情况，优先构造core四周的结构
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1 && old_info[i].size < 4) {
				new_info.push_back(calcCoreStructure(src_map, old_info[i]));
				break;
			}
		}
		// 定义动态规划的状态
		std::vector<std::vector<int>> state(src_map.size(), std::vector<int>(src_map[0].size()));
		// 初始化最小状态
		for (size_t i = 0; i < state.size(); ++i) {
			for (size_t j = 0; j < state[0].size(); ++j) {
				if (src_map[i][j] > 0) {
					state[i][j] = 1;
					src_map[i][j] = 1;
				}
				else
					state[i][j] = 0;
			}
		}

		// 递归求每次结果的最大值
		return dfsHelper(src_map, state, old_info, new_info);

	};

private:
	blockInfo calcCoreStructure(std::vector<std::vector<int>>& src_map, blockInfo coreInfo) {
		// 在以core为中心的最大7x7的矩阵内搜索包含core方块的最大矩形
		// 定义动态规划的状态
		std::vector<std::vector<int>> state(std::min(7, (int)src_map.size()), std::vector<int>(std::min(7, (int)src_map[0].size())));
		// 初始化最小状态
		int offsetX = std::max(0, coreInfo.central.x - 3);
		int offsetY = std::max(0, coreInfo.central.y - 3);
		for (size_t i = 0; i < state.size(); ++i) {
			for (size_t j = 0; j < state[0].size(); ++j) {
				// core已形成的原结构标记为 -1
				if (src_map[offsetY + i][offsetX + j] > 0 || src_map[offsetY + i][offsetX + j] == -1) {
					state[i][j] = 1;
					src_map[offsetY + i][offsetX + j] = 1;
				}

				else
					state[i][j] = 0;
			}
		}
		// 状态转移方程
		int max_size = 0;
		point rigion_leftup;
		point rigion_rightdown;
		for (size_t i = 1; i < state.size(); ++i) {
			for (size_t j = 1; j < state[0].size(); ++j) {
				if (state[i][j] > 0) {
					state[i][j] = std::min(std::min(state[i - 1][j - 1], state[i][j - 1]), state[i - 1][j]) + 1;
					if (/*state[i][j] >= coreInfo.size && */state[i][j] <= 4 && state[i][j] > max_size) {
						rigion_leftup.x = offsetX + j - state[i][j] + 1;
						rigion_leftup.y = offsetY + i - state[i][j] + 1;
						rigion_rightdown.x = offsetX + j;
						rigion_rightdown.y = offsetY + i;
						if (coreInfo.central.x >= rigion_leftup.x && coreInfo.central.x <= rigion_rightdown.x &&
							coreInfo.central.y >= rigion_leftup.y && coreInfo.central.y <= rigion_rightdown.y)
							max_size = state[i][j];
					}
				}
			}
		}
		// core点必定为-1
		src_map[coreInfo.central.y][coreInfo.central.x] = -1;
		if (max_size == 0)
			return coreInfo;
		else {
			for (int i = rigion_leftup.y; i <= rigion_rightdown.y; i++)
				for (int j = rigion_leftup.x; j <= rigion_rightdown.x; j++)
					src_map[i][j] = -1;
			coreInfo.leftup = rigion_leftup;
			coreInfo.rightdown = rigion_rightdown;
			coreInfo.size = max_size;
			return coreInfo;
		}
	}
	std::vector<blockInfo> dfsHelper(std::vector<std::vector<int>>&		src_map,
		std::vector<std::vector<int>>&		state,
		const std::vector<blockInfo>&		old_info,
		std::vector<blockInfo>&			new_info) {
		// 状态转移方程
		int max_size = 0;
		std::vector<point> Q;
		for (size_t i = 1; i < state.size(); ++i) {
			for (size_t j = 1; j < state[0].size(); ++j) {
				if (state[i][j] > 0) {
					state[i][j] = std::min(std::min(state[i - 1][j - 1], state[i][j - 1]), state[i - 1][j]) + 1;
					if (state[i][j] > max_size) {
						Q.clear();
						max_size = state[i][j];
						Q.push_back(point(j, i));
					}
					// 找到多个相同大小的结果，优先构造原有的正方形，再按右上->左下的方式生成
					else if (state[i][j] == max_size && state[i][j] > 1) {
						for (size_t iter = 0; iter < old_info.size(); iter++) {
							if (old_info[iter].rightdown.x == j &&
								old_info[iter].rightdown.y == i &&
								old_info[iter].size == max_size) {
								Q.clear();
								Q.push_back(point(j, i));
							}
						}
					}
				}
			}
		}
		if (max_size <= 1)
			return new_info;
		for (size_t i = 0; i < Q.size(); ++i) {
			// 遍历正方形的每个像素点
			size_t offsetX = Q[i].x - max_size + 1;
			size_t offsetY = Q[i].y - max_size + 1;
			for (int y = offsetY; y <= Q[0].y; ++y) {
				for (int x = offsetX; x <= Q[0].x; ++x) {
					state[y][x] = 0;
					// 让当前的正方形内部填充自己的大小为值
					src_map[y][x] = max_size;
				}
			}
		}

		blockInfo info;
		info.id = getmaxid(new_info) + 1;
		info.size = max_size;
		info.leftup = point(Q[0].x - max_size + 1, Q[0].y - max_size + 1);
		info.rightdown = point(Q[0].x, Q[0].y);
		info.central = point(info.rightdown.x - (max_size / 2), info.rightdown.y - (max_size / 2));
		new_info.push_back(info);

		return dfsHelper(src_map, state, old_info, new_info);
	};

	// 八个扫描方向
	bool scanUPLEFT(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	};

	bool scanUPRIGHT(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int i = 0; i < height; i++) {
			for (int j = width - 1; j >= 0; j--) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	bool scanDOWNLEFT(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int i = height - 1; i >= 0; i--) {
			for (int j = 0; j < width; j++) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	bool scanDOWNRIGHT(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int i = height - 1; i >= 0; i--) {
			for (int j = width - 1; j >= 0; j--) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 && 
					coreInfo.size < 4 && 
					coreInfo.size > 1 && 
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	bool scanLEFTUP(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int j = 0; j < width - 1; j++) {
			for (int i = height - 1; i >= 0; i--) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	};

	bool scanLEFTDOWN(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int j = 0; j < width - 1; j++) {
			for (int i = 0; i < height - 1; i++) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	bool scanRIGHTUP(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int j = width - 1; j >= 0; j--) {
			for (int i = height - 1; i >= 0; i--) {
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	bool scanRIGHTDOWN(const std::vector<std::vector<int>>& src_map, point& p, const std::vector<blockInfo>& old_info) {
		blockInfo coreInfo;
		coreInfo.size = 0;
		for (size_t i = 0; i < old_info.size(); i++) {
			if (old_info[i].id == -1) {
				coreInfo = old_info[i];
				break;
			}
		}
		int height = src_map.size();
		int width = src_map[0].size();
		for (int j = width - 1; j >= 0; j--) {
			for (int i = 0; i < height - 1; i++) {
				if (src_map[i][j] == -1 &&
					coreInfo.size < 4 &&
					coreInfo.size > 1 &&
					(coreInfo.central.x != j || coreInfo.central.y != i)) {
					p.x = j;
					p.y = i;
					return true;
				}
				if (src_map[i][j] > 0) {
					p.x = j;
					p.y = i;
					return true;
				}
			}
		}
		return false;
	}

	int getmaxid(const std::vector<blockInfo>& info) {
		int max = 0;
		for (size_t i = 0; i < info.size(); i++) {
			if (info[i].id > max)
				max = info[i].id;
		}
		return max;
	};

};

std::vector<blockInfo> getBlockorInfo(std::vector<std::vector<int>> src_map) {
	std::vector<blockInfo> retVal;
	int maxId = 1;
	for (size_t y = 0; y < src_map.size(); ++y) {
		for (size_t x = 0; x < src_map[0].size(); ++x) {
			if (src_map[y][x] > 1) {
				blockInfo info;
				info.id = maxId++;
				info.leftup = point(x, y);
				info.size = src_map[y][x];
				info.rightdown = point(x + info.size - 1, y + info.size - 1);
				for (int i = y; i <= info.rightdown.y; ++i) {
					for (int j = x; j <= info.rightdown.x; ++j) {
						src_map[i][j] = 0;
					}
				}
				retVal.push_back(info);
			}
			else if (src_map[y][x] == -1) {
				blockInfo info;
				info.id = -1;
				info.leftup = point(x, y);
				int size = 0;
				for (int i = y; i < src_map.size() && src_map[i][x] == -1; ++i) {
					for (int j = x; j < src_map[0].size() && src_map[i][j] == -1; ++j) {
						src_map[i][j] = 0;
						++size;
					}
					info.size = size;
					size = 0;
				}
				info.rightdown = point(x + info.size - 1, y + info.size - 1);
				info.central = point(info.leftup.x + info.size / 2, info.leftup.y + info.size / 2);
				retVal.push_back(info);
			}
		}
	}
	return retVal;
}

void print(std::vector<std::vector<int>>& v, std::vector<blockInfo> info) {
	if (v.empty() || v[0].empty())
		return;

	for (size_t i = 0; i < v.size(); ++i) {
		for (size_t j = 0; j < v[0].size(); ++j) {
			std::cout << v[i][j] << " ";
		}
		std::cout << std::endl;
	}
	for (size_t i = 0; i < info.size(); i++) {
		std::cout << "id:" << info[i].id << " size:" << info[i].size <<
			" leftup:(" << info[i].leftup.x << ", " << info[i].leftup.y <<
			") rightdown:(" << info[i].rightdown.x << ", " << info[i].rightdown.y << ")" << std::endl;
	}
}

void printToFile(std::vector<std::vector<int>>& v, std::vector<blockInfo> info, std::ofstream& outlog) {
	if (v.empty() || v[0].empty())
		return;

	for (size_t i = 0; i < v.size(); ++i) {
		for (size_t j = 0; j < v[0].size(); ++j) {
			outlog << v[i][j] << " ";
		}
		outlog << std::endl;
	}
	for (size_t i = 0; i < info.size(); i++) {
		outlog << "id:" << info[i].id << " size:" << info[i].size <<
			" leftup:(" << info[i].leftup.x << ", " << info[i].leftup.y <<
			") rightdown:(" << info[i].rightdown.x << ", " << info[i].rightdown.y << ")" << std::endl;
	}
}
// UP 0 DOWN 1 LEFT 2 RIGHT 3

void printDir(int dir90, int dir45) {
	switch (dir90)
	{
	case 0:
		switch (dir45)
		{
		case 2:
			std::cout << "上左" << std::endl;
			break;
		case 3:
			std::cout << "上右" << std::endl;
			break;
		}
		break;
	case 1:
		switch (dir45)
		{
		case 2:
			std::cout << "下左" << std::endl;
			break;
		case 3:
			std::cout << "下右" << std::endl;
			break;
		}
		break;
	case 2:
		switch (dir45)
		{
		case 0:
			std::cout << "左上" << std::endl;
			break;
		case 1:
			std::cout << "左下" << std::endl;
			break;
		}
		break;
	case 3:
		switch (dir45)
		{
		case 0:
			std::cout << "右上" << std::endl;
			break;
		case 1:
			std::cout << "右下" << std::endl;
			break;
		}
		break;
	}
}

// UP 0 DOWN 1 LEFT 2 RIGHT 3

void printDirToFile(int dir90, int dir45, std::ofstream& outlog) {
	switch (dir90)
	{
	case 0:
		switch (dir45)
		{
		case 2:
			outlog << "上左" << std::endl;
			break;
		case 3:
			outlog << "上右" << std::endl;
			break;
		}
		break;
	case 1:
		switch (dir45)
		{
		case 2:
			outlog << "下左" << std::endl;
			break;
		case 3:
			outlog << "下右" << std::endl;
			break;
		}
		break;
	case 2:
		switch (dir45)
		{
		case 0:
			outlog << "左上" << std::endl;
			break;
		case 1:
			outlog << "左下" << std::endl;
			break;
		}
		break;
	case 3:
		switch (dir45)
		{
		case 0:
			outlog << "右上" << std::endl;
			break;
		case 1:
			outlog << "右下" << std::endl;
			break;
		}
		break;
	}
}

int main(int argc, char const *argv[])
{
	Solution2 solution;
	std::ofstream outlog("testcast.log");
	if (!outlog.is_open())
		return 0;
	//parsing input token stream to structure.
	int N;
	std::cin >> N;
	std::cout << "插入点测试:" << std::endl;
	outlog << "插入点测试" << std::endl;
	while (N--) {
		int m, n;
		std::cin >> m >> n;
		std::vector<std::vector<int>> _map(n, std::vector<int>(m));
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				std::cin >> _map[y][x];
			}
		}
		int insX, insY;
		std::cin >> insX >> insY;
		std::vector<std::vector<int>> retVal(_map);
		std::vector<blockInfo> initInfo = getBlockorInfo(_map);
		std::cout << "input:" << std::endl;
		outlog << "input:" << std::endl;
		print(_map, initInfo);
		std::cout << "(" << insX << ", " << insY << ")" << std::endl;
		printToFile(_map, initInfo, outlog);
		outlog << "(" << insX << ", " << insY << ")" << std::endl;

		//solution
		std::vector<blockInfo> updateInfo = solution.insert(retVal, insX, insY, initInfo);
		//output
		std::cout << "output:" << std::endl;
		outlog << "output:" << std::endl;
		print(retVal, updateInfo);
		printToFile(retVal, updateInfo, outlog);
		std::cout << std::endl;
		outlog << std::endl;
	}

	std::cout << "投出点测试" << std::endl;
	outlog << "投出点测试" << std::endl;
	std::cin >> N;
	while (N--) {
		int m, n;
		std::cin >> m >> n;
		std::vector<std::vector<int>> _map(n, std::vector<int>(m));
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				std::cin >> _map[y][x];
			}
		}
		int dir90, dir45;
		std::cin >> dir90 >> dir45;
		std::vector<std::vector<int>> retVal(_map);
		std::vector<blockInfo> initInfo = getBlockorInfo(_map);
		std::cout << "input:" << std::endl;
		print(_map, initInfo);
		printDir(dir90, dir45);
		outlog << "input:" << std::endl;
		printToFile(_map, initInfo, outlog);
		printDirToFile(dir90, dir45, outlog);

		//solution
		std::vector<blockInfo> updateInfo = solution.cast(retVal, (Direct)dir90, (Direct)dir45, initInfo);
		//output
		std::cout << "output:" << std::endl;
		print(retVal, updateInfo);
		std::cout << std::endl;
		outlog << "output:" << std::endl;
		printToFile(retVal, updateInfo, outlog);
		outlog << std::endl;
	}
	outlog.close();
	system("PAUSE");
	return 0;
}
