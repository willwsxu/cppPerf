#pragma once
#include <cassert>
#include <vector>
#include <algorithm>

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

class FixedAllocator
{
public:
	void * Allocate();
	void Deallocate(void* p, size_t blockSize);
private:
	size_t	blockSize_;
	unsigned char	numBlocks_;
	using Chunks = std::vector<Chunk>;
	Chunks	chunks_;
	Chunk *	allocChunk_= nullptr;
	Chunk * deallocChunk_ = nullptr;
};

void *FixedAllocator::Allocate()
{
	if (allocChunk_ == nullptr || allocChunk_->blocksAvailable_ == 0) {
		auto found = std::find_if(chunks_.begin(), chunks_.end(), [](const Chunk& chk) { return chk.blocksAvailable_ > 0; });
		if (found == chunks_.end())
			allocChunk_ = &*found;
		else {
			chunks_.reserve(chunks_.size() + 1);
			chunks_.push_back(Chunk());
			chunks_.back().Init(blockSize_, numBlocks_);
			allocChunk_ = &chunks_.back();
			deallocChunk_ = &chunks_.back();
		}
	}
	return allocChunk_->Allocate(blockSize_);
}

class SmallObjAllocator
{

};