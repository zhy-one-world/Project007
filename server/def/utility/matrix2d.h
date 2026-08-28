/********************************************************************
	created:	2011/04/26
	created:	26:4:2011   14:35
	file base:	Matrix2D
	file ext:	h
	author:	    wenhongming
	purpose:	2d matrix support on server side
*********************************************************************/

#ifndef MATRIX_2D_H_
#define MATRIX_2D_H_

#include <cmath>
#include "logic/type_def.hpp"
#include "vector3d.h"

namespace hld
{
    namespace math
    {
        template <class T>
        class matrix2d
        {
        private:

            struct matrix
            {

                T _11, _12, _13;
                T _21, _22, _23;
                T _31, _32, _33;

                matrix()
                {
                    _11 = 0; _12 = 0; _13 = 0;
                    _21 = 0; _22 = 0; _23 = 0;
                    _31 = 0; _32 = 0; _33 = 0;
                }

            };

            matrix m_matrix;

            //multiplies m_matrix with mIn
            inline void  matrix_multiply(matrix &mIn)
            {
                matrix2d::matrix mat_temp;

                //first row
                mat_temp._11 = (mIn._11*m_matrix._11) + (mIn._12*m_matrix._21) + (mIn._13*m_matrix._31);
                mat_temp._12 = (mIn._11*m_matrix._12) + (mIn._12*m_matrix._22) + (mIn._13*m_matrix._32);
                mat_temp._13 = (mIn._11*m_matrix._13) + (mIn._12*m_matrix._23) + (mIn._13*m_matrix._33);

                //second
                mat_temp._21 = (mIn._21*m_matrix._11) + (mIn._22*m_matrix._21) + (mIn._23*m_matrix._31);
                mat_temp._22 = (mIn._21*m_matrix._12) + (mIn._22*m_matrix._22) + (mIn._23*m_matrix._32);
                mat_temp._23 = (mIn._21*m_matrix._13) + (mIn._22*m_matrix._23) + (mIn._23*m_matrix._33);

                //third
                mat_temp._31 = (mIn._31*m_matrix._11) + (mIn._32*m_matrix._21) + (mIn._33*m_matrix._31);
                mat_temp._32 = (mIn._31*m_matrix._12) + (mIn._32*m_matrix._22) + (mIn._33*m_matrix._32);
                mat_temp._33 = (mIn._31*m_matrix._13) + (mIn._32*m_matrix._23) + (mIn._33*m_matrix._33);

                m_matrix = mat_temp;
            }

        public:

            matrix2d()
            {
                //initialize the matrix to an identity matrix
                Identity();
            }

            //create an identity matrix
            inline void Identity()
            {
                m_matrix._11 = 1; 
                m_matrix._12 = 0; 
                m_matrix._13 = 0;

                m_matrix._21 = 0; 
                m_matrix._22 = 1; 
                m_matrix._23 = 0;

                m_matrix._31 = 0; 
                m_matrix._32 = 0; 
                m_matrix._33 = 1;
            }

            //create a transformation matrix
            inline void translate(T x, T y)
            {
                matrix mat;
                mat._11 = 1; 
                mat._12 = 0; 
                mat._13 = x;

                mat._21 = 0; 
                mat._22 = 1; 
                mat._23 = y;

                mat._31 = 0;    
                mat._32 = 0;    
                mat._33 = 1;
                //and multiply
                matrix_multiply(mat);
            }

            //create a rotation matrix
            inline void rotate(T rot)
            {
                matrix2d::matrix mat;
                
                T sin_value = std::sin(rot);
                T cos_value = std::cos(rot);

                mat._11 = cos_value; 
                mat._12 = -sin_value; 
                mat._13 = 0;

                mat._21 = sin_value; 
                mat._22 = cos_value; 
                mat._23 = 0;

                mat._31 = 0; 
                mat._32 = 0; 
                mat._33 = 1;

                //and multiply
                matrix_multiply(mat);
            }

            //create a rotation matrix from a heading vector
            template <class TVector>
            inline void rotate(const TVector &heading)
            {
                matrix2d::matrix mat;

                mat._11 = (T)heading.x;  
                mat._12 = -(T)heading.y; 
                mat._13 = 0;

                mat._21 = (T)heading.y; 
                mat._22 = (T)heading.x; 
                mat._23 = 0;

                mat._31 = 0; 
                mat._32 = 0; 
                mat._33 = 1;
                //and multiply
                matrix_multiply(mat);
            }

            //applies a 2D transformation matrix to a single Vector2D
            template <class TVector>
            inline void transform_vector2d(TVector &vPoint)
            {
                T tempX = (m_matrix._11*(T)vPoint.x) + (m_matrix._12*(T)vPoint.y) + (m_matrix._13);
                T tempY = (m_matrix._21*(T)vPoint.x) + (m_matrix._22*(T)vPoint.y) + (m_matrix._23);
                vPoint.x = tempX;
                vPoint.y = tempY;
            }

            //accessors to the matrix elements
            void _11(T val){ m_matrix._11 = val; }
            void _12(T val){ m_matrix._12 = val; }
            void _13(T val){ m_matrix._13 = val; }

            void _21(T val){ m_matrix._21 = val; }
            void _22(T val){ m_matrix._22 = val; }
            void _23(T val){ m_matrix._23 = val; }

            void _31(T val){ m_matrix._31 = val; }
            void _32(T val){ m_matrix._32 = val; }
            void _33(T val){ m_matrix._33 = val; }

        };

        typedef matrix2d<f32> matrix2df;

        //--------------------- PointToWorldSpace --------------------------------
        //
        //  Transforms a point from the agent's local space into world space
        //------------------------------------------------------------------------
        template <class TVector>
        inline vector3df point_to_world_space(const TVector &point, const Rotator &agentRot, const TVector &agentPos)
        {
            //make a copy of the point
            TVector trans_point;
            trans_point.x = point.y;
            trans_point.y = point.x;
            trans_point.z = point.z;

            //create a transformation matrix
            f32 angle = (f32)agentRot.Yaw * 2.0f * math::FATIH_PI / (f32)0xffffU;
            matrix2df matTransform;
            //rotate
            matTransform.rotate(angle);
            //and translate
            matTransform.translate(agentPos.x, agentPos.y);
            //now transform the vertices
            matTransform.transform_vector2D(trans_point);
            return trans_point;
        }
    } // end namespace core
}

#endif
