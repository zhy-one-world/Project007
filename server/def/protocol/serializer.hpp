#ifndef _SERIALIZER_HPP
#define _SERIALIZER_HPP

#include "logic/type_def.hpp"

#define MAX_DATASERIALIZER_LENGTH	( 1 * 256 * 1024 )
#define MAX_ALLOCATE_PART	(128)

namespace faith
{
	//Notice: Send Net Data In One Thread
	struct allocator {
		unsigned char m_szBuffer[MAX_ALLOCATE_PART][(MAX_DATASERIALIZER_LENGTH + 1)];
		//Mutex m_Lock;

		allocator()
		{
			memset(m_szBuffer, 0, sizeof(m_szBuffer));
		}
		unsigned char* allocate() {
			//m_Lock.lock();
			for (int32 i = 0; i < MAX_ALLOCATE_PART; i++)
			{
				if (m_szBuffer[i][0] <= 0)
				{
					m_szBuffer[i][0] = 1;
					return &m_szBuffer[i][0];
				}
			}
			//m_Lock.unlock();
			return NULL;
		}

		void  deallocate(unsigned char* pBuffer) {
			//m_Lock.lock();
			if (pBuffer)
				(*pBuffer) = 0;
			//m_Lock.unlock();
		}
	};

	extern allocator g_DataSerializerAlocator;

	struct serializer {
		unsigned char* m_szWriteBuffer;
		unsigned char* m_pWritePtr;
		int32   m_nRemainSize;
		serializer() {
			m_nRemainSize = MAX_DATASERIALIZER_LENGTH;
			m_szWriteBuffer = g_DataSerializerAlocator.allocate();
			if (m_szWriteBuffer)
				m_pWritePtr = (m_szWriteBuffer + 1);
			else
				m_pWritePtr = NULL;
		}

		~serializer()
		{
			g_DataSerializerAlocator.deallocate(m_szWriteBuffer);
		}

		const unsigned char* getBuffer() const {
			return (m_szWriteBuffer + 1);
		}

		int32 writeBuffer(const void* pBuffer, int32 nWriteSize) {
			if (m_pWritePtr && pBuffer && nWriteSize > 0) {
				int32 nByte = nWriteSize > m_nRemainSize ? m_nRemainSize : nWriteSize;
				if (nWriteSize > m_nRemainSize)
				{
					//ASSERT(0);
				}
				memcpy(m_pWritePtr, pBuffer, nByte);
				m_pWritePtr += nByte;
				m_nRemainSize -= nByte;
				return nByte;
			}
			return 0;
		}

		int32 writeZeroBuffer(int32 nWriteSize) {
			if (m_pWritePtr && nWriteSize > 0) {
				int32 nByte = nWriteSize > m_nRemainSize ? m_nRemainSize : nWriteSize;
				if (nWriteSize > m_nRemainSize)
				{
					//ASSERT(0);
				}
				memcpy(m_pWritePtr, 0, nByte);
				m_pWritePtr += nByte;
				m_nRemainSize -= nByte;
				return nByte;
			}
			return 0;
		}

		template<class DataType>
		int32 writeProtoBuf(const DataType& data) {
			int32 nWriteSize = data.ByteSize();
			if (m_pWritePtr && nWriteSize > 0) {
				int32 nByte = nWriteSize > m_nRemainSize ? m_nRemainSize : nWriteSize;
				if (nWriteSize > m_nRemainSize)
				{
					//ASSERT(0);
				}
				data.SerializeToArray(m_pWritePtr, nByte);
				m_pWritePtr += nByte;
				m_nRemainSize -= nByte;
				return nByte;
			}
			return 0;
		}

		template<class DataType>
		int32 write(const DataType& data) {
			return writeBuffer((const void*)&data, sizeof(data));
		}
	};

	struct deserializer
	{
		char* m_pReadPtr;
		int32 m_nRemainSize;
		deserializer() {
			m_pReadPtr = NULL;
			m_nRemainSize = 0;
		}

		deserializer(const void* pBuffer, int32 nBufferSize) {
			init(pBuffer, nBufferSize);
		}

		void init(const void* pBuffer, int32 nBufferSize)
		{
			//ASSERT(pBuffer);
			m_pReadPtr = (char*)pBuffer;
			m_nRemainSize = nBufferSize;
		}

		int32 skipByte(int32 nLength) {
			if (m_pReadPtr && nLength > 0) {
				int32 nByte = nLength > m_nRemainSize ? m_nRemainSize : nLength;
				if (nLength > m_nRemainSize) {
					//ASSERT(0);
					return 0;
				}
				m_pReadPtr += nByte;
				m_nRemainSize -= nByte;
				return nByte;
			}
			return 0;

		}
		int32 readBuffer(void* pBuffer, int32 nReadSize) {
			if (m_pReadPtr && pBuffer && nReadSize > 0) {
				int32 nByte = nReadSize > m_nRemainSize ? m_nRemainSize : nReadSize;
				if (nReadSize > m_nRemainSize) {
					//ASSERT(0);
					return 0;
				}
				memcpy(pBuffer, m_pReadPtr, nByte);
				m_pReadPtr += nByte;
				m_nRemainSize -= nByte;
				return nByte;
			}
			return 0;
		}

		template<class DataType>
		int32 readProtoBuf(DataType& data, int32 nReadSize) {
			if (m_pReadPtr && nReadSize > 0) {
				if (nReadSize > m_nRemainSize) {
					//ASSERT(0);
					return 0;
				}
				data.ParseFromArray(m_pReadPtr, nReadSize);
				m_pReadPtr += nReadSize;
				m_nRemainSize -= nReadSize;
				return nReadSize;
			}
			return 0;
		}

		template<class DataType>
		int32 read(DataType& data) {
			return readBuffer((void*)&data, sizeof(data));
		}
	};

#pragma pack(push, 1)

	class protocolPacker
	{
	public:
		serializer m_sSerializer;
		ui32	length;
		ui16	wheader;
		ui16	wSubHeader;

	public:
		protocolPacker(ui16 wProtocol)
		{
			wheader = wProtocol;
			wSubHeader = 0;
			length = 0;
			length += m_sSerializer.write(wProtocol);
			length += m_sSerializer.write(wSubHeader);
		}

		protocolPacker(ui16 wProtocol, ui16 wSubProtocol)
		{
			wheader = wProtocol;
			wSubHeader = wSubProtocol;
			length = 0;
			length += m_sSerializer.write(wheader);
			length += m_sSerializer.write(wSubHeader);
		}

		void push(const void* pdata, int32 nLength)
		{
			if (pdata == NULL || nLength == 0) {
				return;
			}
			length += m_sSerializer.writeBuffer(pdata, nLength);
		};

		template<typename Type>
		void push(const Type& data)
		{
			length += m_sSerializer.write(data);
		}

		template<typename Type>
		void pushSerializerData(Type& data)
		{
			length += data.SerializeToNet(m_sSerializer);
		}

		template<typename Type>
		void pushProtoBuf(Type& data)
		{
			length += m_sSerializer.write(data.ByteSize());
			length += m_sSerializer.writeProtoBuf(data);
		}

		const unsigned char* getBuffer() const
		{
			return m_sSerializer.getBuffer();
		}

		const int32	getLength() const
		{
			return length;
		}

		int32	getProtocol() const
		{
			return wheader;
		}

		int32	getSubProtocol() const
		{
			return wSubHeader;
		}
	};

	class protocolUnpacker
	{
	public:
		ui16	wheader;
		ui16	wSubHeader;
		deserializer m_sDeserializer;
		ui32	length;

	public:
		protocolUnpacker(const void* pMsg, int32 len)
		{
			length = 0;
			m_sDeserializer.init(pMsg, len);
			pop(wheader);
			pop(wSubHeader);
		}

		~protocolUnpacker()
		{
			//ASSERT(m_sDeSerializer.m_nRemainSize <= 0);
		}

		void pop(void* pdata, int nLength)
		{
			if (pdata == NULL || nLength == 0) {
				return;
			}
			length += m_sDeserializer.readBuffer(pdata, nLength);
		};

		template<typename Type>
		void pop(Type& data)
		{
			length += m_sDeserializer.read(data);
		}

		template<typename Type>
		void popProtoBuf(Type& data)
		{
			uint32 size = 0;
			m_sDeserializer.read(size);
			if (size > 0)
			{
				length += m_sDeserializer.readProtoBuf(data, size);
			}
		}

		void skipByte(int32 len)
		{
			length += m_sDeserializer.skipByte(len);
		}

		int32	getProtocol() const
		{
			return wheader;
		}

		int32	getSubProtocol() const
		{
			return wSubHeader;
		}
	};

#pragma pack(pop)
}

#endif // _SERIALIZER_HPP