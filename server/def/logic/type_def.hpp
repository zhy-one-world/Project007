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
#include <boost/detail/atomic_count.hpp>
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
	struct frotator
	{
		frotator() {}
		frotator(f32 _roll, f32 _pitch, f32 _yaw) : roll(_roll), pitch(_pitch), yaw(_yaw) {}
		union
		{
			f32 roll;
			f32 Roll;
		};
		union
		{
			f32 pitch;
			f32 Pitch;
		};
		union
		{
			f32 yaw;
			f32 Yaw;
		};
		frotator& operator= (const frotator& temp_pos)
		{
			roll = temp_pos.roll;
			pitch = temp_pos.pitch;
			yaw = temp_pos.yaw;
			return *this;
		}
		inline bool operator == (const frotator& p_V) const
		{
			return FLOAT_IS_ZERO(roll - p_V.roll) && FLOAT_IS_ZERO(pitch - p_V.pitch) && FLOAT_IS_ZERO(yaw - p_V.yaw);
		}

		inline bool operator != (const frotator& p_V) const
		{
			return !FLOAT_IS_ZERO(roll - p_V.roll) || !FLOAT_IS_ZERO(pitch - p_V.pitch) || !FLOAT_IS_ZERO(yaw - p_V.yaw);
		}

		void clear_data()
		{
			roll = 0;
			pitch = 0;
			yaw = 0;
		}
		bool is_zero()
		{
			return FLOAT_IS_ZERO(roll) && FLOAT_IS_ZERO(pitch) && FLOAT_IS_ZERO(yaw);
		}
	};

	struct fvector
	{
		fvector() { x = y = z = 0; }
		fvector(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}
		fvector(f32 _x, f32 _y) : x(_x), y(_y), z(0) {}
		union
		{
			f32 x;
			f32 X;
		};
		union
		{
			f32 y;
			f32 Y;
		};
		union
		{
			f32 z;
			f32 Z;
		};
		frotator rotator()
		{
			frotator r;
			r.Roll = 0;
			r.Pitch = atan2f(Z, sqrt(X * X + Y * Y)) * ANGLE;
			r.Yaw = atan2f(Y, X) * ANGLE;
			return r;
		}
		fvector& operator= (const fvector& temp_pos)
		{
			x = temp_pos.x;
			y = temp_pos.y;
			z = temp_pos.z;
			return *this;
		}
		void clear_data()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		void set_data(float temp_x, float temp_y, float temp_z)
		{
			this->x = temp_x;
			this->y = temp_y;
			this->z = temp_z;
		}
		void set_data(std::vector<float>& location)
		{
			if (location.size() >= 3)
			{
				this->x = location[0];
				this->y = location[1];
				this->z = location[2];
			}
		}
		void set_data(std::vector<int32>& location)
		{
			if (location.size() >= 3)
			{
				this->x = (float)location[0];
				this->y = (float)location[1];
				this->z = (float)location[2];
			}
		}
		inline f32 length_sql()
		{
			return x * x + y * y + z * z;
		}
		inline f32 lenth_sql_2d()
		{
			return x * x + y * y;
		}
		inline f32 length()
		{
			return sqrt(length_sql());
		}
		inline f32 length_2d()
		{
			return sqrt(lenth_sql_2d());
		}

		inline f32 squared_distance(const fvector& rhs) const
		{
			return (*this - rhs).length_sql();
		}

		inline f32 squared_distance_2d(const fvector& rhs) const
		{
			return (*this - rhs).lenth_sql_2d();
		}

		inline f32 nomalize()
		{
			float l_fLen = length();

			if (!FLOAT_IS_ZERO(l_fLen))
			{
				x /= l_fLen;
				y /= l_fLen;
				z /= l_fLen;
			}
			return l_fLen;
		}

		inline fvector get_safe_normal(float tolerance = 1.e-8f) const
		{
			const float square_sum = x * x + y * y + z * z;

			if (square_sum == 1.f)
			{
				return *this;
			}
			else if (square_sum < tolerance)
			{
				return  fvector(0, 0, 0);
			}

			float square_num = sqrt(square_sum);
			if (FLOAT_IS_ZERO(square_num))
			{
				return  fvector(0, 0, 0);
			}

			const float scale = 1.0f / square_num;
			return fvector(x * scale, y * scale, z * scale);
		}

		inline bool is_valid()
		{
			return is_zero() == false;
		}
		bool is_zero()
		{
			return FLOAT_IS_ZERO(x) && FLOAT_IS_ZERO(y) && FLOAT_IS_ZERO(z);
		}
		inline const fvector& operator +() const
		{
			return *this;
		}
		inline fvector operator - () const
		{
			return fvector(-x, -y, -z);
		}
		inline bool operator == (const fvector& p_V) const
		{
			return FLOAT_IS_ZERO(x - p_V.x) && FLOAT_IS_ZERO(y - p_V.y) && FLOAT_IS_ZERO(z - p_V.z);
		}
		inline fvector operator + (const fvector& p_V) const
		{
			return fvector(x + p_V.x, y + p_V.y, z + p_V.z);
		}
		inline fvector& operator += (const fvector& p_V)
		{
			x += p_V.x;
			y += p_V.y;
			z += p_V.z;
			return *this;
		}
		inline fvector operator + (const float p_fVal) const
		{
			return fvector(x + p_fVal, y + p_fVal, z + p_fVal);
		}
		inline fvector& operator += (const float p_fVal)
		{
			x += p_fVal;
			y += p_fVal;
			z += p_fVal;
			return *this;
		}
		inline fvector operator - (const fvector& p_V) const
		{
			return fvector(x - p_V.x, y - p_V.y, z - p_V.z);
		}
		inline fvector& operator -= (const fvector& p_V)
		{
			x -= p_V.x;
			y -= p_V.y;
			z -= p_V.z;
			return *this;
		}
		inline fvector operator - (const float p_fVal)
		{
			return fvector(x - p_fVal, y - p_fVal, z - p_fVal);
		}
		inline fvector& operator -= (const float p_fVal)
		{
			x -= p_fVal;
			y -= p_fVal;
			z -= p_fVal;
			return *this;
		}
		inline fvector operator * (const fvector& p_V) const
		{
			return fvector(x * p_V.x, y * p_V.y, z * p_V.z);
		}
		inline fvector& operator *= (const fvector& p_V)
		{
			x *= p_V.x;
			y *= p_V.y;
			z *= p_V.z;
			return *this;
		}
		inline fvector operator * (const float p_fVal) const
		{
			return fvector(x * p_fVal, y * p_fVal, z * p_fVal);
		}
		inline fvector& operator *= (const float p_fVal)
		{
			x *= p_fVal;
			y *= p_fVal;
			z *= p_fVal;
			return *this;
		}
		inline fvector operator / (const fvector& p_V) const
		{
			if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z))
			{
				return fvector(0, 0, 0);
			}
			return fvector(x / p_V.x, y / p_V.y, z / p_V.z);
		}
		inline fvector& operator /= (const fvector& p_V)
		{
			if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z))
			{
				return *this;
			}

			x /= p_V.x;
			y /= p_V.y;
			z /= p_V.z;
			return *this;
		}
		inline fvector operator / (const float p_fVal) const
		{
			if (FLOAT_IS_ZERO(p_fVal))
			{
				return fvector(0, 0, 0);
			}
			return fvector(x / p_fVal, y / p_fVal, z / p_fVal);
		}
		inline fvector& operator /= (const float p_fVal)
		{
			if (FLOAT_IS_ZERO(p_fVal))
			{
				return *this;
			}

			x /= p_fVal;
			y /= p_fVal;
			z /= p_fVal;
			return *this;
		}
		inline friend fvector operator + (const float p_fVal, const fvector& p_V)
		{
			return fvector(p_fVal + p_V.x, p_fVal + p_V.y, p_fVal + p_V.z);
		}
		inline friend fvector operator - (const float p_fVal, const fvector& p_V)
		{
			return fvector(p_fVal - p_V.x, p_fVal - p_V.y, p_fVal - p_V.z);
		}
		inline friend fvector operator * (const float p_fVal, const fvector& p_V)
		{
			return fvector(p_fVal * p_V.x, p_fVal * p_V.y, p_fVal * p_V.z);
		}
		inline friend fvector operator / (const float p_fVal, const fvector& p_V)
		{
			if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z))
			{
				return fvector(0, 0, 0);
			}
			return fvector(p_fVal / p_V.x, p_fVal / p_V.y, p_fVal / p_V.z);
		}

		inline bool operator != (const fvector& rhs) const
		{
			return (x != rhs.x || y != rhs.y || z != rhs.z);
		}
	};
	struct s_map_pos
	{
		fvector unit_location;
		frotator unit_rotation;
		s_map_pos()
		{
			clear_data();
		}
		s_map_pos(f32 pos_x, f32 pos_y, f32 pos_z, f32 roll, f32 pitch, f32 yaw) : unit_location(pos_x, pos_y, pos_z), unit_rotation(roll, pitch, yaw)
		{

		}
		s_map_pos(fvector location) : unit_location(location)
		{
			unit_rotation.clear_data();
		}
		s_map_pos(fvector location, frotator rotation) : unit_location(location), unit_rotation(rotation)
		{

		}
		bool is_zero()
		{
			return unit_location.is_zero() && unit_rotation.is_zero();
		}
		void clear_data()
		{
			unit_location.clear_data();
			unit_rotation.clear_data();
		}
		void set_location(f32 pos_x, f32 pos_y, f32 pos_z)
		{
			unit_location.x = pos_x;
			unit_location.y = pos_y;
			unit_location.z = pos_z;
		}
		void set_location(fvector location)
		{
			unit_location = location;
		}
		void set_rotation(f32 roll, f32 pitch, f32 yaw)
		{
			unit_rotation.roll = roll;
			unit_rotation.pitch = pitch;
			unit_rotation.yaw = yaw;
		}
		void set_rotation(frotator rotation)
		{
			unit_rotation = rotation;
		}
		s_map_pos& operator= (const s_map_pos& temp_pos)
		{
			unit_location = temp_pos.unit_location;
			unit_rotation = temp_pos.unit_rotation;
			return *this;
		}
		bool operator == (const s_map_pos& temp_pos) const
		{
			return unit_location == temp_pos.unit_location && unit_rotation == temp_pos.unit_rotation;
		}

		bool operator != (const s_map_pos& temp_pos) const
		{
			return unit_location != temp_pos.unit_location || unit_rotation != temp_pos.unit_rotation;
		}
	};
#pragma pack(pop)
}
#endif
