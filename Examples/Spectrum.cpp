#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
// screen
gck::Sprite* board = nullptr;
char message[29]{ "Color Code r:255 g:255 b:255" };
gck::Pixel selectedPixel = gck::WHITE;
float hue, saturation, brightness;
// for info
bool displayInfo = true;
bool oneTime = false;

class Example : public gck::xGameEngine
{
public:
	Example()
	{
		sAppName = "Full Color Spectrum by HSV";
	}

private:
	std::string convertToString(char* a, int size)
	{
		int i;
		std::string s = "";

		for (i = 0; i < size; i++)
			s = s + a[i];

		return s;
	}

public:
	bool appCreate() override
	{
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y <= ScreenHeight() / 2; y++)
			{
				hue = (float(x) / float(ScreenWidth())) * 360; // angle
				saturation = (float(y) / float(ScreenHeight()) * 100) * 2;
				brightness = 100; // in the middle of the screen, brightness is full

				Draw(gck::vi2d(x, y), HSVtoRGB(hue, saturation, brightness)); // starting from the top
				Draw(gck::vi2d(x, ScreenHeight() - y), HSVtoRGB(hue, saturation, (float(y) / float(ScreenHeight() / 2) * brightness))); // the bottom
			}
		// full board as target
		if (board == nullptr)
			board = GetDrawTarget();

		oneTime = false;
		return true;
	}

	gck::Pixel HSVtoRGB(float H, float S, float V)
	{
		float s = S / 100;
		float v = V / 100;
		float C = s * v;
		float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
		float m = v - C;
		float r, g, b;

		if (H >= 0 && H < 60)
			r = C, g = X, b = 0;
		else if (H >= 60 && H < 120)
			r = X, g = C, b = 0;
		else if (H >= 120 && H < 180)
			r = 0, g = C, b = X;
		else if (H >= 180 && H < 240)
			r = 0, g = X, b = C;
		else if (H >= 240 && H < 300)
			r = X, g = 0, b = C;
		else
			r = C, g = 0, b = X;

		int R = (r + m) * 255;
		int G = (g + m) * 255;
		int B = (b + m) * 255;

		return gck::Pixel(R, G, B);
	}

	bool appUpdate(float fElapsedTime) override
	{
		if (GetKey(gck::Key::ESCAPE).bPressed)
			return false;
		if (GetKey(gck::Key::SPACE).bPressed)
			displayInfo = !displayInfo;
		// mouse control
		if (GetMouse(0).bPressed)
			selectedPixel = board->GetPixel(gck::vi2d(GetMouseX(), GetMouseY()));

		if (selectedPixel != gck::WHITE)
			snprintf(message, sizeof(message), "Color Code r:%03d g:%03d b:%03d", selectedPixel.r, selectedPixel.g, selectedPixel.b);

		if (displayInfo)
		{
			DrawRect(gck::vi2d(ScreenWidth() / 2, ScreenHeight() - 5), ScreenWidth(), 10, NULL, gck::BLACK, true);
			std::string info = convertToString(message, 28);
			DrawString(gck::vi2d(5, ScreenHeight() - 5), info, selectedPixel, 1);
			oneTime = true;
		}
		else
			if (oneTime)
				appCreate();

		return true;
	}
};

std::pair<int, int> GetDesktopResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return std::make_pair(desktop.right, desktop.bottom);
}

int main()
{
	int pixel = 2; // pixel filter
	std::pair<int, int> r = GetDesktopResolution();
	// set here fullscreen option
	bool fullscreen = false;

	if (!fullscreen)
	{
		r.first /= pixel;
		r.second /= pixel;
	}

	Example demo;

	if (demo.Construct(r.first / pixel, r.second / pixel, pixel, pixel, fullscreen, true, true, false))
		demo.Start();

	return 0;
}
