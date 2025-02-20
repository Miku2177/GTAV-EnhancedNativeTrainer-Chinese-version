#pragma once
#include <math.h>

class Vector3D
{
private:
	// 漂浮 x, y, z;
public:
	float x, y, z;

	// 默认构造函数
	Vector3D(float X = 0, float Y = 0, float Z = 0)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector3D(Vector3 &vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	~Vector3D(){};

	// 计算并返回该向量的模长
	float GetMagnitude()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	// 复制参数
	Vector3D operator=(Vector3D const &vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	// 将该向量乘以一个标量
	Vector3D operator*(float num) const
	{
		return Vector3D(x * num, y * num, z * num);
	}

	// 传入一个向量和一个标量，返回乘积
	friend Vector3D operator*(float num, Vector3D const &vec)
	{
		return Vector3D(vec.x * num, vec.y * num, vec.z * num);
	}

	// 两个向量相加
	Vector3D operator+(const Vector3D &vec) const
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	// 两个向量相减
	Vector3D operator-(const Vector3D &vec) const
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	// 归一化该向量
	void normalizeVector3D()
	{
		float magnitude = sqrtf(x * x + y * y + z * z);
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	// 计算并返回点积
	float dotVector3D(const Vector3D &vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	// 计算并返回叉积
	Vector3D crossVector3D(const Vector3D &vec) const
	{
		return Vector3D(y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x);
	}
};