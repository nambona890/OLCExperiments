#pragma once
#include "common.h"

class Game : public olc::PixelGameEngine
{
private:
	class Mesh;
	class Cube;
private:
	Cube* cube1;
	Cube* cube2;
	float fov = 1.74533f;	// 100 degrees
	Vec3 camPos;
	Vec3 camRot;
	float aspect;			// h/w
	float angle = 0;
	float zNear = 0.1f;
	float zFar = 4000.0f;
	std::vector<Triangle> tList;
public:
	std::vector<float> zBuf;
public:
	Game()
	{
		sAppName = "3D Shit";
	}
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void DrawTris();
	float GetFOV()
	{
		return fov;
	}
	float GetZNear()
	{
		return zNear;
	}
	float GetZFar()
	{
		return zFar;
	}
	float GetAspect()
	{
		return aspect;
	}
};