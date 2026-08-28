#ifndef __VECTOR_3D_H_
#define __VECTOR_3D_H_

#include "logic/type_def.hpp"
//#pragma intrinsic(sqrt, pow)

namespace hld
{
    namespace math
    {
        // Return the FRotator corresponding to the direction that the vector
        // is pointing in.  Sets Yaw and Pitch to the proper numbers, and sets
        // roll to zero because the roll can't be determined from a vector.

#undef FATIH_PI
        //! Constant for FATIH_PI.
        const f32 FATIH_PI = 3.14159265359f;

        //! Constant for reciprocal of FATIH_PI.
        const f32 RECIPROCAL_PI = 1.0f / FATIH_PI;

        //! Constant for 64bit FATIH_PI.
        const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;

        //! Constant for 64bit reciprocal of FATIH_PI.
        const f64 RECIPROCAL_PI64 = 1.0 / PI64;

        //! 32bit Constant for converting from degrees to radians
        const f32 DEGTORAD = FATIH_PI / 180.0f;

        //! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
        const f32 RADTODEG = 180.0f / FATIH_PI;

        //! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
        const f64 DEGTORAD64 = PI64 / 180.0;

        //! 64bit constant for converting from radians to degrees
        const f64 RADTODEG64 = 180.0 / PI64;

        template <class T>
        bool equals(const T& t1, const T& t2, const f32 tolerance = 0.00005f)
        {
            return abs(t1 - t2) < tolerance;
        }
    } // end namespace core
}

#endif

