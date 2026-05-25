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
#include "wwmath.h"

// ---------------------------------------------------------------------------
// WWMath::Fabs
// ---------------------------------------------------------------------------

TEST(WWMathFabs, PositiveReturnsSame)
{
    EXPECT_FLOAT_EQ(WWMath::Fabs(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(WWMath::Fabs(123.456f), 123.456f);
}

TEST(WWMathFabs, NegativeReturnsPositive)
{
    EXPECT_FLOAT_EQ(WWMath::Fabs(-1.0f), 1.0f);
    EXPECT_FLOAT_EQ(WWMath::Fabs(-123.456f), 123.456f);
}

TEST(WWMathFabs, ZeroReturnsZero)
{
    EXPECT_FLOAT_EQ(WWMath::Fabs(0.0f), 0.0f);
}

TEST(WWMathFabs, NegativeZeroReturnsZero)
{
    // -0.0f should become +0.0f (bit-mask strips sign bit)
    float neg_zero = -0.0f;
    EXPECT_FLOAT_EQ(WWMath::Fabs(neg_zero), 0.0f);
}

TEST(WWMathFabs, MatchesStdFabs)
{
    const float values[] = {0.5f, -0.5f, 1e6f, -1e6f, 1e-6f, -1e-6f, WWMATH_FLOAT_MAX};
    for (float v : values)
    {
        EXPECT_FLOAT_EQ(WWMath::Fabs(v), std::fabs(v)) << "input: " << v;
    }
}

// ---------------------------------------------------------------------------
// WWMath::Sqrt
// ---------------------------------------------------------------------------

TEST(WWMathSqrt, KnownValues)
{
    EXPECT_NEAR(WWMath::Sqrt(0.0f),   0.0f,   WWMATH_EPSILON);
    EXPECT_NEAR(WWMath::Sqrt(1.0f),   1.0f,   WWMATH_EPSILON);
    EXPECT_NEAR(WWMath::Sqrt(4.0f),   2.0f,   WWMATH_EPSILON);
    EXPECT_NEAR(WWMath::Sqrt(9.0f),   3.0f,   WWMATH_EPSILON);
    EXPECT_NEAR(WWMath::Sqrt(2.0f),   WWMATH_SQRT2, 1e-4f);
}

// ---------------------------------------------------------------------------
// WWMath::Clamp
// ---------------------------------------------------------------------------

TEST(WWMathClamp, BelowMinClampsToMin)
{
    EXPECT_FLOAT_EQ(WWMath::Clamp(-1.0f, 0.0f, 1.0f), 0.0f);
}

TEST(WWMathClamp, AboveMaxClampsToMax)
{
    EXPECT_FLOAT_EQ(WWMath::Clamp(2.0f, 0.0f, 1.0f), 1.0f);
}

TEST(WWMathClamp, WithinRangeUnchanged)
{
    EXPECT_FLOAT_EQ(WWMath::Clamp(0.5f, 0.0f, 1.0f), 0.5f);
}

TEST(WWMathClamp, AtBoundaryUnchanged)
{
    EXPECT_FLOAT_EQ(WWMath::Clamp(0.0f, 0.0f, 1.0f), 0.0f);
    EXPECT_FLOAT_EQ(WWMath::Clamp(1.0f, 0.0f, 1.0f), 1.0f);
}

// ---------------------------------------------------------------------------
// WWMath::Min / Max
// ---------------------------------------------------------------------------

TEST(WWMathMinMax, MinReturnsSmaller)
{
    EXPECT_FLOAT_EQ(WWMath::Min(3.0f, 5.0f), 3.0f);
    EXPECT_FLOAT_EQ(WWMath::Min(-2.0f, -1.0f), -2.0f);
}

TEST(WWMathMinMax, MaxReturnsLarger)
{
    EXPECT_FLOAT_EQ(WWMath::Max(3.0f, 5.0f), 5.0f);
    EXPECT_FLOAT_EQ(WWMath::Max(-2.0f, -1.0f), -1.0f);
}

TEST(WWMathMinMax, EqualValues)
{
    EXPECT_FLOAT_EQ(WWMath::Min(4.0f, 4.0f), 4.0f);
    EXPECT_FLOAT_EQ(WWMath::Max(4.0f, 4.0f), 4.0f);
}

// ---------------------------------------------------------------------------
// WWMath::Lerp
// ---------------------------------------------------------------------------

TEST(WWMathLerp, AtZeroReturnsA)
{
    EXPECT_FLOAT_EQ(WWMath::Lerp(0.0f, 10.0f, 0.0f), 0.0f);
}

TEST(WWMathLerp, AtOneReturnsB)
{
    EXPECT_FLOAT_EQ(WWMath::Lerp(0.0f, 10.0f, 1.0f), 10.0f);
}

TEST(WWMathLerp, AtHalfReturnsMidpoint)
{
    EXPECT_FLOAT_EQ(WWMath::Lerp(0.0f, 10.0f, 0.5f), 5.0f);
}

// ---------------------------------------------------------------------------
// WWMath::Is_Power_Of_2
// ---------------------------------------------------------------------------

TEST(WWMathIsPowerOf2, PowersOfTwo)
{
    EXPECT_TRUE(WWMath::Is_Power_Of_2(1));
    EXPECT_TRUE(WWMath::Is_Power_Of_2(2));
    EXPECT_TRUE(WWMath::Is_Power_Of_2(4));
    EXPECT_TRUE(WWMath::Is_Power_Of_2(256));
    EXPECT_TRUE(WWMath::Is_Power_Of_2(1024));
    EXPECT_TRUE(WWMath::Is_Power_Of_2(65536));
}

TEST(WWMathIsPowerOf2, NonPowersOfTwo)
{
    EXPECT_FALSE(WWMath::Is_Power_Of_2(0));
    EXPECT_FALSE(WWMath::Is_Power_Of_2(3));
    EXPECT_FALSE(WWMath::Is_Power_Of_2(5));
    EXPECT_FALSE(WWMath::Is_Power_Of_2(100));
}

// ---------------------------------------------------------------------------
// WWMath::Atan2
// ---------------------------------------------------------------------------

TEST(WWMathAtan2, KnownAngles)
{
    EXPECT_NEAR(WWMath::Atan2(0.0f, 1.0f),  0.0f,          WWMATH_EPSILON);
    EXPECT_NEAR(WWMath::Atan2(1.0f, 0.0f),  WWMATH_PI/2,   1e-4f);
    EXPECT_NEAR(WWMath::Atan2(0.0f, -1.0f), WWMATH_PI,     1e-4f);
    EXPECT_NEAR(WWMath::Atan2(-1.0f, 0.0f), -WWMATH_PI/2,  1e-4f);
}
