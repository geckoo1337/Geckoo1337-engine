#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
// 16/9
#define SIZE_X 192
#define SIZE_Y 108
#define SCALE 4

class Game : public gck::xGameEngine
{
private:
	int* board;
	int* aliveBoard;
	bool running = false;
	float fTargetFrameTime = 0.2f;
	float fAccumulatedTime = 0.0f;
	int size = 8;
	int ratio = 2;
	gck::Sprite* ply = nullptr;

	std::string info = "\nUse mouse to draw pattern\nPress enter to start\nUse +- to manage time";

public:
	Game() : board(new int[SIZE_X * SIZE_Y]), aliveBoard(new int[SIZE_X * SIZE_Y])
	{
		sAppName = "Conway's Game of Life";
	}
	// destructor
	~Game()
	{
		delete[] board;
		delete[] aliveBoard;
	}
	// clean up function
	void createBoard()
	{
		for (int i = 0; i < SIZE_X; i += 1)
			for (int j = 0; j < SIZE_Y; j += 1)
			{
				board[i * SIZE_Y + j] = 0;
				aliveBoard[i * SIZE_Y + j] = 0;
			}
	}
	// check neighbours
	int CountNeighbours(int x, int y)
	{
		int count = 0;

		if (x > 0 && y > 0) count += board[y - 1 + (x - 1) * SIZE_Y];
		if (x > 0) count += board[y + (x - 1) * SIZE_Y];
		if (x > 0 && y < SIZE_Y - 1) count += board[y + 1 + (x - 1) * SIZE_Y];
		if (y > 0) count += board[y - 1 + x * SIZE_Y];
		if (y < SIZE_Y - 1) count += board[y + 1 + x * SIZE_Y];
		if (x < SIZE_X - 1 && y > 0) count += board[y - 1 + (x + 1) * SIZE_Y];
		if (x < SIZE_X - 1) count += board[y + (x + 1) * SIZE_Y];
		if (x < SIZE_X - 1 && y < SIZE_Y - 1) count += board[y + 1 + (x + 1) * SIZE_Y];

		return count;
	}
	// clean up
	void clearAliveBoard()
	{
		for (int i = 0; i < SIZE_X; ++i)
			for (int j = 0; j < SIZE_Y; ++j)
				aliveBoard[i * SIZE_Y + j] = 0;
	}

	void updateBoard()
	{
		int* newPopulation = new int[SIZE_X * SIZE_Y];

		for (int x = 0; x < SIZE_X; x++)
			for (int y = 0; y < SIZE_Y; y++)
			{
				int neighbourCount = CountNeighbours(x, y);

				if (board[y + x * SIZE_Y] == 1 && neighbourCount >= 2 && neighbourCount <= 3)
				{
					newPopulation[y + x * SIZE_Y] = 1;
					aliveBoard[y + x * SIZE_Y] = 1;
				}
				else if (board[y + x * SIZE_Y] == 1 && neighbourCount > 3)
					newPopulation[y + x * SIZE_Y] = 0;
				else if (board[y + x * SIZE_Y] == 0 && neighbourCount == 3)
				{
					newPopulation[y + x * SIZE_Y] = 1;
					aliveBoard[y + x * SIZE_Y] = 1;
				}
				else // dead cell
					newPopulation[y + x * SIZE_Y] = 0;
			}

		delete[] board; // erases the previous
		board = newPopulation; // set the new
	}

	void handleMouse()
	{
		int x = GetMouseX();
		int y = GetMouseY();

		if (GetMouse(0).bPressed)
			board[y + x * SIZE_Y] = 1;

		else if (GetMouse(1).bPressed)
			board[y + x * SIZE_Y] = 0;
	}

	void createSprite(gck::Sprite* s, gck::Pixel p)
	{
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++)
				if (y % 2 == 0)
					s->SetPixel(gck::vi2d(x, y), p);
				else
					s->SetPixel(gck::vi2d(x, y), gck::GREEN);
	}

	bool appCreate() override
	{
		ply = new gck::Sprite(size, size);
		createSprite(ply, gck::DARK_YELLOW);
		createBoard();

		return true;
	}

	bool appUpdate(float fElapsedTime) override
	{
		if (!running)
			handleMouse();
		
		if (GetKey(gck::Key::ENTER).bPressed && !running)
			running = true;

		if (GetKey(gck::Key::NP_ADD).bHeld && fTargetFrameTime >= 0.05f)
			fTargetFrameTime -= 0.0025f;
		if (GetKey(gck::Key::NP_SUB).bHeld && fTargetFrameTime <= 0.5f)
			fTargetFrameTime += 0.0025f;

		if (GetKey(gck::Key::ESCAPE).bPressed)
		{
			if (running)
			{
				running = false;
				createBoard();
			}
			else
				return false;
		}
		// manages time
		fAccumulatedTime += fElapsedTime;

		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}
		else // don't do anything this frame
			return true;
		
		for (int x = 0; x < SIZE_X; x++)
			for (int y = 0; y < SIZE_Y; y++)
			{
				gck::Pixel color = board[y + x * SIZE_Y] == 1 ? gck::WHITE : (aliveBoard[y + x * SIZE_Y] ? gck::VERY_DARK_GREEN : gck::BLACK);
				Draw(gck::vi2d(x, y), color);
			}

		if (!running)
		{	// pseudo cursor
			DrawString(gck::vi2d(5, 0), info, gck::BLUE, 1);
			DrawCircle(gck::vi2d(GetMouseX(), GetMouseY()), 1, gck::RED);
		}

		if (running)
			updateBoard();

		return true;
	}
};

int main()
{
	Game Conway;

	if (Conway.Construct(SIZE_X, SIZE_Y, SCALE, SCALE, true, true, false))
		Conway.Start();

	return 0;
}