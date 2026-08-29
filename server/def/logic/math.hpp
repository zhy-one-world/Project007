// vecrot.hpp - extracted frotator, fvector, s_map_pos
#pragma once
#include <cmath>
#include <vector>
#include "type_def.hpp"

namespace faith
{
    struct frotator
    {
        frotator() { clear_data(); }
        frotator(f32 _roll, f32 _pitch, f32 _yaw) : roll(_roll), pitch(_pitch), yaw(_yaw) {}
        union { f32 roll; f32 Roll; };
        union { f32 pitch; f32 Pitch; };
        union { f32 yaw; f32 Yaw; };
        frotator& operator= (const frotator& temp_pos)
        {
            roll = temp_pos.roll; pitch = temp_pos.pitch; yaw = temp_pos.yaw; return *this;
        }
        inline bool operator == (const frotator& p_V) const
        {
            return FLOAT_IS_ZERO(roll - p_V.roll) && FLOAT_IS_ZERO(pitch - p_V.pitch) && FLOAT_IS_ZERO(yaw - p_V.yaw);
        }
        inline bool operator != (const frotator& p_V) const
        {
            return !FLOAT_IS_ZERO(roll - p_V.roll) || !FLOAT_IS_ZERO(pitch - p_V.pitch) || !FLOAT_IS_ZERO(yaw - p_V.yaw);
        }
        void clear_data() { roll = pitch = yaw = 0; }
        bool is_zero() { return FLOAT_IS_ZERO(roll) && FLOAT_IS_ZERO(pitch) && FLOAT_IS_ZERO(yaw); }
    };

    struct fvector
    {
        fvector() { x = y = z = 0; }
        fvector(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}
        fvector(f32 _x, f32 _y) : x(_x), y(_y), z(0) {}
        union { f32 x; f32 X; };
        union { f32 y; f32 Y; };
        union { f32 z; f32 Z; };

        frotator rotator()
        {
            frotator r; r.Roll = 0; r.Pitch = atan2f(Z, sqrt(X * X + Y * Y)) * ANGLE; r.Yaw = atan2f(Y, X) * ANGLE; return r;
        }
        fvector& operator= (const fvector& temp_pos) { x = temp_pos.x; y = temp_pos.y; z = temp_pos.z; return *this; }
        void clear_data() { x = y = z = 0; }
        void set_data(float temp_x, float temp_y, float temp_z) { x = temp_x; y = temp_y; z = temp_z; }
        void set_data(std::vector<float>& location) { if (location.size() >= 3) { x = location[0]; y = location[1]; z = location[2]; } }
        void set_data(std::vector<int32>& location) { if (location.size() >= 3) { x = (float)location[0]; y = (float)location[1]; z = (float)location[2]; } }
        inline f32 length_sql() { return x * x + y * y + z * z; }
        inline f32 lenth_sql_2d() { return x * x + y * y; }
        inline f32 length() { return sqrt(length_sql()); }
        inline f32 length_2d() { return sqrt(lenth_sql_2d()); }
        inline f32 squared_distance(const fvector& rhs) const { return (*this - rhs).length_sql(); }
        inline f32 squared_distance_2d(const fvector& rhs) const { return (*this - rhs).lenth_sql_2d(); }
        inline f32 nomalize() { float l_fLen = length(); if (!FLOAT_IS_ZERO(l_fLen)) { x /= l_fLen; y /= l_fLen; z /= l_fLen; } return l_fLen; }
        inline fvector get_safe_normal(float tolerance = 1.e-8f) const
        {
            const float square_sum = x * x + y * y + z * z;
            if (square_sum == 1.f) return *this;
            else if (square_sum < tolerance) return fvector(0,0,0);
            float square_num = sqrt(square_sum);
            if (FLOAT_IS_ZERO(square_num)) return fvector(0,0,0);
            const float scale = 1.0f / square_num; return fvector(x * scale, y * scale, z * scale);
        }
        inline bool is_valid() { return is_zero() == false; }
        bool is_zero() { return FLOAT_IS_ZERO(x) && FLOAT_IS_ZERO(y) && FLOAT_IS_ZERO(z); }
        inline const fvector& operator +() const { return *this; }
        inline fvector operator - () const { return fvector(-x, -y, -z); }
        inline bool operator == (const fvector& p_V) const { return FLOAT_IS_ZERO(x - p_V.x) && FLOAT_IS_ZERO(y - p_V.y) && FLOAT_IS_ZERO(z - p_V.z); }
        inline fvector operator + (const fvector& p_V) const { return fvector(x + p_V.x, y + p_V.y, z + p_V.z); }
        inline fvector& operator += (const fvector& p_V) { x += p_V.x; y += p_V.y; z += p_V.z; return *this; }
        inline fvector operator + (const float p_fVal) const { return fvector(x + p_fVal, y + p_fVal, z + p_fVal); }
        inline fvector& operator += (const float p_fVal) { x += p_fVal; y += p_fVal; z += p_fVal; return *this; }
        inline fvector operator - (const fvector& p_V) const { return fvector(x - p_V.x, y - p_V.y, z - p_V.z); }
        inline fvector& operator -= (const fvector& p_V) { x -= p_V.x; y -= p_V.y; z -= p_V.z; return *this; }
        inline fvector operator - (const float p_fVal) { return fvector(x - p_fVal, y - p_fVal, z - p_fVal); }
        inline fvector& operator -= (const float p_fVal) { x -= p_fVal; y -= p_fVal; z -= p_fVal; return *this; }
        inline fvector operator * (const fvector& p_V) const { return fvector(x * p_V.x, y * p_V.y, z * p_V.z); }
        inline fvector& operator *= (const fvector& p_V) { x *= p_V.x; y *= p_V.y; z *= p_V.z; return *this; }
        inline fvector operator * (const float p_fVal) const { return fvector(x * p_fVal, y * p_fVal, z * p_fVal); }
        inline fvector& operator *= (const float p_fVal) { x *= p_fVal; y *= p_fVal; z *= p_fVal; return *this; }
        inline fvector operator / (const fvector& p_V) const { if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z)) return fvector(0,0,0); return fvector(x / p_V.x, y / p_V.y, z / p_V.z); }
        inline fvector& operator /= (const fvector& p_V) { if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z)) return *this; x /= p_V.x; y /= p_V.y; z /= p_V.z; return *this; }
        inline fvector operator / (const float p_fVal) const { if (FLOAT_IS_ZERO(p_fVal)) return fvector(0,0,0); return fvector(x / p_fVal, y / p_fVal, z / p_fVal); }
        inline fvector& operator /= (const float p_fVal) { if (FLOAT_IS_ZERO(p_fVal)) return *this; x /= p_fVal; y /= p_fVal; z /= p_fVal; return *this; }
        inline friend fvector operator + (const float p_fVal, const fvector& p_V) { return fvector(p_fVal + p_V.x, p_fVal + p_V.y, p_fVal + p_V.z); }
        inline friend fvector operator - (const float p_fVal, const fvector& p_V) { return fvector(p_fVal - p_V.x, p_fVal - p_V.y, p_fVal - p_V.z); }
        inline friend fvector operator * (const float p_fVal, const fvector& p_V) { return fvector(p_fVal * p_V.x, p_fVal * p_V.y, p_fVal * p_V.z); }
        inline friend fvector operator / (const float p_fVal, const fvector& p_V) { if (FLOAT_IS_ZERO(p_V.x) || FLOAT_IS_ZERO(p_V.y) || FLOAT_IS_ZERO(p_V.z)) return fvector(0,0,0); return fvector(p_fVal / p_V.x, p_fVal / p_V.y, p_fVal / p_V.z); }
        inline bool operator != (const fvector& rhs) const { return (x != rhs.x || y != rhs.y || z != rhs.z); }
    };

    struct s_map_pos
    {
        fvector unit_location;
        frotator unit_rotation;
        s_map_pos() { clear_data(); }
        s_map_pos(f32 pos_x, f32 pos_y, f32 pos_z, f32 roll, f32 pitch, f32 yaw) : unit_location(pos_x, pos_y, pos_z), unit_rotation(roll, pitch, yaw) {}
        s_map_pos(fvector location) : unit_location(location) { unit_rotation.clear_data(); }
        s_map_pos(fvector location, frotator rotation) : unit_location(location), unit_rotation(rotation) {}
        bool is_zero() { return unit_location.is_zero() && unit_rotation.is_zero(); }
        void clear_data() { unit_location.clear_data(); unit_rotation.clear_data(); }
        void set_location(f32 pos_x, f32 pos_y, f32 pos_z) { unit_location.x = pos_x; unit_location.y = pos_y; unit_location.z = pos_z; }
        void set_location(fvector location) { unit_location = location; }
        void set_rotation(f32 roll, f32 pitch, f32 yaw) { unit_rotation.roll = roll; unit_rotation.pitch = pitch; unit_rotation.yaw = yaw; }
        void set_rotation(frotator rotation) { unit_rotation = rotation; }
        s_map_pos& operator= (const s_map_pos& temp_pos) { unit_location = temp_pos.unit_location; unit_rotation = temp_pos.unit_rotation; return *this; }
        bool operator == (const s_map_pos& temp_pos) const { return unit_location == temp_pos.unit_location && unit_rotation == temp_pos.unit_rotation; }
        bool operator != (const s_map_pos& temp_pos) const { return unit_location != temp_pos.unit_location || unit_rotation != temp_pos.unit_rotation; }
    };
}
