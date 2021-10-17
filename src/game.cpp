#include "../lib/olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <time.h>

struct Cell {
    int x;
    int y;
};

class GOLGame {
public:
    std::vector<Cell> cells;
    time_t last_time;
    int max_x = 20; 
    int max_y = 20;

    GOLGame()
    {
        last_time = time(NULL);
        cells = {
            Cell{0, 0}
        };
    }

    bool update(){
        time_t current_time = time(NULL);
        if (current_time != last_time){
            cells[0].x++;
            cells[0].y++;

            if (cells[0].x >= max_x || cells[0].y >= max_y){
                cells[0].x = 0;
                cells[0].y = 0;
            }
            last_time = current_time;
            return true;
        }

        return false;
    }

};

class GOLGraphics : public olc::PixelGameEngine
{
public:
	GOLGraphics()
	{
		sAppName = "Game Of Life";
	}

public:
    GOLGame game;
    int xCellCount = 20;
    int yCellCount = 20;
    int LRborderWidth = 20;
    int TBborderWidth = 20;
    int cellBorderWidth = 2;
    int xCellPixels;
    int yCellPixels;

    olc::Pixel BorderColour = olc::DARK_GREY;
    olc::Pixel EmptyCellColour = olc::GREY;
    olc::Pixel FilledCellColour = olc::YELLOW;

	bool OnUserCreate() override
	{
        xCellPixels = (ScreenWidth() - 2*LRborderWidth - (xCellCount-1)*cellBorderWidth)/xCellCount;
        yCellPixels = (ScreenHeight() - 2*TBborderWidth - (yCellCount-1)*cellBorderWidth)/yCellCount;
        drawEmptyBoard();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        bool should_update = game.update();

        if (should_update) {
            clearAllCells();
            for (Cell c : game.cells){
                highlightCell(c);
            }
        }
		return true;
	}

    void drawEmptyBoard(){
        drawBorders();
        clearAllCells();
    }
    void clearAllCells(){
        for (int x = 0; x < xCellCount; x++){
            for (int y = 0; y < yCellCount; y++){
                clearCell(Cell{x, y});
            }
        }
    }
    void drawBorders(){
        FillRect(0, 0, ScreenWidth(), TBborderWidth, BorderColour);
        FillRect(0, TBborderWidth + yCellCount * yCellPixels + cellBorderWidth*(yCellCount-1), ScreenWidth(), TBborderWidth, BorderColour);
        FillRect(0, 0, LRborderWidth, ScreenHeight(), BorderColour);
        FillRect(LRborderWidth + xCellCount * xCellPixels + cellBorderWidth*(xCellCount-1), 0 , LRborderWidth, ScreenHeight(), BorderColour);

        for (int i = 1; i < xCellCount; i++){
            FillRect(LRborderWidth + (xCellPixels + cellBorderWidth) * i - cellBorderWidth, TBborderWidth, cellBorderWidth, ScreenHeight()-2*TBborderWidth, BorderColour);
        }        
        for (int i = 1; i < yCellCount; i++){
            FillRect(TBborderWidth, TBborderWidth + (yCellPixels + cellBorderWidth) * i - cellBorderWidth, ScreenWidth()-2*LRborderWidth, cellBorderWidth, BorderColour);
        }        
    }

    void clearCell(Cell cell){
        colourCell(cell, EmptyCellColour);
    }

    void highlightCell(Cell cell){
        colourCell(cell, FilledCellColour);
    }

    void colourCell(Cell cell, olc::Pixel colour){
        int xPx = LRborderWidth + cell.x*(xCellPixels + cellBorderWidth);
        int yPx = TBborderWidth + cell.y*(yCellPixels + cellBorderWidth);

        FillRect(xPx, yPx, xCellCount, yCellCount, colour);
    }
};


int main()
{
	GOLGraphics gol;
	if (gol.Construct(490, 490, 4, 4))
		gol.Start();

	return 0;
}