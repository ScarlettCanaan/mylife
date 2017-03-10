#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <fstream>
#include "BlockInfoDef.h"

class MergeLogic
{
public:
	MergeLogic();
	~MergeLogic();

public:
	bool merge(const int8_t* binaryInfo, size_t x, size_t y,
		const std::vector<BlockInfo>& blockInfoList,
		std::vector<BlockInfo>* retBlockList
	);

	bool breakUp(const std::vector<BlockInfo>& blockInfoList,
		int	takenCount,
		float direction,
		int8_t* binaryInfo, size_t x, size_t y,
		std::vector<BlockInfo>* retBlockList,
		std::vector<Point>* retTakenPoints
	);

private:
	bool copyBits2Vector(const int8_t* binaryInfo, size_t x, size_t y,
		std::vector<std::vector<int8_t>>* retVec);

	bool radian2direct(float direction, Direct& direct90, Direct& direct45);

	int getTakenPoints(std::vector<std::vector<int8_t>>& src_map,
		const std::vector<BlockInfo>& blockInfoList,
		Direct direct90, Direct direct45,
		int takenCount,
		std::vector<Point>* retTakenPoints);

	bool calcStructure(std::vector<std::vector<int8_t>>& src_map,
		const std::vector<BlockInfo>& old_info,
		std::vector<BlockInfo>* retBlockList);

	BlockInfo calcCoreStructure(std::vector<std::vector<int8_t>>& src_map,
		const BlockInfo& coreInfo);

	bool dfsHelper(std::vector<std::vector<int8_t>>&	src_map,
		std::vector<std::vector<int>>& state,
		const std::vector<BlockInfo>& old_info,
		std::vector<BlockInfo>* new_info
	);

	int scanUPLEFT(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanLEFTUP(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints, 
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanUPRIGHT(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanRIGHTUP(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanDOWNLEFT(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanLEFTDOWN(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanDOWNRIGHT(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);

	int scanRIGHTDOWN(const std::vector<std::vector<int8_t>>& src_map,
		std::vector<Point>* retTakenPoints,
		const std::vector<BlockInfo>& old_info, int takenCount);
};

