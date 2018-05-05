#pragma once

#include "dynbuffer.h"
#include <sstream>
// each msg is prefixed with its len as unsigned long, Len is msg Len only

// Don't call any CDynBuffer methods that change the buffer
// Default max size is 32MB
template<typename Logger, typename T>
class CDynMsg : public CDynBuffer<Logger, T>
{
protected:
	long msgs;

public:
	CDynMsg(const CDynMsg& copy_) : CDynBuffer(copy_.m_lSize, copy_.m_lMaxSize) {
		copyFrom(copy_);
	}

	CDynMsg() {
		msgs = 0;
		m_lMaxSize = 0x2000000;
	}
	//CDynMsg(const CDynMsg&) = delete;  // support no copy
	//CDynMsg& operator=(const CDynMsg&) = delete;  // support no copy

	// Application should lock
	void copyFrom(const CDynMsg& copy_) {
		CDynBuffer::copyFrom(copy_);
		msgs = copy_.msgs;
	}

	bool swapBuffer(CDynMsg& pair)
	{
		if (msgs == 0 && pair.msgs>0)
		{
			std::swap(msgs, pair.msgs);
			return CDynBuffer::swapBuffer(pair);
		}
		return false;
	}

	// len - length of pBuf, exclude \0 if any
	bool AddMsg(char *pBuf, unsigned long len) {
		bool ok = AppendData((char*)&len, sizeof(unsigned long));
		if (ok) {
			ok = AppendData(pBuf, len);
			if (!ok)
				m_lLen -= sizeof(unsigned long);//roll back
		}
		if (ok)
			msgs++;
		return ok;
	}

	bool AddMsg(short iType, char *pBuf, unsigned long len) {
		len += sizeof(short);// include type in len
		bool ok = AppendData((char*)&len, sizeof(unsigned long));
		if (!ok)
			return ok;
		ok = AppendData((char*)&iType, sizeof(short));
		if (!ok) {
			m_lLen -= sizeof(unsigned long);//roll back
			return ok;
		}
		ok = AppendData(pBuf, len - sizeof(short));
		if (!ok) {
			m_lLen -= sizeof(unsigned long);//roll back
			m_lLen -= sizeof(short);//roll back
		}
		else
			msgs++;
		return ok;
	}

	// addr is client pointer address
	bool AddMsgCliData(void * addr, char *pBuf, unsigned long len) {
		len += sizeof(void *);// include addr in len
		bool ok = AppendData((char*)&len, sizeof(unsigned long));
		if (!ok)
			return ok;
		ok = AppendData((char*)&addr, sizeof(void *));
		if (!ok) {
			m_lLen -= sizeof(unsigned long);//roll back
			return ok;
		}
		ok = AppendData(pBuf, len - sizeof(void *));
		if (!ok) {
			m_lLen -= sizeof(unsigned long);//roll back
			m_lLen -= sizeof(void *);//roll back
		}
		else
			msgs++;
		return ok;
	}
	// reverse of AddMsgCliData
	const char *GetMsgCliData(void **pCliData, unsigned long *piLen) {
		const char *pBuf = GetBuffer();
		*piLen = 0;
		*pCliData = 0;
		if (GetBufferLen() <= sizeof(unsigned long) * 2)
			return NULL;
		*piLen = *(unsigned long *)pBuf;
		*piLen -= sizeof(void *);
		pBuf += sizeof(unsigned long);
		*pCliData = *(void **)pBuf;
		return pBuf + sizeof(void *);
	}

	void DiscardMsg(int iWhich)
	{
		totalDiscardCount++;
		long lBytes = 0;
		tb_sev_msg(SEV_WARN, "DynMsg buffer [%p] [%d] is full (%u) bytes", GetBuffer(), iWhich + 1, GetBufferLen());
		int nRem = RemoveMsg(GetNumMsgs() / 5 * 2, &lBytes);//discard 40%
		totalDiscardMsg += nRem;
		totalDiscardBytes += lBytes;
		tb_sev_msg(SEV_WARN, "DynMsg [%d] discard %d packets ( %u bytes), %d msg remain. Total Discarded Count %lu packets %lu, bytes %I64u",
			iWhich + 1, nRem, lBytes, msgs, totalDiscardCount, totalDiscardMsg, totalDiscardBytes);
		std::ostringstream ostr;
		GetStats(ostr);
		tb_msg("DynMsg buffer [%d] %s", iWhich + 1, ostr.str().c_str());
	}

	bool RemoveMsg() {
		const char *pBuf = GetBuffer();
		unsigned long iLen = *(unsigned long *)pBuf;
		if (m_lLen <iLen + sizeof(unsigned long))
			return false;
		msgs--;
		SetDataConsumed(iLen + sizeof(unsigned long));
		return true;
	}

	// return actual number of msg removed
	int RemoveMsg(int n, long *bytes) {
		*bytes = 0;
		for (int i = 0; i<n; i++)
		{
			const char *pBuf = GetBuffer();
			unsigned long iLen = *(unsigned long *)pBuf;
			if (m_lLen <sizeof(unsigned long) + iLen)
			{
				*bytes += GetBufferLen();
				n = i + msgs;
				RemoveAll();
				return n;
			}
			*bytes += iLen;
			msgs--;
			SetDataConsumed(iLen + sizeof(unsigned long));
		}
		return n;
	}
	const char *GetMsg(unsigned long *piLen) {
		const char *pBuf = GetBuffer();
		*piLen = 0;
		if (GetBufferLen() <= sizeof(unsigned long))
			return NULL;
		*piLen = *(unsigned long *)pBuf;
		return pBuf + sizeof(unsigned long);
	}

	const char *GetMsg(const char *pNext, unsigned long *piLen) {
		const char *pBegin = GetBuffer();
		*piLen = 0;
		if ((unsigned long)GetBufferLen() <= pNext - pBegin + sizeof(unsigned long))
			return NULL;
		*piLen = *(unsigned long *)pNext;
		return pNext + sizeof(unsigned long);
	}

	const char *GetMsg(unsigned long *piLen, short *piType) {
		const char *pBuf = GetBuffer();
		*piLen = 0;
		if (GetBufferLen() <= sizeof(unsigned long) + sizeof(short))
			return NULL;
		*piLen = *(unsigned long *)pBuf;
		*piLen -= sizeof(short);
		pBuf += sizeof(long);
		*piType = *(short *)pBuf;
		return pBuf + sizeof(short);
	}
	const char *GetMsg(const char *pNext, unsigned long *piLen, short *piType) {
		const char *pBegin = GetBuffer();
		*piLen = 0;
		if ((unsigned long)GetBufferLen() <= pNext - pBegin + sizeof(long) + sizeof(short))
			return NULL;
		*piLen = *(unsigned long *)pNext;
		*piLen -= sizeof(short);
		pNext += sizeof(long);
		*piType = *(short *)pNext;
		return pNext + sizeof(short);
	}

	long GetNumMsgs() const {
		return msgs;
	}
	void RemoveAll() {
		ClearData();
		msgs = 0;
	}

	void GetStats(std::ostringstream &ostr)
	{
		ostr << "DynMsg Msg " << msgs << " ";
		getStats_(ostr);
	}

#ifdef UNIT_TEST
	void test(int n)
	{
		char temp[2050];
		long x = rand();
		long y = rand();
		while (n--) {
			x %= 204;
			y %= 2;
			if (x) {
				memset(temp, x % 26 + 'A', x);
				(void)AddMsg(temp, x);
			}
			if (y)
				(void)RemoveMsg();
			x = rand();
			y = rand();
		}
	}
#endif
};

