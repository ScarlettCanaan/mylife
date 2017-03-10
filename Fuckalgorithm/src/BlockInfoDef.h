#pragma once
#ifndef BLOCK_INFO_DEF_H__
#define BLOCK_INFO_DEF_H__

enum Direct
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
	NONE = 4,
};

struct Point
{
	int x;
	int y;
	Point(int _x, int _y) : x(_x), y(_y) {};
	Point() : x(0), y(0) {};
	float Distance(const Point& p2)const {
		short dx = p2.x - x;
		short dy = p2.y - y;
		return (float)sqrt(dx * dx + dy * dy);
	}
};

class BlockInfo
{
public:
	bool isHeart;
	size_t size;
	Point leftup;
	Point rightdown;
	Point central;

public:
	bool adjust();
	BlockInfo();
	~BlockInfo();
};

#endif
