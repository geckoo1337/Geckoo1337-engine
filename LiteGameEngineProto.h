/* 
* The Geckoo Game Engine is a little framework written in a single header file
* mostly inspired by the amazing OLC Pixel Game Engine (1.v) developed by Javixd9.
* This is a minimalist way to create applications, games and prototypes quickly.
* The Geckoo Game Engine has been released with some harsh limitations, but it
* allows you to draw many polygons, shapes and text strings, setting app features...
* It has its own font based on a retro style (7x5 pixels). Can be modified easily.
* 1337geckoo_dot_gmail.com for comments and/or questions. Have fun ++
*/
#ifndef GECKOO_ENGINE_DEF
#define GECKOO_ENGINE_DEF
// visual studio libraries embedded
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "opengl32.lib")
// includes
#include <windows.h>
#include <gdiplus.h>
#include <GL/gl.h> // OpenGL extension
#include <string>
#include <condition_variable>
#include <map>
#include <functional>
#include <sstream> // for string precision
#include <random> // for rndGenerator
// shut up!
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
// for VSYNC option
typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t* wglSwapInterval;

namespace gck
{	// static template in order to randomize an integer or a float according to min/max values
	template <typename T>
	static T rndGenerator(T min, T max)
	{   // avoid a crash if min is greater than max
		if (min > max) std::swap(min, max);

		static std::random_device seeder;
		static std::mt19937 gen(seeder());

		typename std::conditional<std::is_integral<T>::value,
			std::uniform_int_distribution<T>, // for integral
			std::uniform_real_distribution<T> // for real numbers
		>::type distribution(min, max);

		return distribution(gen);
	};
	// pixel structure
	struct Pixel
	{
		union
		{	// opaque black pixel
			uint32_t n = 0xFF000000;
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a; // alpha
			};
		};

		Pixel(); // constructor
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

		bool operator==(const Pixel& p) const;
		bool operator!=(const Pixel& p) const;
	};
	// return code
	enum rc { FAIL = 0, OK = 1 };
	const float PI = static_cast<float>(acos(-1)); // 3.14159265f
	// constants for pixel color - create here your color palette
	static const Pixel
		BLANK(0, 0, 0, 0), WHITE(255, 255, 255), BLACK(0, 0, 0),
		GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
		RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
		YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
		GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
		CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
		BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
		MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64);
	// minimalist font - numeral
	std::vector<unsigned char> num0{ 0x7F, 0x41, 0x49, 0x41, 0x7F }; 
	std::vector<unsigned char> num1{ 0x41, 0x41, 0x7F, 0x40, 0x40 };
	std::vector<unsigned char> num2{ 0x79, 0x49, 0x49, 0x49, 0x4F }; 
	std::vector<unsigned char> num3{ 0x41, 0x49, 0x49, 0x49, 0x7F };
	std::vector<unsigned char> num4{ 0x0F, 0x08, 0x08, 0x08, 0x7F }; 
	std::vector<unsigned char> num5{ 0x4F, 0x49, 0x49, 0x49, 0x79 };
	std::vector<unsigned char> num6{ 0x7F, 0x49, 0x49, 0x49, 0x79 }; 
	std::vector<unsigned char> num7{ 0x01, 0x01, 0x01, 0x01, 0x7F };
	std::vector<unsigned char> num8{ 0x7F, 0x49, 0x49, 0x49, 0x7F }; 
	std::vector<unsigned char> num9{ 0x4F, 0x49, 0x49, 0x49, 0x7F };
	// alpha
	std::vector<unsigned char> keyA{ 0x7E, 0x09, 0x09, 0x09, 0x7E }; 
	std::vector<unsigned char> keyB{ 0x7F, 0x49, 0x49, 0x49, 0x36 };
	std::vector<unsigned char> keyC{ 0x3E, 0x41, 0x41, 0x41, 0x22 }; 
	std::vector<unsigned char> keyD{ 0x41, 0x7F, 0x41, 0x41, 0x3E };
	std::vector<unsigned char> keyE{ 0x7F, 0x49, 0x49, 0x49, 0x41 }; 
	std::vector<unsigned char> keyF{ 0x7F, 0x09, 0x09, 0x09, 0x01 };
	std::vector<unsigned char> keyG{ 0x3E, 0x41, 0x41, 0x51, 0x32 }; 
	std::vector<unsigned char> keyH{ 0x7F, 0x08, 0x08, 0x08, 0x7F };
	std::vector<unsigned char> keyI{ 0x41, 0x41, 0x7F, 0x41, 0x41 }; 
	std::vector<unsigned char> keyJ{ 0x20, 0x40, 0x40, 0x40, 0x3F };
	std::vector<unsigned char> keyK{ 0x7F, 0x08, 0x14, 0x22, 0x41 }; 
	std::vector<unsigned char> keyL{ 0x7F, 0x40, 0x40, 0x40, 0x40 };
	std::vector<unsigned char> keyM{ 0x7F, 0x02, 0x0C, 0x02, 0x7F }; 
	std::vector<unsigned char> keyN{ 0x7F, 0x04, 0x08, 0x10, 0x7F };
	std::vector<unsigned char> keyO{ 0x3E, 0x41, 0x41, 0x41, 0x3E }; 
	std::vector<unsigned char> keyP{ 0x7F, 0x09, 0x09, 0x09, 0x06 };
	std::vector<unsigned char> keyQ{ 0x3E, 0x41, 0x71, 0x41, 0x3E }; 
	std::vector<unsigned char> keyR{ 0x7F, 0x09, 0x19, 0x29, 0x46 };
	std::vector<unsigned char> keyS{ 0x26, 0x49, 0x49, 0x49, 0x32 }; 
	std::vector<unsigned char> keyT{ 0x01, 0x01, 0x7F, 0x01, 0x01 };
	std::vector<unsigned char> keyU{ 0x3F, 0x40, 0x40, 0x40, 0x3F }; 
	std::vector<unsigned char> keyV{ 0x0F, 0x30, 0x40, 0x30, 0x0F };
	std::vector<unsigned char> keyW{ 0x3F, 0x40, 0x38, 0x40, 0x3F }; 
	std::vector<unsigned char> keyX{ 0x63, 0x14, 0x08, 0x14, 0x63 };
	std::vector<unsigned char> keyY{ 0x03, 0x04, 0x78, 0x04, 0x03 }; 
	std::vector<unsigned char> keyZ{ 0x61, 0x51, 0x49, 0x45, 0x43 };
	// special chars (line 863 for more info)
	std::vector<unsigned char> sc00{ 0x00, 0x00, 0x00, 0x00, 0x00 }; 
	std::vector<unsigned char> sc01{ 0x00, 0x00, 0x40, 0x00, 0x00 };
	std::vector<unsigned char> sc02{ 0x00, 0x40, 0x20, 0x00, 0x00 }; 
	std::vector<unsigned char> sc03{ 0x00, 0x00, 0x5F, 0x00, 0x00 };
	std::vector<unsigned char> sc04{ 0x06, 0x01, 0x59, 0x09, 0x06 }; 
	std::vector<unsigned char> sc05{ 0x14, 0x3E, 0x14, 0x3E, 0x14 };
	std::vector<unsigned char> sc06{ 0x08, 0x08, 0x3E, 0x08, 0x08 }; 
	std::vector<unsigned char> sc07{ 0x08, 0x08, 0x08, 0x08, 0x08 };
	std::vector<unsigned char> sc08{ 0x14, 0x14, 0x14, 0x14, 0x14 }; 
	std::vector<unsigned char> sc09{ 0x00, 0x00, 0x22, 0x00, 0x00 };
	std::vector<unsigned char> sc10{ 0x00, 0x40, 0x22, 0x00, 0x00 }; 
	std::vector<unsigned char> sc11{ 0x22, 0x14, 0x08, 0x14, 0x22 };
	std::vector<unsigned char> sc12{ 0x20, 0x10, 0x08, 0x04, 0x02 }; 
	std::vector<unsigned char> sc13{ 0x02, 0x04, 0x08, 0x10, 0x20 };
	std::vector<unsigned char> sc14{ 0x00, 0x00, 0x3E, 0x41, 0x00 }; 
	std::vector<unsigned char> sc15{ 0x00, 0x41, 0x3E, 0x00, 0x00 };
	std::vector<unsigned char> sc16{ 0x00, 0x00, 0x03, 0x00, 0x00 }; 
	std::vector<unsigned char> sc17{ 0x00, 0x03, 0x00, 0x03, 0x00 };
	std::vector<unsigned char> sc18{ 0x00, 0x22, 0x14, 0x08, 0x00 }; 
	std::vector<unsigned char> sc19{ 0x00, 0x08, 0x14, 0x22, 0x00 };
	// last one used for non recognized char
	std::vector<unsigned char> scXX{ 0xAA, 0x55, 0xAA, 0x55, 0xAA };
	// overloaded operators
	template <class T> struct v2d_gen
	{
		T x = 0; T y = 0;
		inline v2d_gen() : x(0), y(0) { }
		inline v2d_gen(T _x, T _y) : x(_x), y(_y) { }
		inline v2d_gen(const v2d_gen& v) : x(v.x), y(v.y) { }
		inline v2d_gen  operator +  (const v2d_gen& rhs) const { return v2d_gen(this->x + rhs.x, this->y + rhs.y); }
		inline v2d_gen  operator -  (const v2d_gen& rhs) const { return v2d_gen(this->x - rhs.x, this->y - rhs.y); }
		inline v2d_gen  operator *  (const T& rhs) const { return v2d_gen(this->x * rhs, this->y * rhs); }
		inline v2d_gen  operator /  (const T& rhs) const { return v2d_gen(this->x / rhs, this->y / rhs); }
		inline v2d_gen& operator += (const v2d_gen& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
		inline v2d_gen& operator -= (const v2d_gen& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
		inline v2d_gen& operator *= (const T& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
		inline v2d_gen& operator /= (const T& rhs) { this->x /= rhs; this->y /= rhs; return *this; }
		bool operator == (const v2d_gen& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
		bool operator != (const v2d_gen& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }
		inline T& operator [] (std::size_t i) { return *((T*)this + i); }
		inline operator v2d_gen<int>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
		inline operator v2d_gen<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }
	};

	template<class T> inline v2d_gen<T> operator * (const float& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs * rhs.x, lhs * rhs.y); }
	template<class T> inline v2d_gen<T> operator * (const double& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs * rhs.x, lhs * rhs.y); }
	template<class T> inline v2d_gen<T> operator * (const int& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs * rhs.x, lhs * rhs.y); }
	template<class T> inline v2d_gen<T> operator / (const float& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs / rhs.x, lhs / rhs.y); }
	template<class T> inline v2d_gen<T> operator / (const double& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs / rhs.x, lhs / rhs.y); }
	template<class T> inline v2d_gen<T> operator / (const int& lhs, const v2d_gen<T>& rhs) { return v2d_gen<T>(lhs / rhs.x, lhs / rhs.y); }
	template<class T> inline std::string float_precision(const T a_value, const int n = 2) { std::ostringstream out; out.precision(n); out << std::fixed << a_value; return out.str(); }

	typedef v2d_gen<int32_t> vi2d;	// integer
	typedef v2d_gen<float> vf2d;	// float
	typedef v2d_gen<double> vd2d;	// double
	// button/key state
	struct HWButton
	{
		bool bPressed = false;
		bool bReleased = false;
		bool bHeld = false;
	};
	// manages sprites
	class Sprite
	{
	public:
		Sprite();
		Sprite(int32_t w, int32_t h);
		~Sprite(); // destructor

		int32_t width = 0; 
		int32_t height = 0;

		Pixel GetPixel(gck::vi2d pos);
		bool  SetPixel(gck::vi2d pos, Pixel p);
		Pixel* GetData();

	private:
		Pixel* pColData = nullptr; // all pixels
	};
	// keys available
	enum Key
	{
		NONE, 
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, PERIOD, COMMA, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN, BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9, NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL
	};
	// core
	class xGameEngine
	{
	public:
		xGameEngine();
		// defines our window
		gck::rc Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h, bool fullScreen = false, bool vsync = true, bool showCursor = true, bool thickFrame = true);
		gck::rc Start();

		virtual bool appCreate();
		virtual bool appUpdate(float fElapsedTime);
		virtual bool appDestroy();

		bool IsFocused(); // returns true if window is currently in focus
		HWButton GetKey(Key k);
		HWButton GetMouse(uint32_t b);
		int32_t GetMouseX();
		int32_t GetMouseY();
		int32_t ScreenWidth();
		int32_t ScreenHeight();
		Sprite* GetDrawTarget();
		void SetDrawTarget(Sprite* target); // null for primary screen
		// an unique sprite which works like a buffer for chars
		Sprite* stamp = nullptr;
		///////////////////////////////////////////////////////
		// useful functions declarations
		///////////////////////////////////////////////////////
		inline float map(int s, float a1, float a2, float b1, float b2);
		inline float computeDistance(const gck::vi2d& p1, const gck::vi2d& p2);
		inline float lerp(float a, float b, float f);
		inline vf2d GetVector(int32_t nIndex, int32_t fRadius, int32_t fFactor);
		vi2d rotatePoint(gck::vi2d p, gck::vi2d axis, float angle);
		bool inTriangle(const gck::vi2d& p, const gck::vi2d& pos1, const gck::vi2d& pos2, const gck::vi2d& pos3);
		bool isPointInsidePolygon(std::vector<gck::vi2d> vertices, gck::vi2d px);
		///////////////////////////////////////////////////////
		// functions declarations in order to draw everything
		///////////////////////////////////////////////////////
		virtual bool Draw(const gck::vi2d& pos, Pixel p = gck::WHITE);
		void DrawLine(const gck::vi2d& pos1, const gck::vi2d& pos2, Pixel p = gck::WHITE);
		void DrawFrame(const gck::vi2d& pos1, const gck::vi2d& pos2, Pixel p = gck::WHITE, bool fill = false); // simple rectangle
		void DrawRect(const gck::vi2d& pos, const int32_t w, const int32_t h, const int& angle = 0, Pixel p = gck::WHITE, bool fill = false); // rotation available
		void DrawTris(const gck::vi2d& pos1, const gck::vi2d& pos2, const gck::vi2d& pos3, const int& angle, Pixel p = gck::WHITE, bool fill = false);
		void DrawCircle(const gck::vi2d& pos, const int& fRadius, Pixel p = gck::WHITE, bool fill = false, const int& thickness = 0);
		void DrawPolygon(const gck::vi2d& pos, const int& fRadius, int32_t fFactor, const int& angle, Pixel p = gck::WHITE, bool fill = false); // regular polygon
		void DrawPolygon(const std::vector<gck::vi2d> vertices, Pixel p = gck::WHITE, bool fill = false); // no regular polygon
		void DrawSprite(const gck::vi2d& pos, Sprite* sprite, uint32_t scale = 1);
		void DrawFont(int32_t x, int32_t y); // in order to write string
		std::vector<unsigned char> checkChar(char c);
		void DrawString(gck::vi2d pos, std::string &str, gck::Pixel p = gck::WHITE, int32_t scale = 1);
		void Clear(Pixel p, Pixel pp = VERY_DARK_GREY, int stripe = 0); // to clear screen each frame
		void SetScreenSize(int w, int h);
		// application name
		std::string sAppName;

	private: // inner mysterious workings for window creation
		Sprite* pDefaultDrawTarget = nullptr;
		Sprite* pDrawTarget = nullptr;
		uint32_t	nScreenWidth = 256;
		uint32_t	nScreenHeight = 240;
		uint32_t	nPixelWidth = 4;
		uint32_t	nPixelHeight = 4;
		int32_t		nMousePosX = 0;
		int32_t		nMousePosY = 0;
		int32_t		nMousePosXcache = 0;
		int32_t		nMousePosYcache = 0;
		int32_t		nWindowWidth = 0;
		int32_t		nWindowHeight = 0;
		int32_t		nViewX = 0;
		int32_t		nViewY = 0;
		int32_t		nViewW = 0;
		int32_t		nViewH = 0;
		float		fPixelX = 1.0f;
		float		fPixelY = 1.0f;
		bool		bHasInputFocus = false;
		bool		bHasMouseFocus = false;
		bool		bFullScreen = false;	// fullscreen
		bool		bEnableVSYNC = false;	// Vsync
		bool        bShowCursor = true;		// visible cursor 
		bool		bThickFrame = false;	// resize window control
		float		fFrameTimer = 0.0f;
		int			nFrameCount = 0;
		std::function<gck::Pixel(const int x, const int y, const gck::Pixel&, const gck::Pixel&)> funcPixelMode;
		// keys
		static std::map<size_t, uint8_t> mapKeys;
		bool		pKeyNewState[256]{ 0 };
		bool		pKeyOldState[256]{ 0 };
		HWButton	pKeyboardState[256];
		// mouse
		bool		pMouseNewState[5]{ 0 };
		bool		pMouseOldState[5]{ 0 };
		HWButton	pMouseState[5];
		// context
		HDC			glDeviceContext = nullptr;
		HGLRC		glRenderContext = nullptr;
		GLuint		glBuffer;
		// core
		void		EngineThread();
		static std::atomic<bool> bAtomActive; // in order to kill the main thread
		// common initialisation functions
		void gck_UpdateMouse(int32_t x, int32_t y);
		void gck_UpdateWindowSize(int32_t x, int32_t y);
		void gck_UpdateViewport();
		bool gck_OpenGLCreate();
		HWND gck_WindowCreate();
		HWND gck_hWnd = nullptr;
		std::wstring wsAppName;
		static LRESULT CALLBACK gck_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class XGGE
	{	
		friend class gck::xGameEngine;
	protected:
		static xGameEngine* xxx;
	};
}
#endif // GECKOO_ENGINE_DEF
/////////////////////////////////////////////////////////////////////////////////
#ifdef GECKOO_ENGINE_APP
#undef GECKOO_ENGINE_APP

namespace gck
{	// black pixel full alpha at start
	Pixel::Pixel() 
	{ 
		r = 0; 
		g = 0; 
		b = 0; 
		a = 255; 
	} 

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) 
	{   // rgba entries
		r = red; 
		g = green; 
		b = blue; 
		a = alpha; 
	}
	// blank
	Sprite::Sprite()
	{
		pColData = nullptr;
		width = 0;
		height = 0;
	}
	
	Sprite::Sprite(int32_t w, int32_t h)
	{	// erases the previous one if exists
		if (pColData) 
			delete[] pColData;

		width = w; height = h;
		pColData = new Pixel[width * height];
		// set array with pixels
		for (int32_t i = 0; i < width * height; i++)
			pColData[i] = Pixel(); // black pixel
	}
	// clean up
	Sprite::~Sprite()
	{
		if (pColData) 
			delete[] pColData;
	}
	// boolean operators - is it the same pixel?
	bool Pixel::operator==(const Pixel& p) const { return n == p.n; }
	bool Pixel::operator!=(const Pixel& p) const { return n != p.n; }

	Pixel Sprite::GetPixel(gck::vi2d pos)
	{
		if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height)
			return pColData[pos.y * width + pos.x]; // return sprite at location pos
		else
			return gck::BLANK; // return an empty pixel
	}
	// paint a pixel
	bool Sprite::SetPixel(gck::vi2d pos, Pixel p)
	{
		if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height)
		{	// color this pos pixel with p color
			pColData[pos.y * width + pos.x] = p;
			return true;
		}
		else
			return false;
	}

	Pixel* Sprite::GetData() { return pColData; }
	// constructor
	xGameEngine::xGameEngine()
	{
		sAppName = "Undefined";
		gck::XGGE::xxx = this;
	}
	// creates our window according to its features
	gck::rc xGameEngine::Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h, bool fullScreen, bool vsync, bool showCursor, bool thickFrame)
	{
		nScreenWidth = screen_w;
		nScreenHeight = screen_h;
		nPixelWidth = pixel_w;
		nPixelHeight = pixel_h;
		bFullScreen = fullScreen;
		bEnableVSYNC = vsync;
		bShowCursor = showCursor;
		bThickFrame = thickFrame;
		fPixelX = 2.0f / (float)(nScreenWidth);
		fPixelY = 2.0f / (float)(nScreenHeight);
		// be serious :/
		if (nPixelWidth == 0 || nPixelHeight == 0 || nScreenWidth == 0 || nScreenHeight == 0) 
			return gck::FAIL;
		// create a sprite that represents the primary drawing target
		pDefaultDrawTarget = new Sprite(nScreenWidth, nScreenHeight);
		SetDrawTarget(nullptr);

		return gck::OK; // done
	}
	// size and/or resize screen
	void xGameEngine::SetScreenSize(int w, int h)
	{
		delete pDefaultDrawTarget;
		nScreenWidth = w; nScreenHeight = h;
		pDefaultDrawTarget = new Sprite(nScreenWidth, nScreenHeight);

		SetDrawTarget(nullptr);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(glDeviceContext);
		glClear(GL_COLOR_BUFFER_BIT);
		gck_UpdateViewport();
	}

	gck::rc xGameEngine::Start()
	{	// constructs the window
		if (!gck_WindowCreate()) 
			return gck::FAIL; // oups
		
		bAtomActive = true; // starts the thread with an atomic boolean at true (false is the end)
		std::thread t = std::thread(&xGameEngine::EngineThread, this);
		MSG msg; // handle for message

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		t.detach(); // or join ???
		return gck::OK; // done
	}
	// which sprite we want to modify
	// which sprite we want to modify
	void xGameEngine::SetDrawTarget(Sprite* target)
	{
		if (target) 
			pDrawTarget = target;
		else 
			pDrawTarget = pDefaultDrawTarget;
	}
	// which sprite we are controlling
	Sprite* xGameEngine::GetDrawTarget() { return pDrawTarget; }
	// events
	bool xGameEngine::IsFocused() { return bHasInputFocus; }
	HWButton xGameEngine::GetKey(Key k) { return pKeyboardState[k]; }
	HWButton xGameEngine::GetMouse(uint32_t b) { return pMouseState[b]; }
	int32_t xGameEngine::GetMouseX() { return nMousePosX; }
	int32_t xGameEngine::GetMouseY() { return nMousePosY; }
	int32_t xGameEngine::ScreenWidth() { return nScreenWidth; }
	int32_t xGameEngine::ScreenHeight() { return nScreenHeight; }
	///////////////////////////////////////////////////////
	// useful functions definitions
	///////////////////////////////////////////////////////
	inline float xGameEngine::map(int s, float a1, float a2, float b1, float b2) { return b1 + (s - a1) * (b2 - b1) / (a2 - a1); }
	// gives point in a circle according its factor
	inline vf2d xGameEngine::GetVector(int32_t nIndex, int32_t fRadius, int32_t fFactor)
	{
		float fAngle = map(nIndex % fFactor, 0.0f, static_cast<float>(fFactor), 0.0f, 2 * PI);
		return { round(fRadius * sin(fAngle + PI)), round(fRadius * cos(fAngle + PI)) };
	}
	// compute distance between two points
	inline float xGameEngine::computeDistance(const gck::vi2d& p1, const gck::vi2d& p2)
	{
		float diffY = static_cast<float>(p1.y) - static_cast<float>(p2.y);
		float diffX = static_cast<float>(p1.x) - static_cast<float>(p2.x);
		return sqrt((diffY * diffY) + (diffX * diffX));
	}
	// experimental lerp !!!
	inline float xGameEngine::lerp(float a, float b, float f) 
	{ 
		return a + f * (b - a); 
	}
	// function to rotate a point around its axis
	vi2d xGameEngine::rotatePoint(gck::vi2d p, gck::vi2d axis, float angle)
	{
		float fAngle = angle * (PI / 180);
		float s = sin(fAngle); float c = cos(fAngle);

		p -= axis; // hop
		gck::vf2d nP = { p.x * c - p.y * s, p.x * s + p.y * c };
		p = { int(round(nP.x)), int(round(nP.y)) };
		p += axis; // and hop again

		return p;
	}
	// function in order to check if a point is inside a polygon
	bool xGameEngine::isPointInsidePolygon(std::vector<gck::vi2d> vertices, gck::vi2d pos)
	{
		bool isInside = false;
		auto num_verts = vertices.size();

		for (size_t i = 0, j = num_verts - 1; i < num_verts; j = i++)
		{
			double x1 = vertices[i].x;
			double y1 = vertices[i].y;
			double x2 = vertices[j].x;
			double y2 = vertices[j].y;

			if (((y1 > pos.y) != (y2 > pos.y)) && (pos.x < (x2 - x1) * (pos.y - y1) / (y2 - y1) + x1))
				isInside = !isInside;
		}

		return isInside;
	}
	// check if a point is inside a triangle (used to fill polygons)
	bool xGameEngine::inTriangle(const gck::vi2d& p, const gck::vi2d& pos1, const gck::vi2d& pos2, const gck::vi2d& pos3)
	{
		int s = (pos1.x - pos3.x) * (p.y - pos3.y) - (pos1.y - pos3.y) * (p.x - pos3.x);
		int t = (pos2.x - pos1.x) * (p.y - pos1.y) - (pos2.y - pos1.y) * (p.x - pos1.x);

		if ((s < 0) != (t < 1) && s != 0 && t != 1)
			return false;

		int d = (pos3.x - pos2.x) * (p.y - pos2.y) - (pos3.y - pos2.y) * (p.x - pos2.x);

		return d == 0 || (d < 0) == (s + t <= 0);
	}
	///////////////////////////////////////////////////////
	// functions definitions in order to draw (almost) everything
	///////////////////////////////////////////////////////
	bool xGameEngine::Draw(const gck::vi2d& pos, Pixel p)
	{
		if (!pDrawTarget) 
			return false;

		Pixel d = pDrawTarget->GetPixel(pos);
		// escape quickly if the pixel has the same color
		if (p == d) 
			return false;

		float a = (float)(p.a / 255.0f);
		float c = 1 - a;
		float r = a * (float)p.r + c * (float)d.r;
		float g = a * (float)p.g + c * (float)d.g;
		float b = a * (float)p.b + c * (float)d.b;
		// new pixel at pos
		return pDrawTarget->SetPixel(pos, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b));
	}
	// draws a line
	void xGameEngine::DrawLine(const gck::vi2d& pos1, const gck::vi2d& pos2, Pixel p)
	{	// draws a line using a clever stairs/cliff system
		float x1 = static_cast<float>(pos1.x); 
		float y1 = static_cast<float>(pos1.y);
		float x2 = static_cast<float>(pos2.x); 
		float y2 = static_cast<float>(pos2.y);

		float xx = fabs(x1 - x2); 
		float yy = fabs(y1 - y2);

		float xBy = xx / yy; 
		float yBx = yy / xx;
		float oo = 0;
		// line is always weird - even horizontal or vertical
		if (xx >= yy)
		{	// stairs
			for (int o = 0; o < xx; o++)
			{
				if (x1 <= x2 && y1 <= y2)
					Draw(gck::vi2d(static_cast<int>(x1 + o), static_cast<int>(y1 + round(oo))), p);
				else if (x1 <= x2 && y1 > y2)
					Draw(gck::vi2d(static_cast<int>(x1 + o), static_cast<int>(y1 - round(oo))), p);
				else if (x1 > x2 && y1 <= y2)
					Draw(gck::vi2d(static_cast<int>(x1 - o), static_cast<int>(y1 + round(oo))), p);
				else if (x1 > x2 && y1 > y2)
					Draw(gck::vi2d(static_cast<int>(x1 - o), static_cast<int>(y1 - round(oo))), p);
				else
					return;

				oo += yBx;
			}
		}
		else {
			for (int o = 0; o < yy; o++)
			{	// cliff
				if (x1 <= x2 && y1 <= y2)
					Draw(gck::vi2d(static_cast<int>(x1 + round(oo)), static_cast<int>(y1 + o)), p);
				else if (x1 <= x2 && y1 > y2)
					Draw(gck::vi2d(static_cast<int>(x1 + round(oo)), static_cast<int>(y1 - o)), p);
				else if (x1 > x2 && y1 <= y2)
					Draw(gck::vi2d(static_cast<int>(x1 - round(oo)), static_cast<int>(y1 + o)), p);
				else if (x1 > x2 && y1 > y2)
					Draw(gck::vi2d(static_cast<int>(x1 - round(oo)), static_cast<int>(y1 - o)), p);
				else
					return;

				oo += xBy;
			}
		}
	}
	// draws a rectangle according to two points
	void xGameEngine::DrawFrame(const gck::vi2d& pos1, const gck::vi2d& pos2, Pixel p, bool fill)
	{
		if (!fill)
		{
			DrawLine(gck::vi2d(pos1.x, pos1.y), gck::vi2d(pos2.x, pos1.y), p);
			DrawLine(gck::vi2d(pos2.x, pos1.y), gck::vi2d(pos2.x, pos2.y), p);
			DrawLine(gck::vi2d(pos2.x, pos2.y), gck::vi2d(pos1.x, pos2.y), p);
			DrawLine(gck::vi2d(pos1.x, pos2.y), gck::vi2d(pos1.x, pos1.y), p);
		}
		else {
			DrawTris(gck::vi2d(pos1.x, pos1.y), gck::vi2d(pos2.x, pos1.y), gck::vi2d(pos2.x, pos2.y), 0, p, true);
			DrawTris(gck::vi2d(pos2.x, pos2.y), gck::vi2d(pos1.x, pos2.y), gck::vi2d(pos1.x, pos1.y), 0, p, true);
		}
	}
	// draws a rectangle according to a position and sizes
	void xGameEngine::DrawRect(const gck::vi2d& pos, const int32_t w, const int32_t h, const int& angle, Pixel p, bool fill)
	{
		const gck::vi2d& pos1 = { pos.x - w / 2, pos.y - h / 2 };
		const gck::vi2d& pos2 = { pos.x + w / 2, pos.y - h / 2 };
		const gck::vi2d& pos3 = { pos.x + w / 2, pos.y + h / 2 };
		const gck::vi2d& pos4 = { pos.x - w / 2, pos.y + h / 2 };

		gck::vi2d nPos1 = rotatePoint(pos1, pos, static_cast<float>(angle));
		gck::vi2d nPos2 = rotatePoint(pos2, pos, static_cast<float>(angle));
		gck::vi2d nPos3 = rotatePoint(pos3, pos, static_cast<float>(angle));
		gck::vi2d nPos4 = rotatePoint(pos4, pos, static_cast<float>(angle));

		if (fill)
		{
			DrawTris(nPos1, nPos3, nPos2, 0, p, true);
			DrawTris(nPos1, nPos3, nPos4, 0, p, true);
		}
		else {
			DrawLine(nPos1, nPos2, p);
			DrawLine(nPos2, nPos3, p);
			DrawLine(nPos3, nPos4, p);
			DrawLine(nPos4, nPos1, p);
		}
	}
	// draws a triangle according to three points
	void xGameEngine::DrawTris(const gck::vi2d& pos1, const gck::vi2d& pos2, const gck::vi2d& pos3, const int& angle, Pixel p, bool fill)
	{
		const gck::vi2d centroïd = (pos1 + pos2 + pos3) / 3;
		gck::vi2d nPos1 = rotatePoint(pos1, centroïd, static_cast<float>(angle));
		gck::vi2d nPos2 = rotatePoint(pos2, centroïd, static_cast<float>(angle));
		gck::vi2d nPos3 = rotatePoint(pos3, centroïd, static_cast<float>(angle));

		if (fill)
		{	// wrap the std::min/max to avoid conflict with macro in windows.h
			int maxX = (std::max)(nPos1.x, (std::max)(nPos2.x, nPos3.x));
			int minX = (std::min)(nPos1.x, (std::min)(nPos2.x, nPos3.x));
			int maxY = (std::max)(nPos1.y, (std::max)(nPos2.y, nPos3.y));
			int minY = (std::min)(nPos1.y, (std::min)(nPos2.y, nPos3.y));

			for (int x = minX; x <= maxX; x++)
				for (int y = minY; y <= maxY; y++)
					if (inTriangle(gck::vi2d(x, y), nPos1, nPos2, nPos3))
						Draw(gck::vi2d(x, y), p);
		}
		else {
			DrawLine(nPos1, nPos2, p);
			DrawLine(nPos2, nPos3, p);
			DrawLine(nPos3, nPos1, p);
		}
	}
	// draws a circle according to a single point and a radius 
	void xGameEngine::DrawCircle(const gck::vi2d& pos, const int& fRadius, Pixel p, bool fill, const int& thickness)
	{
		int x0 = 0;
		int y0 = fRadius;
		int d = 3 - 2 * fRadius;

		if (!fill)
		{
			if (fRadius > 0)
			{
				while (y0 >= x0) // only formulate 1/8 of circle
				{
					Draw(gck::vi2d(pos.x + x0, pos.y - y0), p);
					Draw(gck::vi2d(pos.x + y0, pos.y + x0), p);
					Draw(gck::vi2d(pos.x - x0, pos.y + y0), p);
					Draw(gck::vi2d(pos.x - y0, pos.y - x0), p);

					if (x0 != 0 && x0 != y0)
					{
						Draw(gck::vi2d(pos.x + y0, pos.y - x0), p);
						Draw(gck::vi2d(pos.x + x0, pos.y + y0), p);
						Draw(gck::vi2d(pos.x - y0, pos.y + x0), p);
						Draw(gck::vi2d(pos.x - x0, pos.y - y0), p);
					}

					if (d < 0)
						d += 4 * x0++ + 6;
					else
						d += 4 * (x0++ - y0--) + 10;
				}
			}
			else // draws a single point for radius 0
				Draw(gck::vi2d(pos.x, pos.y), p);
		}
		else {
			if (fRadius > 0)
			{
				auto drawline = [&](int sx, int ex, int y)
				{
					for (int x = sx; x <= ex; x++)
						if (thickness > 0 && thickness < fRadius)
						{	// donuts
							if (computeDistance(gck::vi2d(x, y), pos) > fRadius - thickness)
								Draw(gck::vi2d(x, y), p);
						}
						else
							Draw(gck::vi2d(x, y), p);
				};

				while (y0 >= x0)
				{
					drawline(pos.x - y0, pos.x + y0, pos.y - x0);

					if (x0 > 0)	
						drawline(pos.x - y0, pos.x + y0, pos.y + x0);

					if (d < 0)
						d += 4 * x0++ + 6;
					else
					{
						if (x0 != y0)
						{
							drawline(pos.x - x0, pos.x + x0, pos.y - y0);
							drawline(pos.x - x0, pos.x + x0, pos.y + y0);
						}
						d += 4 * (x0++ - y0--) + 10;
					}
				}
			}
			else // draws a single point for radius 0
				Draw(gck::vi2d(pos.x, pos.y), p);
		}
	}
	// draws a regular polygon according to a single point, a factor and a radius
	void xGameEngine::DrawPolygon(const gck::vi2d& pos, const int& fRadius, int32_t fFactor, const int& angle, Pixel p, bool fill)
	{
		if (fFactor > 8) 
			fFactor = 8; // beyond 8 it's almost a circle no ?
		// sometimes a point at the center is required
		if (fill) 
			Draw(pos, p);

		for (int i = 0; i < fFactor; i++)
		{
			gck::vf2d vPoint = rotatePoint(GetVector(i, fRadius, fFactor) + pos, pos, static_cast<float>(angle));
			gck::vf2d pPoint = rotatePoint(GetVector(i - 1, fRadius, fFactor) + pos, pos, static_cast<float>(angle));

			fill ? DrawTris(pos, vPoint, pPoint, 0, p, true) : DrawLine(vPoint, pPoint, p);
		}
	}
	// draws a regular polygon according to a vector with points 
	void xGameEngine::DrawPolygon(std::vector<gck::vi2d> vertices, Pixel p, bool fill)
	{
		for (size_t v = 0; v < vertices.size() - 1; ++v)
			DrawLine(gck::vi2d(vertices[v].x, vertices[v].y), gck::vi2d(vertices[v + 1].x, vertices[v + 1].y ), p);

		DrawLine(gck::vi2d(vertices[vertices.size() - 1].x, vertices[vertices.size() - 1].y), gck::vi2d(vertices[0].x, vertices[0].y ), p);

		if (fill)
		{
			std::vector<gck::vi2d> f;
			int lx = 0; // lower x
			int ly = 0;	// higher x
			int hx = 0; // lower y
			int hy = 0; // higher y
			// check lower and higher x y coordinates
			for (auto& o : f)
			{
				if (o.x < lx) lx = o.x;
				if (o.x > hx) hx = o.x;
				if (o.y < ly) ly = o.y;
				if (o.y > hy) hy = o.y;
			}
			// it works as expected, but don't use it. Instead check each pixel at start (OnUserCreate) according to the vertices vector. Store pixels one time ++
			for (size_t w = lx; w < hx; ++w)
				for (size_t h = ly; h < hy; ++h)
					if (isPointInsidePolygon(vertices, gck::vi2d(static_cast<int>(w), static_cast<int>(h)))) // too hard !!!
						Draw(gck::vi2d(static_cast<int>(w), static_cast<int>(h)), p);
		}
	}
	// draws sprite which we declare or the first one - our main screen
	void xGameEngine::DrawSprite(const gck::vi2d& pos, Sprite* sprite, uint32_t scale)
	{
		if (sprite == nullptr)
			return;

		for (int32_t i = 0; i < sprite->width; i++)
			for (int32_t j = 0; j < sprite->height; j++)
				for (uint32_t is = 0; is < scale; is++)
					for (uint32_t js = 0; js < scale; js++)
						Draw(gck::vi2d(pos.x + (i * scale) + is - (sprite->width / 2 * scale), pos.y + (j * scale) + js - (sprite->height / 2 * scale)), sprite->GetPixel(gck::vi2d(i, j)));
	}

	void xGameEngine::DrawFont(int32_t x, int32_t y) 
	{   // a unique sprite for each letter
		stamp = new gck::Sprite(x, y); 
	} 

	std::vector<unsigned char> xGameEngine::checkChar(char c)
	{	// numeral
		if (c == '0') return num0; 
		if (c == '1') return num1; 
		if (c == '2') return num2; 
		if (c == '3') return num3;
		if (c == '4') return num4; 
		if (c == '5') return num5; 
		if (c == '6') return num6; 
		if (c == '7') return num7;
		if (c == '8') return num8; 
		if (c == '9') return num9;
		// alpha
		if (c == 'A') return keyA; 
		if (c == 'B') return keyB; 
		if (c == 'C') return keyC; 
		if (c == 'D') return keyD;
		if (c == 'E') return keyE; 
		if (c == 'F') return keyF; 
		if (c == 'G') return keyG; 
		if (c == 'H') return keyH;
		if (c == 'I') return keyI; 
		if (c == 'J') return keyJ; 
		if (c == 'K') return keyK; 
		if (c == 'L') return keyL;
		if (c == 'M') return keyM; 
		if (c == 'N') return keyN; 
		if (c == 'O') return keyO; 
		if (c == 'P') return keyP;
		if (c == 'Q') return keyQ; 
		if (c == 'R') return keyR; 
		if (c == 'S') return keyS; 
		if (c == 'T') return keyT;
		if (c == 'U') return keyU; 
		if (c == 'V') return keyV; 
		if (c == 'W') return keyW; 
		if (c == 'X') return keyX;
		if (c == 'Y') return keyY; 
		if (c == 'Z') return keyZ;
		// special chars
		if (c == ' ') return sc00; 
		if (c == '.') return sc01; 
		if (c == ',') return sc02; 
		if (c == '!') return sc03;
		if (c == '?') return sc04; 
		if (c == '#') return sc05; 
		if (c == '+') return sc06; 
		if (c == '-') return sc07;
		if (c == '=') return sc08; 
		if (c == ':') return sc09; 
		if (c == ';') return sc10; 
		if (c == '*') return sc11;
		if (c == '/') return sc12; 
		if (c == '\\')return sc13; 
		if (c == '(') return sc14; 
		if (c == ')') return sc15;
		if (c == '\'')return sc16; 
		if (c == '\"')return sc17; 
		if (c == '>') return sc18; 
		if (c == '<') return sc19;
		else
			return scXX; // blank
	}

	void xGameEngine::DrawString(gck::vi2d pos, std::string &str, gck::Pixel p, int32_t scale)
	{	// our letters scale - only 5x7 pixels
		short fontX = 5; 
		short fontY = 7; 
		short pp = pos.x;

		if (stamp == nullptr) 
			DrawFont(fontX, fontY);
		// only lower case
		std::vector<unsigned char> byte;
		transform(str.begin(), str.end(), str.begin(), ::toupper);

		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] == '\n')
			{
				pos.y += (fontY + 1) * scale;
				pos.x = pp * scale;
				continue;
			}

			byte = checkChar(str[i]);

			for (int x = 0; x < fontX; x++)
				for (int y = 0; y < fontY; y++)
					if ((byte[x] >> y) & 0x1)
						stamp->SetPixel(gck::vi2d(x, y), p);
					else
						stamp->SetPixel(gck::vi2d(x, y), gck::BLANK);

			DrawSprite(pos, stamp, scale);
			pos.x += (fontX + 1) * scale; // shift to the next char
		}
	}
	// clear the entire sprite (or screen) according a specific color
	void xGameEngine::Clear(Pixel p, Pixel pp, int stripe)
	{
		int pixels = pDrawTarget->width * pDrawTarget->height;
		Pixel* m = GetDrawTarget()->GetData();

		for (int i = 0; i < pixels; ++i)
			stripe != 0 ? (i % stripe ? m[i] = p : m[i] = pp) : m[i] = p;
	}

	bool xGameEngine::appCreate() { return false; }
	bool xGameEngine::appUpdate(float fElapsedTime) { fElapsedTime;  return false; }
	bool xGameEngine::appDestroy() { return true; }

	void xGameEngine::gck_UpdateViewport()
	{
		int32_t w = nScreenWidth * nPixelWidth;
		int32_t h = nScreenHeight * nPixelHeight;
		float wasp = (float)w / (float)h;

		nViewW = (int32_t)nWindowWidth;
		nViewH = (int32_t)((float)nViewW / wasp);

		if (nViewH > nWindowHeight)
		{
			nViewH = nWindowHeight;
			nViewW = (int32_t)((float)nViewH * wasp);
		}

		nViewX = (nWindowWidth - nViewW) / 2;
		nViewY = (nWindowHeight - nViewH) / 2;
	}
	// update screen size
	void xGameEngine::gck_UpdateWindowSize(int32_t x, int32_t y)
	{
		nWindowWidth = x;
		nWindowHeight = y;

		gck_UpdateViewport();
	}
	// update mouse
	void xGameEngine::gck_UpdateMouse(int32_t x, int32_t y)
	{
		x -= nViewX; 
		y -= nViewY;

		nMousePosXcache = (int32_t)(((float)x / (float)(nWindowWidth - (nViewX * 2)) * (float)nScreenWidth));
		nMousePosYcache = (int32_t)(((float)y / (float)(nWindowHeight - (nViewY * 2)) * (float)nScreenHeight));
		// clamp coordinates X Y
		if (nMousePosXcache >= (int32_t)nScreenWidth) 
			nMousePosXcache = nScreenWidth - 1;
		if (nMousePosYcache >= (int32_t)nScreenHeight) 
			nMousePosYcache = nScreenHeight - 1;
		
		if (nMousePosXcache < 0) 
			nMousePosXcache = 0; 
		if (nMousePosYcache < 0) 
			nMousePosYcache = 0;
	}

	void xGameEngine::EngineThread()
	{	// starts OpenGL, the context is owned by the game thread
		gck_OpenGLCreate();
		glEnable(GL_TEXTURE_2D); // creates screen Texture - disable filtering
		glGenTextures(1, &glBuffer);
		glBindTexture(GL_TEXTURE_2D, glBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nScreenWidth, nScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());
		// it doesn't work as expected - bye
		if (!appCreate()) 
			bAtomActive = false;
		// for fps - initialization
		auto tp1 = std::chrono::system_clock::now(); 
		auto tp2 = std::chrono::system_clock::now();

		while (bAtomActive) // atomic boolean at true
		{
			while (bAtomActive)
			{	// handle timing
				tp2 = std::chrono::system_clock::now(); // difference
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2; // flip them
				// our time per frame coefficient
				float fElapsedTime = elapsedTime.count();
				// handle User Input - Keyboard control
				for (int i = 0; i < UCHAR_MAX; ++i)
				{
					pKeyboardState[i].bPressed = false;
					pKeyboardState[i].bReleased = false;

					if (pKeyNewState[i] != pKeyOldState[i])
					{
						if (pKeyNewState[i])
						{
							pKeyboardState[i].bPressed = !pKeyboardState[i].bHeld;
							pKeyboardState[i].bHeld = true;
						}
						else {
							pKeyboardState[i].bReleased = true;
							pKeyboardState[i].bHeld = false;
						}
					}

					pKeyOldState[i] = pKeyNewState[i];
				}
				// handle user input for mouse - only 3 buttons ?
				for (int i = 0; i < 3; i++)
				{
					pMouseState[i].bPressed = false;
					pMouseState[i].bReleased = false;

					if (pMouseNewState[i] != pMouseOldState[i])
					{
						if (pMouseNewState[i])
						{
							pMouseState[i].bPressed = !pMouseState[i].bHeld;
							pMouseState[i].bHeld = true;
						}
						else {
							pMouseState[i].bReleased = true;
							pMouseState[i].bHeld = false;
						}
					}
					pMouseOldState[i] = pMouseNewState[i];
				}
				// mouse pos
				nMousePosX = nMousePosXcache;
				nMousePosY = nMousePosYcache;
				// handle Frame Update
				if (!appUpdate(fElapsedTime)) 
					bAtomActive = false; // problem - bye
				// displays graphics
				glViewport(nViewX, nViewY, nViewW, nViewH);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());
				// displays the main plane on screen - texture is inverted acording to its vertices
				glBegin(GL_QUADS);
				// down left
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-1.0f, 1.0f, 0.0f);
				// up left
				glTexCoord2f(0.0, 1.0); 
				glVertex3f(-1.0f, -1.0f, 0.0f);
				// up right
				glTexCoord2f(1.0, 1.0);
				glVertex3f(1.0f, -1.0f, 0.0f);
				// down right
				glTexCoord2f(1.0, 0.0); 
				glVertex3f(1.0f, 1.0f, 0.0f);
				glEnd();

				SwapBuffers(glDeviceContext);
				// update title bar each second
				fFrameTimer += fElapsedTime;
				++nFrameCount;

				if (fFrameTimer >= 1.0f)
				{
					fFrameTimer = 0.0f; // flip
					// here you get some informations in the title bar - customize them as you want
					std::string sTitle = sAppName + " - FPS : " + std::to_string(nFrameCount) + " ~ " + float_precision(fElapsedTime, 4);
					SetWindowTextA(gck_hWnd, sTitle.c_str());
					nFrameCount = 0; // reset frame count
				}
			}
			// close the application if the atomic boolean is false
			appDestroy()?bAtomActive = false : bAtomActive = true;
		}

		wglDeleteContext(glRenderContext);
		PostMessage(gck_hWnd, WM_DESTROY, 0, 0);
	}
	
	HWND xGameEngine::gck_WindowCreate()
	{
		WNDCLASS wc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// no specific icon
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// classical
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// redraw available
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = gck_WindowEvent;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = L"GECKOO_GAME_ENGINE";
		RegisterClass(&wc);
		nWindowWidth = (LONG)nScreenWidth * (LONG)nPixelWidth;
		nWindowHeight = (LONG)nScreenHeight * (LONG)nPixelHeight;
		// defines window features
		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		if (bThickFrame && !bFullScreen) 
			dwStyle = dwStyle | WS_THICKFRAME; // allows main window resize
		
		nViewW = nWindowWidth;
		nViewH = nWindowHeight;
		ShowCursor(bShowCursor); // hide or show cursor
		// fullscreen
		if (bFullScreen)
		{
			dwExStyle = 0;
			dwStyle = WS_VISIBLE | WS_POPUP;
			HMONITOR hmon = MonitorFromWindow(gck_hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };

			if (!GetMonitorInfo(hmon, &mi)) 
				return NULL;

			nWindowWidth = mi.rcMonitor.right;
			nWindowHeight = mi.rcMonitor.bottom;
		}
		gck_UpdateViewport();
		// keep client size as requested
		RECT rWndRect = { 0, 0, nWindowWidth, nWindowHeight };
		AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
		int width = rWndRect.right - rWndRect.left;
		int height = rWndRect.bottom - rWndRect.top;
		// in order to display our application (no fullscreen) in the center of the screen
		int xPos = (GetSystemMetrics(SM_CXSCREEN) - rWndRect.right) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - rWndRect.bottom) / 2;
		gck_hWnd = CreateWindowEx(dwExStyle, L"GECKOO_GAME_ENGINE", L"", dwStyle, xPos, yPos, width, height, NULL, NULL, GetModuleHandle(nullptr), this);
		// keyboard mapping
		mapKeys[0x00] = Key::NONE;
		mapKeys[0x41] = Key::A; 
		mapKeys[0x42] = Key::B; 
		mapKeys[0x43] = Key::C; 
		mapKeys[0x44] = Key::D; 
		mapKeys[0x45] = Key::E; 
		mapKeys[0x46] = Key::F;
		mapKeys[0x47] = Key::G; 
		mapKeys[0x48] = Key::H; 
		mapKeys[0x49] = Key::I; 
		mapKeys[0x4A] = Key::J; 
		mapKeys[0x4B] = Key::K; 
		mapKeys[0x4C] = Key::L;
		mapKeys[0x4D] = Key::M; 
		mapKeys[0x4E] = Key::N; 
		mapKeys[0x4F] = Key::O; 
		mapKeys[0x50] = Key::P; 
		mapKeys[0x51] = Key::Q; 
		mapKeys[0x52] = Key::R;
		mapKeys[0x53] = Key::S; 
		mapKeys[0x54] = Key::T; 
		mapKeys[0x55] = Key::U; 
		mapKeys[0x56] = Key::V; 
		mapKeys[0x57] = Key::W; 
		mapKeys[0x58] = Key::X;
		mapKeys[0x59] = Key::Y; 
		mapKeys[0x5A] = Key::Z;
		// F keys
		mapKeys[VK_F1] = Key::F1; 
		mapKeys[VK_F2] = Key::F2; 
		mapKeys[VK_F3] = Key::F3; 
		mapKeys[VK_F4] = Key::F4; 
		mapKeys[VK_F5] = Key::F5; 
		mapKeys[VK_F6] = Key::F6;
		mapKeys[VK_F7] = Key::F7; 
		mapKeys[VK_F8] = Key::F8; 
		mapKeys[VK_F9] = Key::F9; 
		mapKeys[VK_F10] = Key::F10;
		mapKeys[VK_F11] = Key::F11; 
		mapKeys[VK_F12] = Key::F12;
		// arrows keys
		mapKeys[VK_DOWN] = Key::DOWN; 
		mapKeys[VK_LEFT] = Key::LEFT; 
		mapKeys[VK_RIGHT] = Key::RIGHT; 
		mapKeys[VK_UP] = Key::UP; 

		mapKeys[VK_RETURN] = Key::ENTER;
		// special keys
		mapKeys[VK_BACK] = Key::BACK; 
		mapKeys[VK_ESCAPE] = Key::ESCAPE; 
		mapKeys[VK_RETURN] = Key::ENTER; 
		mapKeys[VK_PAUSE] = Key::PAUSE; 
		mapKeys[VK_SCROLL] = Key::SCROLL;
		mapKeys[VK_TAB] = Key::TAB; 
		mapKeys[VK_DELETE] = Key::DEL; 
		mapKeys[VK_HOME] = Key::HOME; 
		mapKeys[VK_END] = Key::END; 
		mapKeys[VK_PRIOR] = Key::PGUP;
		mapKeys[VK_NEXT] = Key::PGDN; 
		mapKeys[VK_INSERT] = Key::INS; 
		mapKeys[VK_SHIFT] = Key::SHIFT; 
		mapKeys[VK_CONTROL] = Key::CTRL; 
		mapKeys[VK_SPACE] = Key::SPACE;
		mapKeys[VK_OEM_PERIOD] = Key::PERIOD; 
		mapKeys[VK_OEM_COMMA] = Key::COMMA;
		// digital on keyboard
		mapKeys[0x30] = Key::K0; 
		mapKeys[0x31] = Key::K1; 
		mapKeys[0x32] = Key::K2; 
		mapKeys[0x33] = Key::K3; 
		mapKeys[0x34] = Key::K4;
		mapKeys[0x35] = Key::K5; 
		mapKeys[0x36] = Key::K6; 
		mapKeys[0x37] = Key::K7; 
		mapKeys[0x38] = Key::K8; 
		mapKeys[0x39] = Key::K9;
		// numpad keys
		mapKeys[VK_NUMPAD0] = Key::NP0; 
		mapKeys[VK_NUMPAD1] = Key::NP1; 
		mapKeys[VK_NUMPAD2] = Key::NP2; 
		mapKeys[VK_NUMPAD3] = Key::NP3; 
		mapKeys[VK_NUMPAD4] = Key::NP4;
		mapKeys[VK_NUMPAD5] = Key::NP5; 
		mapKeys[VK_NUMPAD6] = Key::NP6; 
		mapKeys[VK_NUMPAD7] = Key::NP7; 
		mapKeys[VK_NUMPAD8] = Key::NP8; 
		mapKeys[VK_NUMPAD9] = Key::NP9;
		mapKeys[VK_MULTIPLY] = Key::NP_MUL; 
		mapKeys[VK_ADD] = Key::NP_ADD; 
		mapKeys[VK_DIVIDE] = Key::NP_DIV; 
		mapKeys[VK_SUBTRACT] = Key::NP_SUB; 
		mapKeys[VK_DECIMAL] = Key::NP_DECIMAL;

		return gck_hWnd;
	}

	bool xGameEngine::gck_OpenGLCreate()
	{	// creates device context
		glDeviceContext = GetDC(gck_hWnd);

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};
		
		int pf {};

		if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) 
			return false;

		SetPixelFormat(glDeviceContext, pf, &pfd);
		
		if (!(glRenderContext = wglCreateContext(glDeviceContext))) 
			return false;

		wglMakeCurrent(glDeviceContext, glRenderContext);
		glViewport(nViewX, nViewY, nViewW, nViewH);
		// manages VSYNC
		wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapInterval && !bEnableVSYNC) 
			wglSwapInterval(0); // disable VSYNC

		return true;
	}
	// windows event handler
	LRESULT CALLBACK xGameEngine::gck_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static xGameEngine* sge;

		switch (uMsg)
		{
		case WM_CREATE:
			sge = (xGameEngine*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			return 0;
		case WM_MOUSEMOVE:
		{
			uint16_t x = lParam & 0xFFFF;
			uint16_t y = (lParam >> 16) & 0xFFFF;
			int16_t ix = *(int16_t*)&x;
			int16_t iy = *(int16_t*)&y;

			sge->gck_UpdateMouse(ix, iy);
			return 0;
		}
		case WM_SIZE:
		{
			sge->gck_UpdateWindowSize(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
			return 0;
		}

		case WM_MOUSELEAVE: sge->bHasMouseFocus = false;				return 0;
		case WM_SETFOCUS:	sge->bHasInputFocus = true;					return 0;
		case WM_KILLFOCUS:	sge->bHasInputFocus = false;				return 0;
		case WM_KEYDOWN:	sge->pKeyNewState[mapKeys[wParam]] = true;	return 0;
		case WM_KEYUP:		sge->pKeyNewState[mapKeys[wParam]] = false;	return 0;
		case WM_LBUTTONDOWN:sge->pMouseNewState[0] = true;				return 0;
		case WM_LBUTTONUP:	sge->pMouseNewState[0] = false;				return 0;
		case WM_RBUTTONDOWN:sge->pMouseNewState[1] = true;				return 0;
		case WM_RBUTTONUP:	sge->pMouseNewState[1] = false;				return 0;
		case WM_MBUTTONDOWN:sge->pMouseNewState[2] = true;				return 0;
		case WM_MBUTTONUP:	sge->pMouseNewState[2] = false;				return 0;
		case WM_CLOSE:		bAtomActive = false;						return 0;
		case WM_DESTROY:	PostQuitMessage(0);							return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	std::atomic<bool> xGameEngine::bAtomActive{ false };
	std::map<size_t, uint8_t> xGameEngine::mapKeys;
	gck::xGameEngine* gck::XGGE::xxx = nullptr;
}
#endif // GECKOO_ENGINE_APP
