/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include <cmath>

#include "Lib/BaseType.h"
#include "Common/GameMemory.h"
#include "vector3.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static void ExpectV3Near(const Vector3 &v, float x, float y, float z, float tol = 1e-5f)
{
    EXPECT_NEAR(v.X, x, tol);
    EXPECT_NEAR(v.Y, y, tol);
    EXPECT_NEAR(v.Z, z, tol);
}

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST(Vector3Ctor, ComponentConstruction)
{
    Vector3 v(1.0f, 2.0f, 3.0f);
    EXPECT_FLOAT_EQ(v.X, 1.0f);
    EXPECT_FLOAT_EQ(v.Y, 2.0f);
    EXPECT_FLOAT_EQ(v.Z, 3.0f);
}

TEST(Vector3Ctor, CopyConstruction)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(a);
    EXPECT_FLOAT_EQ(b.X, 4.0f);
    EXPECT_FLOAT_EQ(b.Y, 5.0f);
    EXPECT_FLOAT_EQ(b.Z, 6.0f);
}

TEST(Vector3Ctor, ArrayConstruction)
{
    float arr[3] = {7.0f, 8.0f, 9.0f};
    Vector3 v(arr);
    EXPECT_FLOAT_EQ(v.X, 7.0f);
    EXPECT_FLOAT_EQ(v.Y, 8.0f);
    EXPECT_FLOAT_EQ(v.Z, 9.0f);
}

// ---------------------------------------------------------------------------
// Length and Length2
// ---------------------------------------------------------------------------

TEST(Vector3Length, UnitAxisX)
{
    Vector3 v(1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
    EXPECT_FLOAT_EQ(v.Length2(), 1.0f);
}

TEST(Vector3Length, UnitAxisY)
{
    Vector3 v(0.0f, 1.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

TEST(Vector3Length, UnitAxisZ)
{
    Vector3 v(0.0f, 0.0f, 1.0f);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

TEST(Vector3Length, KnownLength)
{
    // 3-4-5 right triangle in 3D
    Vector3 v(3.0f, 4.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.Length2(), 25.0f);
    EXPECT_NEAR(v.Length(), 5.0f, 1e-5f);
}

TEST(Vector3Length, ZeroVector)
{
    Vector3 v(0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.Length(), 0.0f);
    EXPECT_FLOAT_EQ(v.Length2(), 0.0f);
}

// ---------------------------------------------------------------------------
// Arithmetic operators
// ---------------------------------------------------------------------------

TEST(Vector3Arithmetic, Addition)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a + b;
    ExpectV3Near(c, 5.0f, 7.0f, 9.0f);
}

TEST(Vector3Arithmetic, Subtraction)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c = a - b;
    ExpectV3Near(c, 3.0f, 3.0f, 3.0f);
}

TEST(Vector3Arithmetic, ScalarMultiply)
{
    Vector3 v(1.0f, 2.0f, 3.0f);
    Vector3 r = v * 2.0f;
    ExpectV3Near(r, 2.0f, 4.0f, 6.0f);
}

TEST(Vector3Arithmetic, Negate)
{
    Vector3 v(1.0f, -2.0f, 3.0f);
    Vector3 r = -v;
    ExpectV3Near(r, -1.0f, 2.0f, -3.0f);
}

// ---------------------------------------------------------------------------
// Dot product
// ---------------------------------------------------------------------------

TEST(Vector3DotProduct, Perpendicular)
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    EXPECT_FLOAT_EQ(Vector3::Dot_Product(a, b), 0.0f);
}

TEST(Vector3DotProduct, Parallel)
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(Vector3::Dot_Product(a, b), 1.0f);
}

TEST(Vector3DotProduct, AntiParallel)
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(-1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(Vector3::Dot_Product(a, b), -1.0f);
}

TEST(Vector3DotProduct, General)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    EXPECT_FLOAT_EQ(Vector3::Dot_Product(a, b), 32.0f);
}

// ---------------------------------------------------------------------------
// Cross product
// ---------------------------------------------------------------------------

TEST(Vector3CrossProduct, StandardBasis_XxY_is_Z)
{
    Vector3 x(1.0f, 0.0f, 0.0f);
    Vector3 y(0.0f, 1.0f, 0.0f);
    Vector3 z;
    Vector3::Cross_Product(x, y, &z);
    ExpectV3Near(z, 0.0f, 0.0f, 1.0f);
}

TEST(Vector3CrossProduct, StandardBasis_YxZ_is_X)
{
    Vector3 y(0.0f, 1.0f, 0.0f);
    Vector3 z(0.0f, 0.0f, 1.0f);
    Vector3 x;
    Vector3::Cross_Product(y, z, &x);
    ExpectV3Near(x, 1.0f, 0.0f, 0.0f);
}

TEST(Vector3CrossProduct, ParallelVectorsGiveZero)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 4.0f, 6.0f);
    Vector3 c;
    Vector3::Cross_Product(a, b, &c);
    ExpectV3Near(c, 0.0f, 0.0f, 0.0f, 1e-4f);
}

TEST(Vector3CrossProduct, AntiCommutative)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 ab;
    Vector3 ba;
    Vector3::Cross_Product(a, b, &ab);
    Vector3::Cross_Product(b, a, &ba);
    ExpectV3Near(ab + ba, 0.0f, 0.0f, 0.0f, 1e-5f);
}

// ---------------------------------------------------------------------------
// Normalize
// ---------------------------------------------------------------------------

TEST(Vector3Normalize, UnitLengthAfterNormalize)
{
    Vector3 v(3.0f, 4.0f, 0.0f);
    v.Normalize();
    EXPECT_NEAR(v.Length(), 1.0f, 1e-5f);
}

TEST(Vector3Normalize, DirectionPreserved)
{
    Vector3 v(3.0f, 0.0f, 0.0f);
    v.Normalize();
    ExpectV3Near(v, 1.0f, 0.0f, 0.0f);
}

TEST(Vector3Normalize, DiagonalVector)
{
    // (1,1,0) normalized should be (sqrt(2)/2, sqrt(2)/2, 0)
    Vector3 v(1.0f, 1.0f, 0.0f);
    v.Normalize();
    EXPECT_NEAR(v.X, WWMATH_OOSQRT2, 1e-5f);
    EXPECT_NEAR(v.Y, WWMATH_OOSQRT2, 1e-5f);
    EXPECT_NEAR(v.Z, 0.0f, 1e-5f);
}
