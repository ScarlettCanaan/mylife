#include "MergeLogic.h"
#define _USE_MATH_DEFINES
#include <math.h>

MergeLogic::MergeLogic()
{
}


MergeLogic::~MergeLogic()
{
}

bool MergeLogic::merge(const int8_t* binaryInfo, size_t x, size_t y, 
	const std::vector<BlockInfo>& blockInfoList, 
	std::vector<BlockInfo>* retBlockList) {

	std::vector<std::vector<int8_t>> map_info(y, std::vector<int8_t>(x));
	if (!copyBits2Vector(binaryInfo, x, y, &map_info))
		return false;

	return calcStructure(map_info, blockInfoList, retBlockList);
}

bool MergeLogic::breakUp(const std::vector<BlockInfo>& blockInfoList,
	int takenCount,
	float direction,
	int8_t* binaryInfo, size_t x, size_t y,
	std::vector<BlockInfo>* retBlockList,
	std::vector<Point>* retTakenPoints) {

	std::vector<std::vector<int8_t>> map_info(y, std::vector<int8_t>(x));
	Direct direct90 = NONE, direct45 = NONE;

	if (!copyBits2Vector(binaryInfo, x, y, &map_info))
		return false;
	if (!radian2direct(direction, direct90, direct45))
		return false;
	int realTakenCount = getTakenPoints(map_info, 
										blockInfoList, 
										direct90, 
										direct45, 
										takenCount, 
										retTakenPoints);
	if (realTakenCount < takenCount)
		return false;
	for (int i = 0; i < realTakenCount; ++i) {
		int offsetx = ((*retTakenPoints)[i]).x;
		int offsety = ((*retTakenPoints)[i]).y;
		binaryInfo[x * offsety + offsetx] = 0;
		map_info[offsety][offsetx] = 0;
	}
	
	return calcStructure(map_info, blockInfoList, retBlockList);
}

bool MergeLogic::copyBits2Vector(const int8_t* binaryInfo, size_t x, size_t y,
	std::vector<std::vector<int8_t>>* retVec) {
	size_t colom_size = retVec->size(), row_size = ((*retVec)[0]).size();
	if (y != colom_size || x != row_size)
		return false;

	const int8_t *dummy = binaryInfo;
	std::for_each(retVec->begin(), retVec->end(), [&](std::vector<int8_t>& _row) {
		std::for_each(_row.begin(), _row.end(), [&](int8_t& _grid) { 
			_grid = *dummy;
			dummy++; });
	});
	return true;
}

bool MergeLogic::radian2direct(float direction, Direct& direct90, Direct& direct45) {

	if (direction > 0 && direction <= M_PI_4) {
		direct90 = RIGHT;
		direct45 = DOWN;
	}
	else if (direction > M_PI_4 && direction <= M_PI_2) {
		direct90 = UP;
		direct45 = RIGHT;
	}
	else if (direction > M_PI_2 && direction <= M_PI_2 + M_PI_4) {
		direct90 = UP;
		direct45 = LEFT;
	}
	else if (direction > M_PI_2 + M_PI_4 && direction <= M_PI) {
		direct90 = LEFT;
		direct45 = DOWN;
	}
	else if (direction > -M_PI && direction <= -(M_PI_2 + M_PI_4)) {
		direct90 = LEFT;
		direct45 = UP;
	}
	else if (direction > -(M_PI_2 + M_PI_4) && direction <= -M_PI_2) {
		direct90 = DOWN;
		direct45 = LEFT;
	}
	else if (direction > -M_PI_2 && direction <= -M_PI_4) {
		direct90 = DOWN;
		direct45 = RIGHT;
	}
	else if (direction > -M_PI_4 && direction <= 0) {
		direct90 = RIGHT;
		direct45 = UP;
	}
	else {
		return false;
	}
	
	return true;
}

int MergeLogic::getTakenPoints(std::vector<std::vector<int8_t>>& src_map,
	const std::vector<BlockInfo>& blockInfoList,
	Direct direct90, Direct direct45,
	int takenCount,
	std::vector<Point>* retTakenPoints) {
	int realTakenCount = 0;
	switch (direct90) {
	case UP:
		switch (direct45) {
		case LEFT:
			realTakenCount = scanUPLEFT(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		case RIGHT:
			realTakenCount = scanUPRIGHT(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		}
		break;
	case DOWN:
		switch (direct45) {
		case LEFT:
			realTakenCount = scanDOWNLEFT(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		case RIGHT:
			realTakenCount = scanDOWNRIGHT(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		}
		break;
	case LEFT:
		switch (direct45) {
		case UP:
			realTakenCount = scanLEFTUP(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		case DOWN:
			realTakenCount = scanLEFTDOWN(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		}
		break;
	case RIGHT:
		switch (direct45) {
		case UP:
			realTakenCount = scanRIGHTUP(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		case DOWN:
			realTakenCount = scanRIGHTDOWN(src_map, retTakenPoints, blockInfoList, takenCount);
			break;
		}
		break;
	default:
		break;
	}
	return realTakenCount;
}

bool MergeLogic::calcStructure(std::vector<std::vector<int8_t>>& src_map,
	const std::vector<BlockInfo>& old_info,
	std::vector<BlockInfo>* retBlockList) {
	// 有core并且core没有成长到4*4的情况，优先构造core四周的结构
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart == true) {
			if (old_info[i].size < 4) {
				retBlockList->push_back(calcCoreStructure(src_map, old_info[i]));
				break;
			}
			else {
				retBlockList->push_back(old_info[i]);
				break;
			}
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
	return dfsHelper(src_map, state, old_info, retBlockList);
}

BlockInfo MergeLogic::calcCoreStructure(std::vector<std::vector<int8_t>>& src_map,
	const BlockInfo& coreInfo) {
	// 在以core为中心的最大7x7的矩阵内搜索包含core方块的最大矩形
	// 定义动态规划的状态
	std::vector<std::vector<int>> state(std::min(7, (int)src_map.size()),
		std::vector<int>(std::min(7, (int)src_map[0].size())));
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
	size_t max_size = 0;
	Point rigion_leftup;
	Point rigion_rightdown;
	for (size_t i = 1; i < state.size(); ++i) {
		for (size_t j = 1; j < state[0].size(); ++j) {
			if (state[i][j] > 0) {
				state[i][j] = std::min(std::min(state[i - 1][j - 1], state[i][j - 1]), state[i - 1][j]) + 1;
				if (/*state[i][j] >= coreInfo.size && */state[i][j] <= 4 && state[i][j] > max_size) {
					// 该正方形范围是否包含core位置在内
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
	// 如果重计算的红心size和当前的相同，则不做改变
	if (max_size == 0)
		return std::move(coreInfo);
	else if (max_size <= coreInfo.size) {
		for (int i = coreInfo.leftup.y; i <= coreInfo.rightdown.y; ++i) {
			for (int j = coreInfo.leftup.x; j <= coreInfo.rightdown.x; ++j) {
				src_map[i][j] = -1;
			}
		}
		return std::move(coreInfo);
	}
	else 
	{
		for (int i = rigion_leftup.y; i <= rigion_rightdown.y; i++) {
			for (int j = rigion_leftup.x; j <= rigion_rightdown.x; j++) {
				src_map[i][j] = -1;
			}
		}
		BlockInfo newCoreInfo;
		newCoreInfo.isHeart = true;
		newCoreInfo.leftup = rigion_leftup;
		newCoreInfo.rightdown = rigion_rightdown;
		newCoreInfo.size = max_size;
		return std::move(newCoreInfo);
	}
}

bool MergeLogic::dfsHelper(std::vector<std::vector<int8_t>>& src_map,
	std::vector<std::vector<int>>& state,
	const std::vector<BlockInfo>& old_info,
	std::vector<BlockInfo>* new_info) {

	BlockInfo coreInfo;
	for (int i = 0; i < old_info.size(); ++i) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}

	// 状态转移方程
	int max_size = 0;
	std::vector<Point> Q;
	for (size_t i = 0; i < state.size(); ++i) {
		for (size_t j = 0; j < state[0].size(); ++j) {
			if (state[i][j] > 0) {
				if (i > 0 && j > 0)
					state[i][j] = std::min(std::min(state[i - 1][j - 1], state[i][j - 1]), state[i - 1][j]) + 1;
				if (state[i][j] > max_size) {
					Q.clear();
					max_size = state[i][j];
					Q.push_back(Point(j, i));
				}
				// 找到多个相同大小的结果，优先构造原有的正方形，再按右上->左下的方式生成
				else if (state[i][j] == max_size && state[i][j] > 1) {
					// 离红心近的正方形优先生成
					if (!Q.empty()) {
						Point prevCentral(Q[0].x - max_size / 2 + 1, Q[0].y - max_size / 2 + 1);
						Point nextCentral(j - max_size / 2 + 1, i - max_size / 2 + 1);
						if (coreInfo.central.Distance(prevCentral) > coreInfo.central.Distance(nextCentral)) {
							Q.clear();
							Q.push_back(Point(j, i));
						}
					}
					// 上一次结构出现过的正方形优先生成
					for (size_t iter = 0; iter < old_info.size(); iter++) {
						if (old_info[iter].rightdown.x == j &&
							old_info[iter].rightdown.y == i &&
							old_info[iter].size == max_size) {
							Q.clear();
							Q.push_back(Point(j, i));
						}
					}
				}
			}
		}
	}
	if (max_size < 1)
		return true;
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

	BlockInfo info;
	info.isHeart = false;
	info.size = max_size;
	info.leftup = Point(Q[0].x - max_size + 1, Q[0].y - max_size + 1);
	info.rightdown = Point(Q[0].x, Q[0].y);
	info.central = Point(info.rightdown.x - (max_size / 2), info.rightdown.y - (max_size / 2));
	new_info->push_back(info);

	return dfsHelper(src_map, state, old_info, new_info);
}

int MergeLogic::scanUPLEFT(const std::vector<std::vector<int8_t>>& src_map, 
	std::vector<Point>* retTakenPoints, 
	const std::vector<BlockInfo>& old_info, int	takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanUPRIGHT(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int i = 0; i < height; i++) {
		for (int j = width - 1; j >= 0; j--) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanDOWNLEFT(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanDOWNRIGHT(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int i = height - 1; i >= 0; i--) {
		for (int j = width - 1; j >= 0; j--) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanLEFTUP(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int j = 0; j < width - 1; j++) {
		for (int i = height - 1; i >= 0; i--) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanLEFTDOWN(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int j = 0; j < width - 1; j++) {
		for (int i = 0; i < height - 1; i++) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}

int MergeLogic::scanRIGHTUP(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int j = width - 1; j >= 0; j--) {
		for (int i = height - 1; i >= 0; i--) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}
int MergeLogic::scanRIGHTDOWN(const std::vector<std::vector<int8_t>>& src_map,
	std::vector<Point>* retTakenPoints,
	const std::vector<BlockInfo>& old_info, int takenCount) {
	int realTakenCount = 0;
	BlockInfo coreInfo;
	coreInfo.size = 0;
	for (size_t i = 0; i < old_info.size(); i++) {
		if (old_info[i].isHeart) {
			coreInfo = old_info[i];
			break;
		}
	}
	int height = src_map.size();
	int width = src_map[0].size();
	for (int j = width - 1; j >= 0; j--) {
		for (int i = 0; i < height - 1; i++) {
			if (realTakenCount == takenCount)
				return realTakenCount;
			if (src_map[i][j] > 0) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
			if (src_map[i][j] == -1 && coreInfo.size < 4 && coreInfo.size > 1 &&
				(coreInfo.central.x != j || coreInfo.central.y != i)) {
				retTakenPoints->push_back(Point(j, i));
				++realTakenCount;
			}
		}
	}
	return realTakenCount;
}