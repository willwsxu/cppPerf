#pragma once
#include <cassert>

struct Chunk
{
	void Init(size_t blockSize, unsigned char blocks);
	void* Allocate(size_t blockSize);
	void Deallocate(void* p, size_t blockSize);

	unsigned char* pData_;
	unsigned char
		firstAvailableBlock_,   // which holds the index of the first block available in this chunk
		blocksAvailable_;		// the number of blocks available in this chunk, max 255
};

void Chunk::Init(size_t blockSize, unsigned char blocks)
{
	pData_ = new unsigned char[blockSize*blocks];
	firstAvailableBlock_ = 0;
	blocksAvailable_ = blocks;
	unsigned char *p = pData_;
	for (unsigned i = 0; i < blocks; p += blockSize)
		*p = ++i;  // first char of each block acting as pointer pointing to next available block
}

void *Chunk::Allocate(size_t blockSize) {
	if (!blocksAvailable_)
		return nullptr;
	unsigned char *res = pData_ + firstAvailableBlock_*blockSize;
	firstAvailableBlock_ = *res;
	--blocksAvailable_;
	return res;
}

void Chunk::Deallocate(void *p, size_t blockSize) {
	assert(p >= pData_);
	unsigned char *toRelease = static_cast<unsigned char *>(p);
	assert((toRelease - pData_) % blockSize == 0);
	*toRelease = firstAvailableBlock_;
	firstAvailableBlock_ = (toRelease - pData_) / blockSize;
	++blocksAvailable_;
}