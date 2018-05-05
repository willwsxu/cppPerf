// (c) 2001-2005 Townsend Analytics, Ltd. All rights reserved.
#ifndef _CDynBuffer_HH_
#define _CDynBuffer_HH_

#include <cassert>
#include <sstream>
#include <algorithm>
#include <memory>
#include <ctime>
#include <string>

void szCurTime(char szTime[], size_t size)
{
	time_t tim;
	(void)time(&tim);
	struct tm buf;
	errno_t result = localtime_s(&buf, &tim);
	if (result != 0)
		return;
	asctime_s(szTime, size, &buf);
	int len = (int)strlen(szTime);
	szTime[len - 1] = 0;
}

#define SEV_WARN 2
// Recommend to call reserve to avoid memory realloc
// no virtual functions for speed
template<typename Logger, typename T>
class CDynBuffer
{
protected:
	Logger			logger;
	std::unique_ptr<T[]>			m_pBuffer;
	unsigned long	m_lSize;		// size of the current buffer
	unsigned long	m_lMaxSize;		// max allowed
	unsigned long	m_lLen;
	T *				m_pCur;
	int				resizeCount;
	int				resizeMove;
	int				resizeFailedCount;
	int				resizeAttempted;
	int				resizeException;
	unsigned long	m_peakLen;
	char			szBufPeakTime[100];
	long			totalDiscardCount;
	long			totalDiscardMsg;
	__int64			totalDiscardBytes;

public:
	CDynBuffer(unsigned long initSize = 4096, unsigned long maxSize = 0x1000000)
	{
		m_lSize = initSize;
		m_lMaxSize = maxSize;
		m_lLen = 0;
		m_pBuffer = make_unique<T[]>(m_lSize);
		m_pCur = m_pBuffer.get();
		ClearStats();
	}

	bool resize(unsigned long len) {
		if (m_lLen + len > m_lMaxSize)
		{
			resizeFailedCount++;
			return false;
		}
		if (m_lLen + len > m_lSize) {	// time to expand buffer
			std::unique_ptr<T[]> pOld = std::move(m_pBuffer);
			unsigned long newSize = m_lSize / 2 * 3 + 2;
			resizeAttempted++;
			if (newSize < m_lLen + len) {
				newSize = m_lLen + len;
				newSize = newSize / 2 * 3 + 2;
			}
			if (newSize > m_lMaxSize)
			{
				newSize = m_lMaxSize;
				if (newSize - m_lSize < 100)
				{
					resizeFailedCount++;
					return false;
				}
			}
			try {
				m_pBuffer = make_unique<T[]>(newSize);
			}
			catch (std::bad_alloc())
			{
				m_pBuffer = 0;
			}
			catch (...)
			{
				m_pBuffer = 0;
			}
			if (m_pBuffer == 0)
			{
				// roll back
				resizeException++;
				m_pBuffer = std::move(pOld);
				logger(SEV_WARN, "DynBuffer [%p]. Failed to resize to %lu from %lu", m_pBuffer.get(), newSize, m_lSize);
				return false;
			}
			if (newSize == m_lMaxSize)
			{
				logger("[%p] DynBuffer (from [%p]) resize to max allowed %d", m_pBuffer.get(), pOld.get(), m_lMaxSize);
			}
			memcpy(m_pBuffer.get(), m_pCur, m_lLen);

			m_pCur = m_pBuffer.get();
			m_lSize = newSize;
			resizeCount++;
		}
		else if (m_pBuffer.get() - m_pCur + m_lSize < len + m_lLen) {  // move data to beginning if there is not enough from pCur
#ifdef UNIT_TEST
			char *pTemp = new char[m_lLen + 1];
			memcpy(pTemp, m_pCur, m_lLen);
#endif
			memmove(m_pBuffer.get(), m_pCur, m_lLen);
#ifdef UNIT_TEST
			assert(memcmp(m_pBuffer, pTemp, m_lLen) == 0);
			delete[]pTemp;
#endif
			//			memcpy(m_pBuffer, pTemp, m_lLen);
			m_pCur = m_pBuffer.get();
			resizeMove++;
		}
		return true;
	}

	// check if buffer pair is intact
	bool Pair(CDynBuffer& pair)
	{
		return m_lSize == pair.m_lSize && m_lMaxSize == pair.m_lMaxSize;
	}

	// Application should lock and also called pair to ensure two buffers are swapable
	// data flow pair -> this
	// Assert this->m_lLen==0 && m_pCur == m_pBuffer
	// Swap buffer from its pair
	bool swapBuffer(CDynBuffer& pair)
	{
		assert(m_lLen == 0);
		assert(m_pBuffer.get() == m_pCur);
		assert(pair.m_pBuffer.get() == pair.m_pCur);
		if (pair.m_lSize > m_lSize)
		{
			std::unique_ptr<T[]> pOld = std::move(m_pBuffer);
			try {
				m_pBuffer = make_unique<T[](pair.m_lSize);
			}
			catch (std::bad_alloc())
			{
				m_pBuffer = std::move(pOld);
			}
			catch (...)
			{
				m_pBuffer = std::move(pOld);
			}
			if (pOld==nullptr)
			{
				logger(SEV_WARN, "DynBuffer swap from [%p] [%p]. Failed to resize to %lu from %lu", m_pBuffer.get(), pair.m_pBuffer.get(), pair.m_lSize, m_lSize);
			}
			else
			{
				if (pair.m_lSize == m_lMaxSize)
					logger("DynBuffer swap old [%p] -> new [%p] resize to max allowed %lu from %lu. Swap with [%p]", pOld, m_pBuffer, m_lMaxSize, m_lSize, pair.m_pBuffer);
				m_lSize = pair.m_lSize;
			}
		}
		if (pair.m_lLen>0)
		{
			std::swap(m_pBuffer, pair.m_pBuffer);
			std::swap(m_pCur, pair.m_pCur);
			std::swap(m_lLen, pair.m_lLen);
			std::swap(m_lSize, pair.m_lSize);
			if (m_lLen>m_peakLen)
			{
				m_peakLen = m_lLen;
				szCurTime(szBufPeakTime, sizeof(szBufPeakTime));
			}
			return true;
		}
		return false;
	}


	bool AppendData(const char *data, unsigned long len)
	{
		if (!resize(len))
			return false;
		assert(m_pBuffer.get() - m_pCur + m_lSize >= len + m_lLen);
		memcpy(m_pCur + m_lLen, data, len);

		m_lLen += len;
		if (m_lLen>m_peakLen)
		{
			m_peakLen = m_lLen;
			szCurTime(szBufPeakTime, sizeof(szBufPeakTime));
		}
		return true;
	}

	bool AppendData(const char *data)
	{
		return AppendData(data, (unsigned long)strlen(data));
	}

	const char *GetBuffer() const {
		return m_pCur;
	}

	unsigned long GetBufferLen() const {
		return m_lLen;
	}

	unsigned long GetSize() const {
		return m_lSize;
	}

	unsigned long GetMaxSize() const {
		return m_lMaxSize;
	}

	unsigned long GetPeakLen() const {
		return m_peakLen;
	}
	const char *GetPeakTime() const
	{
		return szBufPeakTime;
	}
	unsigned long GetResizeCount() const {
		return resizeCount;
	}
	unsigned long GetResizeMove() const {
		return resizeMove;
	}
	unsigned long GetResizeFail() const {
		return resizeFailedCount;
	}

	// Reserve the buffer size, update maz size if needed
	void Reserve(unsigned long max_)
	{
		if (m_lMaxSize<max_)
			m_lMaxSize = max_;
		resize((max_ - m_lLen) * 2 / 3);  // resize would inflate 1.5 fold
	}

	void ClearStats()
	{
		m_peakLen = 0;
		resizeCount = 0;
		resizeMove = 0;
		resizeFailedCount = 0;
		szBufPeakTime[0];
		resizeAttempted = 0;
		resizeException = 0;
		totalDiscardCount = 0;
		totalDiscardMsg = 0;
		totalDiscardBytes = 0;
	}

	void GetStats(std::ostringstream &ostr)
	{
		ostr << "DynBuf ";
		getStats_(ostr);
	}


	void ClearData() {
		m_pCur = m_pBuffer.get();
		m_lLen = 0;
	}

	// Mark data as read, move buffer pointer and update length
	void SetDataConsumed(unsigned long lLen) {
		if (lLen>m_lLen)
			lLen = m_lLen;
		m_lLen -= lLen;
		m_pCur += lLen;
		if (m_lLen == 0)
			m_pCur = m_pBuffer.get();
	}
	// A special case
	void ConsumeLong()
	{
		assert(m_lLen >= sizeof(long));
		//		unsigned long val = (unsigned long)m_pCur;
		SetDataConsumed(sizeof(long));
		//		return val;
	}


	// use this buffer to store/write data directly, then tell read size
	// Need to call GetWritePtr first
	// call resize first to make sure enough buffer
	// Name should be AppendData
	bool DataRead(unsigned long len)
	{
		assert(m_pBuffer.get() - m_pCur + m_lSize >= len + m_lLen);
		m_lLen += len;
		return true;
	}
	// 
	char *GetWritePtr()
	{
		return m_pCur + m_lLen;
	}



	void test(int n)
	{
		char temp[2050];
		long x = rand();
		long y = rand();
		long discard = 0;
		while (n--) {
			x %= 2049;
			y %= 1037;
			if (x) {
				memset(temp, x % 26 + 'A', x);
				if (!AppendData(temp, x))
				{
					std::ostringstream ostr;
					discard += x;
					GetStats(ostr);
					printf("Discard %d. %s\n", discard, ostr.str().c_str());
				}
			}
			if (y)
				SetDataConsumed(y);
			x = rand();
			y = rand();
		}
	}

protected:
	void getStats_(std::ostringstream &ostr)
	{
		ostr << "Len " << m_lLen << "(alloc size " << m_lSize << " max " << m_lMaxSize << "), peak " << m_peakLen << " at " << szBufPeakTime;
		if (totalDiscardBytes > 0)
			ostr << ", Discard " << totalDiscardBytes;
		if (resizeCount>0 || resizeFailedCount>0 || resizeMove>0 || resizeException>0)
			ostr << ", resize # " << resizeCount;
		if (resizeMove>0)
			ostr << ", Move # " << resizeMove;
		if (resizeFailedCount>0)
			ostr << ", failed " << resizeFailedCount;
		if (resizeException>0)
			ostr << ", badalloc " << resizeException;
		if (resizeAttempted > 0)
			ostr << ", attempt " << resizeAttempted;

	}
	CDynBuffer(const CDynBuffer&)=delete;  // support no copy
	CDynBuffer& operator=(const CDynBuffer&) = delete;  // support no copy
};

#endif	//_CDynBuffer_HH_

