/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 19:45
  file base: typedef
  file ext: hpp
  author: zhy

  purpose:
*********************************************************************/
#ifndef _TYPE_DEF_HPP_
#define _TYPE_DEF_HPP_

#if !PLATFORM_IOS
#pragma warning(disable: 4125)
#endif

#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <math.h>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#ifdef _WIN32
#include <intrin.h>
#include <assert.h>
#include <mmintrin.h>
#include <basetsd.h>
#endif

#include <xchar.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/exception/to_string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <tracy/Tracy.hpp>
#include "Logic/game_enum_def_s.h"


namespace faith
{
	// 跨平台类型定义
	typedef boost::int8_t		i8;
	typedef boost::int16_t		i16;
	typedef boost::int32_t		i32;
	typedef boost::int64_t		i64;

	typedef boost::uint8_t		ui8;
	typedef boost::uint16_t		ui16;
	typedef boost::uint32_t		ui32;
	typedef boost::uint64_t		ui64;

	typedef boost::int8_t		int8;
	typedef boost::int16_t		int16;
	typedef boost::int32_t		int32;
	typedef boost::int64_t		int64;

	typedef boost::uint8_t		uint8;
	typedef boost::uint16_t		uint16;
	typedef boost::uint32_t		uint32;
	typedef boost::uint64_t		uint64;

#define FString				std::string
#define TArray				std::vector
#define TSet				std::set
#define TMap				std::map
	typedef wchar_t				TCHAR;
	typedef char				ANSICHAR;
	typedef boost::function<void(uint32, const void*, size_t)> msg_handler_type;
	typedef boost::function<bool(const void*, size_t)> data_len_verifier_type;

	typedef float				f32;
	typedef double				f64;
#ifdef _WIN32
	typedef ULONG_PTR			ulong_ptr;
#else
	typedef	ui64* ULONG_PTR;
#endif
#pragma pack(push,1)

#define FLOAT_IS_ZERO(float_value) (fabs(float_value)<1e-5)
#define FLOAT_NEAR_ZERO(fValue) ((fValue)> -0.1 && (fValue) < 0.1)

#define  SAFE_DELETE(ptr) {if(ptr) delete ptr; ptr=NULL;}

#ifndef VALID_INFO_ID			// 有效的模板ID
#define VALID_INFO_ID(ID) ( (ID) != -1 && (ID) != 0 )
#endif
#define INDEX_NONE -1
#define int32_MIN_NUM								-2147483648	// int32最小值
#define int32_MAX_NUM								2147483647	// int32最大值
#define Uint32_MAX_NUM							4294967295	// uint32最大值

#define MAX_GROUP_ID							64
#define BATTLE_GROUP_ID							MAX_GROUP_ID
#define VALID_GROUP_ID(group_id)				( group_id>0 && group_id<=MAX_GROUP_ID )
#define GROUP_ROLE_ID_RANGE						( (0x7fffffff) / MAX_GROUP_ID )
#define GROUP_ROLE_ID_MIN(group_id)				(  GROUP_ROLE_ID_RANGE*(group_id-1))
#define GROUP_ROLE_ID_MAX(group_id)				( GROUP_ROLE_ID_MIN(group_id) + GROUP_ROLE_ID_RANGE)
#define GET_ROLE_GROUP(obj_id)					( (obj_id)/GROUP_ROLE_ID_RANGE + 1)

	const f32 FAITH_PI = 3.14159265359f;
	const int32 FULL_ANGLE = 360;
	const f32 RADIAN = FAITH_PI / 180.0f;
	const f32 ANGLE = 180.0f / FAITH_PI;
	const int32 packege_length_max = 11500;
	const int32 packege_length_max_c2s = 11500;
	const int32 guid_size = 2;
	const int32 kInvalidId = -1;
	const float k_kinda_small_number = 1.e-4f;
	const int32 md5_length = 32;
	static xchar empty_char_array[1] = {0};
	static xstring empty_string = "";

	typedef	std::list<int32>	unit_index_list;
	typedef unit_index_list::iterator	unit_index_list_it;
	typedef	std::map<int32, int32>	unit_index_map;
	typedef unit_index_map::iterator	unit_index_map_it;
	typedef unit_index_map::const_iterator	unit_index_map_const_it;
	typedef	std::map<ui64, int64>	unit_guid_map;
	typedef unit_guid_map::iterator	unit_guid_map_it;
	void none_fuction(uint32 array_index, const void* data_ptr, size_t data_len);

	template<typename T>
	void g_clear_data(T* data, int32 length)
	{
		for (int32 i = 0; i < length; i++)
		{
			data[i].clear_data();
		}
	}
	template<typename T>
	void g_clear_data(T& data)
	{
		data.clear_data();
	}
	template<typename T>
	void random_array(T& data)
	{
		std::mt19937 gen(std::random_device{}());
		std::shuffle(data.begin(), data.end(), gen);
	}

	struct guid_64
	{
	public:
		union
		{
			struct
			{
				uint32 A;
				uint32 B;
			};
			ui64 server_64;
		};

		guid_64() : server_64(0)
		{}

		guid_64(const guid_64& rGuid)
		{
			server_64 = rGuid.server_64;
		}

		guid_64(const ui64& In64)
		{
			server_64 = In64;
		}

		guid_64(uint32 InA, uint32 InB)
			: A(InA), B(InB)
		{}
		int32 get_A() { return A; }
		int32 get_B() { return B; }
		int64 get_AB() { return server_64; }
		xstring to_string() const
		{
			xstring temp_guid;
			std::stringstream stream;
			uint32 temp_value = A;
			uint32 cut_value = 1000000000;
			int32 i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			temp_value = B;
			cut_value = 1000000000;
			i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			stream >> temp_guid;
			return temp_guid;
		}
		void parse_guid(std::string str_guid)
		{
			if (str_guid.length() < 20)
			{
				return;
			}
			std::stringstream str_a;
			str_a << str_guid.substr(0, 10);
			str_a >> A;
			std::stringstream str_b;
			str_b << str_guid.substr(10, 10);
			str_b >> B;
		}
		void clear_data()
		{
			server_64 = 0;
		}
		bool is_valid() const
		{
			return server_64 != 0;
		}
		operator ui64() const
		{
			return server_64;
		}
		bool operator < (const guid_64& _Left) const
		{
			return  server_64 < _Left.server_64;
		}

		// Binary comparison operators.
		bool operator == (const guid_64& _left) const
		{
			return server_64 == _left.server_64;
		}
		bool operator != (const guid_64& _left) const
		{
			return server_64 != _left.server_64;
		}

		guid_64& operator = (const guid_64& _left)
		{
			server_64 = _left.server_64;
			return *this;
		}

		guid_64& operator = (const ui64& _left)
		{
			server_64 = _left;
			return *this;
		}
	};
	static guid_64 empty_guid_64 = guid_64(0, 0);
	struct guid_128
	{
	public:
		uint32 A;
		uint32 B;
		uint32 C;
		uint32 D;

		guid_128()
		{
			clear_data();
		}

		guid_128(const guid_128& r)
		{
			A = r.A;
			B = r.B;
			C = r.C;
			D = r.D;
		}

		guid_128(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
			: A(InA), B(InB), C(InC), D(InD)
		{}

		/**
		* Returns whether this _Guid is valid or not. We reserve an all 0 _Guid to represent "invalid".
		*
		* @return TRUE if valid, FALSE otherwise
		*/
		void clear_data()
		{
			A = 0;
			B = 0;
			C = 0;
			D = 0;
		}
		xstring to_string() const
		{
			xstring temp_guid;
			std::stringstream stream;
			uint32 temp_value = A;
			uint32 cut_value = 1000000000;
			int32 i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			temp_value = B;
			cut_value = 1000000000;
			i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			temp_value = C;
			cut_value = 1000000000;
			i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			temp_value = D;
			cut_value = 1000000000;
			i = 0;
			while (i < 10)
			{
				uint32 temp = temp_value / cut_value;
				stream << temp;
				temp_value %= cut_value;
				cut_value /= 10;
				++i;
			}
			stream >> temp_guid;
			return temp_guid;
		}
		void parse_guid(std::string str_guid)
		{
			if (str_guid.length() < 40)
			{
				return;
			}
			std::stringstream str_a;
			str_a << str_guid.substr(0, 10);
			str_a >> A;
			std::stringstream str_b;
			str_b << str_guid.substr(10, 10);
			str_b >> B;
			std::stringstream str_c;
			str_c << str_guid.substr(20, 10);
			str_c >> C;
			std::stringstream str_d;
			str_d << str_guid.substr(30, 10);
			str_d >> D;
		}
		bool is_valid() const
		{
			return (A | B | C | D) != 0;
		}

		bool operator < (const guid_128& _Left) const
		{
			return memcmp(this, &_Left, sizeof(guid_128)) < 0;
		}
		guid_128 operator = (const guid_128& _right)
		{
			A = _right.A;
			B = _right.B;
			C = _right.C;
			D = _right.D;
			return *this;
		}
		// Binary comparison operators.
		bool operator == (const guid_128& _left) const
		{
			return ((_left.A == A) && (_left.B == B) && (_left.C == C) && (_left.D == D));
		}
		bool operator != (const guid_128& _left) const
		{
			return ((_left.A ^ A) | (_left.B ^ B) | (_left.C ^ C) | (_left.D ^ D)) != 0;
		}

		//support for hash
		operator size_t() const
		{
			return (A & 0x000000FF) | (B & 0x0000FF00) | (C & 0x00FF0000) | (D & 0xFF000000);
		}
	};
	//#ifdef _WIN32
	/**
	* Converts a float to an integer with truncation towards zero.
	* @param F		Floating point value to convert
	* @return		Truncated integer.
	*/
	inline int32 app_trunc(f32 F)
	{
#ifdef _WIN32
		return _mm_cvtt_ss2si(_mm_set_ss(F));
#else
		return (int32)F;
#endif
	}


	/**
	* Converts a float to a less or equal integer.
	* @param F		Floating point value to convert
	* @return		An integer less or equal to 'F'.
	*/
	inline int32 app_floor(f32 F)
	{
		// Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5
#ifdef _WIN32
		return _mm_cvt_ss2si(_mm_set_ss(F + F - 0.5f)) >> 1;
#else
		return app_trunc(floor(F));
#endif
	}

	// Constants.
	enum { e_angle_shift = 2 };		// Bits to right-shift to get lookup value.
	enum { e_angle_bits = 14 };		// Number of valid bits in angles.
	enum { e_num_angles = 16384 }; 	// Number of angles that are in lookup table.
	enum { e_angle_mask = (((1 << e_angle_bits) - 1) << (16 - e_angle_bits)) };

	/**
	* Eliminates all fractional precision from an angle.
	*/
	inline int32 reduce_angle(int32 angle)
	{
		return angle & e_angle_mask;
	};

	/**
	* Add to a word angle, constraining it within a min (not to cross)
	* and a max (not to cross).  Accounts for funkyness of word angles.
	* Assumes that angle is initially in the desired range.
	*/
	inline ui16 fadd_angle_confined(int32 angle, int32 delta, int32 minthresh, int32 maxthresh)
	{
		if (delta < 0)
		{
			if (delta <= -0x10000l || delta <= -(int32)((ui16)(angle - minthresh)))
				return (ui16)minthresh;
		}
		else if (delta > 0)
		{
			if (delta >= 0x10000l || delta >= (int32)((ui16)(maxthresh - angle)))
				return (ui16)maxthresh;
		}
		return (ui16)(angle + delta);
	}

	/**
	* snaps a value to the nearest grid multiple.
	*/
	inline f32 fsnap(f32 location, f32 grid)
	{
		if (grid == 0.f)	return location;
		else			return (f32)app_floor((location + 0.5f * grid) / grid) * grid;
	}

	/**
	* snaps a value to the nearest grid multiple.
	*/
	inline f32 fsnap(int32 location, int32 grid)
	{
		return fsnap((f32)location, (f32)grid);
	}

	template< class t > inline t abs(const t a)
	{
		return (a >= (t)0) ? a : -a;
	}
#pragma pack(pop)
}
#endif
