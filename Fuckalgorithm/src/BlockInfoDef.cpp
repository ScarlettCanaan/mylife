#include "BlockInfoDef.h"
#include <assert.h>

bool BlockInfo::adjust() {
	assert(this->leftup.x >= 0 && this->leftup.y >= 0 && this->size >= 0);
	this->rightdown.x = this->leftup.x + this->size - 1;
	this->rightdown.y = this->leftup.y + this->size - 1;
	this->central.x = this->leftup.x + this->size / 2;
	this->central.y = this->leftup.y + this->size / 2;
	return true;
}

BlockInfo::BlockInfo() {

}

BlockInfo::~BlockInfo() {

}