#pragma once
#include <cmath>
#include "common.h"

class Mat4x4
{
public:
	float m[4][4] = { 0 };
public:
	static Mat4x4 Multiply(const Mat4x4& m1, const Mat4x4& m2)
	{
		Mat4x4 ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ret.m[i][j] = (m1.m[i][0] * m2.m[0][j]) + (m1.m[i][1] * m2.m[1][j]) + (m1.m[i][2] * m2.m[2][j]) + (m1.m[i][3] * m2.m[3][j]);
			}
		}
		return ret;
	}
	static Mat4x4 RotateX(const float theta)
	{
		Mat4x4 ret;
		ret.m[0][0] = 1;

		ret.m[1][1] = cosf(theta);
		ret.m[1][2] = sinf(theta);
		ret.m[2][1] = -sinf(theta);
		ret.m[2][2] = cosf(theta);

		ret.m[3][3] = 1;
		return ret;
	}
	static Mat4x4 RotateY(const float theta)
	{
		Mat4x4 ret;
		ret.m[1][1] = 1;

		ret.m[0][0] = cosf(theta);
		ret.m[0][2] = -sinf(theta);
		ret.m[2][0] = sinf(theta);
		ret.m[2][2] = cosf(theta);

		ret.m[3][3] = 1;
		return ret;
	}
	static Mat4x4 RotateZ(const float theta)
	{
		Mat4x4 ret;
		ret.m[2][2] = 1;

		ret.m[0][0] = cosf(theta);
		ret.m[0][1] = sinf(theta);
		ret.m[1][0] = -sinf(theta);
		ret.m[1][1] = cosf(theta);

		ret.m[3][3] = 1;
		return ret;
	}
	static Mat4x4 RotateXYZ(const Vec3& theta)
	{
		Mat4x4 rX = RotateX(theta.x);
		Mat4x4 rY = RotateY(theta.y);
		Mat4x4 rZ = RotateZ(theta.z);

		Mat4x4 rXY = Multiply(rX, rY);

		Mat4x4 rXYZ = Multiply(rXY, rZ);
		return rXYZ;
	}
	static Mat4x4 Translate(const Vec3& xyz)
	{
		Mat4x4 ret;
		ret.m[0][0] = 1.0f;
		ret.m[1][1] = 1.0f;
		ret.m[2][2] = 1.0f;
		ret.m[3][0] = xyz.x;
		ret.m[3][1] = xyz.y;
		ret.m[3][2] = xyz.z;
		return ret;
	}
	Vec3 Transform(const Vec3& input)
	{
		Vec3 prime;
		prime.x = (m[0][0] * input.x) + (m[1][0] * input.y) + (m[2][0] * input.z) + m[3][0];
		prime.y = (m[0][1] * input.x) + (m[1][1] * input.y) + (m[2][1] * input.z) + m[3][1];
		prime.z = (m[0][2] * input.x) + (m[1][2] * input.y) + (m[2][2] * input.z) + m[3][2];
		float w = (m[0][3] * input.x) + (m[1][3] * input.y) + (m[2][3] * input.z) + m[3][3];
		if (w != 0.0f)
		{
			prime.x /= w;
			prime.y /= w;
			prime.z /= w;
		}
		return prime;
	}
	Triangle Transform(const Triangle& input)
	{
		Triangle prime;
		prime.v[0] = Transform(input.v[0]);
		prime.v[1] = Transform(input.v[1]);
		prime.v[2] = Transform(input.v[2]);
		return prime;
	}
	static Mat4x4 Perspective(const float fov, const float zfar, const float znear, const float aspect)
	{
		Mat4x4 ret;
		float s = 1.0f / tanf(fov / 2.0f);
		float q = -zfar / (zfar - znear);
		ret.m[0][0] = s * aspect;
		ret.m[1][1] = s;
		ret.m[2][2] = q;
		ret.m[3][2] = q*znear;
		ret.m[2][3] = 1.0f;
		return ret;
	}
};