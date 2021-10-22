#include "../lib/olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <time.h>

struct Cell {
    int x;
    int y;

    Cell operator+(const Cell& other){
        return Cell{x+other.x, y+other.y};
    }

    bool operator==(const Cell& other){
        return x==other.x && y == other.y;
    }
};

class GOLGame {
public:
    std::vector<Cell> new_cells;
    std::vector<Cell> cells;
    time_t last_time;
    int max_x = 20; 
    int max_y = 20;

    Cell neighbour_offsets[8] = {
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
        {-1,0},
        {-1,-1},
        {0,-1},
        {1,-1}
    };


    GOLGame()
    {
        last_time = time(NULL);
        //initial state

        //glider
        cells = {
            Cell{0, 2},
            Cell{1, 3},
            Cell{2, 1},
            Cell{2, 2},
            Cell{2, 3},
        };

        //cells = {
        //    Cell{4,4},
        //    Cell{5,4},
        //    Cell{4,5},
        //    Cell{5,5},
        //};

        new_cells = {};
    }

    void run_rules(){
        for (Cell cell : cells){
            int c = neighbour_count(cell);
            if ( 2 <= c && c <= 3){
                new_cells.push_back(cell);
            }
        }

        for (Cell cell : cells){
            for (Cell offset : neighbour_offsets){
                Cell trial = cell+offset;
                if (is_dead(trial) && !cell_in_vector(trial, new_cells) && neighbour_count(trial) == 3){
                    new_cells.push_back(trial);
                }
            }
        }
    }

    bool cell_in_vector(Cell cell, std::vector<Cell> vec){
        for (Cell c : vec)
            if (cell == c)
                return true;
        return false;
    }

    bool is_dead(Cell cell){
        return !cell_in_vector(cell, cells);
    }

    int neighbour_count(Cell cell){
        int count = 0;

        for (Cell offset : neighbour_offsets)
            if (!is_dead(offset + cell)) 
                count++;

        return count;
    }

    bool update(){
        time_t current_time = time(NULL);
        if (current_time != last_time){

            run_rules();
            cells = new_cells;
            new_cells = {};

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

    //top left cell pos
    int xPos = 0;
    int yPos = 0;
    //The pixel offset to start drawing at
    int xCellOffset = 10;
    int yCellOffset = 10;

    //How many pixels to draw each cell with
    int xCellPixels = 20;
    int yCellPixels = 20;

    //Border style
    int LRborderWidth = 20;
    int TBborderWidth = 20;
    int cellBorderWidth = 2;

    //The total number of cells that are displayed, rounded up
    int xCellCount, yCellCount;

    olc::Pixel BorderColour = olc::DARK_GREY;
    olc::Pixel EmptyCellColour = olc::GREY;
    olc::Pixel FilledCellColour = olc::YELLOW;

	bool OnUserCreate() override
	{
        //ChangeZoom();
        //drawEmptyBoard();
		return true;
	}

    void ChangeZoom() {
        //xCellPixels = (ScreenWidth() - 2*LRborderWidth - (xCellCount-1)*cellBorderWidth)/xCellCount;
        //yCellPixels = (ScreenHeight() - 2*TBborderWidth - (yCellCount-1)*cellBorderWidth)/yCellCount;
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
        //draw();
        highlightCell(Cell{0,0});
        highlightCell(Cell{1,0});
        highlightCell(Cell{0,1});
        highlightCell(Cell{1,1});
		return true;
	}

    void draw(){
        if (game.update()) {
            clearAllCells();
            for (Cell c : game.cells){
                highlightCell(c);
            }
        }
    }

    void drawEmptyBoard(){
        drawBorders();
        draw();
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
        if (cell.x < xPos || cell.y < yPos) return;

        int onScreenOffsetX = cell.x - xPos;
        int actualXRoot;

        if (onScreenOffsetX == 0){
            actualXRoot = LRborderWidth;
        } else {
            actualXRoot = xCellPixels*(onScreenOffsetX-1) + xCellOffset + onScreenOffsetX*cellBorderWidth + LRborderWidth;
        }
        if (actualXRoot > ScreenWidth() - LRborderWidth) return;

        int onScreenOffsetY = cell.y - yPos;
        int actualYRoot;

        if (onScreenOffsetY == 0){
            actualYRoot = TBborderWidth;
        } else {
            actualYRoot = yCellPixels*(onScreenOffsetY-1) + yCellOffset + onScreenOffsetY*cellBorderWidth + TBborderWidth;
        }
        if (actualYRoot > ScreenHeight() - TBborderWidth) return;

        int xWidth = onScreenOffsetX== 0 ? xCellPixels-xCellOffset : xCellPixels;
        int yWidth = onScreenOffsetY== 0 ? yCellPixels-yCellOffset : yCellPixels;

        std::cout << actualXRoot << " " << actualYRoot << " " << actualXRoot << std::endl;
        FillRect(actualXRoot, actualYRoot, xWidth, yWidth, colour);
    }
};


int main()
{
	GOLGraphics gol;
	if (gol.Construct(490, 490, 4, 4))
		gol.Start();

	return 0;
}