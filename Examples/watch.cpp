#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
#include <ctime>

class Watch : public gck::xGameEngine
{
public:
	Watch()
	{
		sAppName = "Watch";
	}

public:
	short fRadius = 90;
	short fFactor = 12;

	int hours; int minutes; int seconds;
	char x[16]{};

	gck::vi2d center;
	short sWidth = 257;
	short sHeight = 240;
	short thickness = 20;

	float fTargetFrameTime = 1.0f;
	float fAccumulatedTime = 0.0f;

	struct tm now;
	__time64_t long_time;

	bool appCreate() override
	{
		center = gck::vi2d(sWidth / 2, sHeight / 2);
		return true;
	}

	bool appUpdate(float fElapsedTime) override
	{
		if (GetKey(gck::Key::ESCAPE).bPressed)
			return false;
		// manages time
		fAccumulatedTime += fElapsedTime;

		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}
		else // don't do anything this frame
			return true;
		// this point is reached one time each second
		Clear(gck::BLACK, gck::VERY_DARK_GREY, 8);

		_time64(&long_time);
		_localtime64_s(&now, &long_time);

		int hours = now.tm_hour;
		int minutes = now.tm_min;
		int seconds = now.tm_sec;
		// draw background
		DrawCircle(center, fRadius, gck::BLACK, true);
		DrawCircle(center, fRadius + thickness, gck::VERY_DARK_YELLOW, true, thickness);
		DrawCircle(center, fRadius, gck::MAGENTA, false);
		DrawCircle(center, fRadius + thickness, gck::MAGENTA, false);

		for (int i = 1; i <= fFactor; i++)
		{
			std::string si = std::to_string(i);
			gck::vi2d vPointB = GetVector(i, fRadius + 10, -fFactor);
			DrawString(vPointB + center, si, gck::YELLOW, 1);
		}

		int progress = (minutes / 60 * 5);
		// draw lines
		DrawLine(center, GetVector(-hours * 5 - progress, fRadius - 30, 60) + center, gck::GREEN);
		DrawLine(center, GetVector(-minutes, fRadius - 20, 60) + center, gck::BLUE);
		DrawLine(center, GetVector(-seconds, fRadius - 10, 60) + center, gck::RED);
		DrawCircle(center, 5, gck::WHITE, true); // pin

		strftime(x, sizeof(x), "%F", &now);
		std::string xx = x;
		DrawString(gck::vi2d(10, 10), xx, gck::DARK_YELLOW, 1);

		return true;
	}
};

int main()
{       // hide console
        ShowWindow(GetConsoleWindow(), SW_HIDE);
	
	Watch Watch;

	if (Watch.Construct(Watch.sWidth, Watch.sHeight, 2, 2, false, true, false, false))
		Watch.Start();

	return 0;
}
