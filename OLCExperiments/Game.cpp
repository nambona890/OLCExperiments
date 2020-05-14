#include "Game.h"
#include "Cube.h"
#include "Slope.h"

bool Game::OnUserCreate()
{
	zBuf.resize(ScreenWidth() * ScreenHeight());
	aspect = (float)ScreenHeight() / (float)ScreenWidth();
	cube1 = new Cube();
	cube2 = new Cube();
	cube1->translation.x = -1.8f;
	cube2->translation.x = 1.8f;
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	tList.clear();
	std::fill(zBuf.begin(), zBuf.end(), zFar);
	Clear(olc::BLACK);
	cube1->rotation.y += fElapsedTime;
	cube1->rotation.x += fElapsedTime * 0.5f;

	cube1->Draw(*this);

	cube2->rotation.y -= fElapsedTime;
	cube2->rotation.x -= fElapsedTime * 0.3f;

	cube2->Draw(*this);

	DrawTris();

	return true;
}

void Game::DrawTris()
{
	std::sort(tList.begin(), tList.end(), [](Triangle a, Triangle b)
	{
		return (a.v[0].z + a.v[1].z + a.v[2].z) / 3.0f > (b.v[0].z + b.v[1].z + b.v[2].z) / 3.0f;
	});
	for (auto t : tList)
	{
		//FillTriangle(t.v[0].x, t.v[0].y, t.v[1].x, t.v[1].y, t.v[2].x, t.v[2].y, t.c);
		std::sort(std::begin(t.v), std::end(t.v), [](Vec3 a, Vec3 b) {
			return a.y < b.y;
		});
		if ((int)t.v[0].y < (int)t.v[2].y)
		{
			/*for (auto& v : t.v)
			{
				v.z = 1.0f / v.z;
			}*/
			Slope xL(t.v[0].x, t.v[2].x, (int)std::abs(t.v[2].y - t.v[0].y));
			Slope zL(t.v[0].z, t.v[2].z, (int)std::abs(t.v[2].y - t.v[0].y));
			bool bL = (t.v[1].y - t.v[0].y) * (t.v[2].x - t.v[0].x) > (t.v[1].x - t.v[0].x)* (t.v[2].y - t.v[0].y);
			float y = t.v[0].y;
			if ((int)t.v[0].y < (int)t.v[1].y)
			{
				Slope xS1(t.v[0].x, t.v[1].x, (int)std::abs(t.v[1].y - t.v[0].y));
				Slope zS1(t.v[0].z, t.v[1].z, (int)std::abs(t.v[1].y - t.v[0].y));
				while (y < t.v[1].y)
				{
					Slope zC(zS1.Get(), zL.Get(), (int)std::abs(bL ? xL.Get() - xS1.Get() : xS1.Get() - xL.Get()));
					for (int x = xS1.Get(); bL ? x < xL.Get() : x >= xL.Get(); bL ? x++ : x--)
					{
						if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight())
						{
							if (zC.Get() < zBuf[x + (ScreenWidth() * y)])
							{
								Draw(x, y, t.c);
								zBuf[x + (ScreenWidth() * y)] = zC.Get();
							}
						}
						zC.Step();
					}
					y += 1.0f;
					xL.Step();
					xS1.Step();
					zL.Step();
					zS1.Step();
				}
			}
			if ((int)t.v[1].y < (int)t.v[2].y)
			{
				Slope xS2(t.v[1].x, t.v[2].x, (int)std::abs(t.v[2].y - t.v[1].y));
				Slope zS2(t.v[1].z, t.v[2].z, (int)std::abs(t.v[2].y - t.v[1].y));
				while (y < t.v[2].y)
				{
					Slope zC(zS2.Get(), zL.Get(), (int)std::abs(bL ? xL.Get() - xS2.Get() : xS2.Get() - xL.Get()));
					for (int x = xS2.Get(); bL ? x < xL.Get() : x >= xL.Get(); bL ? x++ : x--)
					{
						if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight())
						{
							if (zC.Get() < zBuf[x + (ScreenWidth() * y)])
							{
								Draw(x, y, t.c);
								zBuf[x + (ScreenWidth() * y)] = zC.Get();
							}
						}
						zC.Step();
					}
					y += 1.0f;
					xL.Step();
					xS2.Step();
					zL.Step();
					zS2.Step();
				}
			}
		}
	}
}
