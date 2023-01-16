#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
// cell data 
struct sCell
{
    bool bVisible = false; // is it visible
    bool bHasMine = false; // does it have a mine
    bool bFlagged = false; // has it been flagged
    int nMineCount = 0;    // neighbour mine count
};

class MineSweeper : public gck::xGameEngine
{
public:
    MineSweeper()
    {
        sAppName = "Mine Sweeper";
    }

private:
    std::string loser = "You Lose";
    std::string winner = "You Win!";
    std::string flag = "\41"; // !

    sCell* cells = nullptr;                     // cells array
    int nCells = 10;                            // number of X Y cells
    int nCellWidth = 15;                        // cell width for drawing
    int nCellWidthBy2 = static_cast<int>(floor(nCellWidth / 2));  // by two
    int nMines = 0;                             // total mines on board
    int nNonVisibleCells = nCells * nCells;     // initial number of non visible cells
    bool bGameOver = false;                     // is game over state
    // colours for cell neighbours count
    gck::Pixel countColours[9] = { gck::BLANK, gck::BLUE, gck::DARK_GREEN, gck::RED, gck::DARK_BLUE, gck::MAGENTA, gck::DARK_CYAN, gck::BLACK, gck::GREY };
    // given a cell, return its contents as a string for display
    std::string cellContents(int nCell)
    {
        sCell cell = cells[nCell];

        if (cell.bHasMine)
            return "\43"; // #
        else if (cell.nMineCount != 0)
            return std::to_string(cell.nMineCount);
        else
            return "\40"; // empty cell
    }
    // return the cell colour for the mine count
    gck::Pixel cellColour(int nCell)
    {
        sCell cell = cells[nCell];

        if (cell.bHasMine)
            return gck::WHITE;

        return countColours[cell.nMineCount];
    }
    // return the cell colour if not visible and marked
    gck::Pixel cellMarked(int nCell)
    {
        sCell cell = cells[nCell];

        if (cell.bFlagged)
            return gck::RED;

        return gck::GREY;
    }
    // recursively reveal cells look up, down, left and right
    void floodReveal(int x, int y)
    {   // cant be bomb if here because entry is by a 0 non bomb and bombs are surrounder by non zero mine counts
        if (cells[y * nCells + x].bVisible)
            return;

        cells[y * nCells + x].bVisible = true;
        nNonVisibleCells--;

        if (cells[y * nCells + x].nMineCount != 0)
            return;
        // look up, down, left and right
        if (x > 0)
            floodReveal(x - 1, y); // left
        if (x < nCells - 1)
            floodReveal(x + 1, y); // right
        if (y > 0)
            floodReveal(x, y - 1); // up
        if (y < nCells - 1)
            floodReveal(x, y + 1); // down
    }

public:
    bool appCreate() override
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        std::string hm = "Hidden Mines > ";
        int nTotalCells = nCells * nCells;
        // create the cell nodes
        if (cells != nullptr)
            delete cells;

        cells = new sCell[nTotalCells];
        nNonVisibleCells = nCells * nCells;
        // populate with mines
        nMines = 0;

        for (int i = 0; i < gck::rndGenerator(11, 16); i++)
        {
            int nMineCell = rand() % nTotalCells;

            if (!cells[nMineCell].bHasMine)
                nMines++;

            cells[nMineCell].bHasMine = true;
        }
        // calculate surrounding mine count for each cell
        for (int x = 0; x < nCells; x++)
            for (int y = 0; y < nCells; y++)
            {   // if cell has a mine, increase mine count around it
                if (cells[y * nCells + x].bHasMine)
                {
                    if (x > 0 && y > 0) // top left
                        cells[(y - 1) * nCells + (x - 1)].nMineCount++;
                    if (x > 0) // middle left
                        cells[y * nCells + (x - 1)].nMineCount++;
                    if (x > 0 && y < nCells - 1) // bottom left
                        cells[(y + 1) * nCells + (x - 1)].nMineCount++;
                    if (y > 0) // top
                        cells[(y - 1) * nCells + x].nMineCount++;
                    if (y < nCells - 1) // bottom
                        cells[(y + 1) * nCells + x].nMineCount++;
                    if (x < nCells - 1 && y > 0) // top right
                        cells[(y - 1) * nCells + (x + 1)].nMineCount++;
                    if (x < nCells - 1) // middle right
                        cells[y * nCells + (x + 1)].nMineCount++;
                    if (x < nCells - 1 && y < nCells - 1) // bottom right
                        cells[(y + 1) * nCells + (x + 1)].nMineCount++;
                }
            }
        // clean up
        Clear(gck::BLACK);
        // draw blank grid
        for (int x = 0; x <= nCells; x++)
        {
            DrawLine(gck::vi2d(nCellWidth, nCellWidth + (x * (nCellWidth + 1))), gck::vi2d((nCells + 1) * (nCellWidth + 1) - 1, nCellWidth + (x * (nCellWidth + 1))), gck::WHITE);
            DrawLine(gck::vi2d(nCellWidth + (x * (nCellWidth + 1)), nCellWidth), gck::vi2d(nCellWidth + (x * (nCellWidth + 1)), (nCells + 1) * (nCellWidth + 1)), gck::WHITE);
        }
        // draw total mines to find
        DrawString(gck::vi2d(ScreenWidth() / 2 - 34, 7), sAppName, gck::DARK_YELLOW, 1);
        hm += std::to_string(nMines);
        DrawString(gck::vi2d(ScreenWidth() / 2 - 48, ScreenHeight() - 18), hm, gck::DARK_MAGENTA, 1);

        return true;
    }

    bool appUpdate(float fElapsedTime) override
    {
        if (GetKey(gck::Key::ESCAPE).bPressed)
            return false;
        // get mouse position in cell size
        int nSelectedCellX = GetMouseX() / (nCellWidth + 1) - 1;
        int nSelectedCellY = GetMouseY() / (nCellWidth + 1) - 1;
        // if mouse pressed, show the cell if not already visible
        if (GetMouse(0).bPressed)
        {   // if in game over state, restart game
            if (bGameOver)
            {
                bGameOver = false;
                appCreate();
                return true;
            }
            // if mouse is within a valid cell, reveal it if not already visible.
            if ((nSelectedCellX >= 0 && nSelectedCellX < nCells) && (nSelectedCellY >= 0 && nSelectedCellY < nCells))
                if (!cells[nSelectedCellY * nCells + nSelectedCellX].bVisible)
                {   // boom!
                    if (cells[nSelectedCellY * nCells + nSelectedCellX].bHasMine)
                    {
                        for (int x = 0; x < nCells; x++)
                            for (int y = 0; y < nCells; y++)
                                cells[y * nCells + x].bVisible = true;
                        // game over
                        bGameOver = true;
                        DrawString(gck::vi2d(ScreenWidth() / 2 - 24, 192), loser, gck::RED, 1);
                    }
                    // if cell is not a bomb and has no bombs near, recursively flood fill around it
                    else if (cells[nSelectedCellY * nCells + nSelectedCellX].nMineCount == 0)
                        floodReveal(nSelectedCellX, nSelectedCellY);
                    else // must be a non bomb but with neighbours, just reveal the cell
                    {
                        cells[nSelectedCellY * nCells + nSelectedCellX].bVisible = true;
                        nNonVisibleCells--;
                    }
                }
        }
        // if right mouse button pressed, flag or unflag cell
        if (GetMouse(1).bPressed)
        {
            if ((nSelectedCellX >= 0 && nSelectedCellX < nCells) && (nSelectedCellY >= 0 && nSelectedCellY < nCells))
                if (!cells[nSelectedCellY * nCells + nSelectedCellX].bVisible)
                    cells[nSelectedCellY * nCells + nSelectedCellX].bFlagged = !cells[nSelectedCellY * nCells + nSelectedCellX].bFlagged;
        }
        // draw all the visible cells
        for (int x = 0; x < nCells; x++)
            for (int y = 0; y < nCells; y++)
                if (cells[y * nCells + x].bVisible)
                {
                    DrawRect(gck::vi2d((nCellWidth + 1) * (x + 1) + nCellWidthBy2, (nCellWidth + 1) * (y + 1) + nCellWidthBy2), nCellWidth, nCellWidth, 0, gck::BLACK, true);
                    std::string v = cellContents(y * nCells + x);
                    DrawString(gck::vi2d((nCellWidth + 1) * (x + 1) + nCellWidthBy2, (nCellWidth + 1) * (y + 1) + nCellWidthBy2), v, cellColour(y * nCells + x), 1);
                }
                else {
                    DrawRect(gck::vi2d((nCellWidth + 1) * (x + 1) + nCellWidthBy2, (nCellWidth + 1) * (y + 1) + nCellWidthBy2), nCellWidth, nCellWidth, 0, cellMarked(y * nCells + x), true);
                    // add a little char for info
                    if (cellMarked(y * nCells + x) == gck::RED)
                        DrawString(gck::vi2d((nCellWidth + 1) * (x + 1) + nCellWidthBy2, (nCellWidth + 1) * (y + 1) + nCellWidthBy2), flag, gck::DARK_RED, 1);
                }
        // you win!
        if (!bGameOver && nNonVisibleCells == nMines)
        {
            bGameOver = true;
            DrawString(gck::vi2d(ScreenWidth() / 2 - 24, 192), winner, gck::GREEN, 1);
            gck::vi2d lol = { GetMouseX(), GetMouseY() };
        }

        return true;
    }
};

int main()
{   // hide console
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    // create object instance
    MineSweeper* game = new MineSweeper;
    // game features
    if (game->Construct(190, 200, 4, 4, false, true, true, false))
        game->Start();
    // clean up
    delete game;

    return 0;
}
