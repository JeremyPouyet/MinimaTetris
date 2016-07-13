#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <cmath>

//Screen dimension constants
const int H_CELL_NUMBER = 10;
const int V_CELL_NUMBER = 24;
const int GRID_THICKNESS = 1;
const int CELL_SIZE	= 25;
const int BOARD_WIDTH	= CELL_SIZE * H_CELL_NUMBER;
const int BOARD_HEIGHT	= CELL_SIZE * V_CELL_NUMBER;
const int RIGHT_SIDE	= 200;
const int SCREEN_WIDTH	= BOARD_WIDTH + RIGHT_SIDE;
const int SCREEN_HEIGHT = BOARD_HEIGHT;
const int NEXT_SQUARE_LENGTH = 6 * CELL_SIZE;
const int NEXT_SQUARE_HEIGHT = 4 * CELL_SIZE;

enum Color {
  WHITE = 0,	BLUE,
  YELLOW,	PURPLE,
  PINK,		ORANGE,
  GREEN,	RED,
  GREY,		BLACK
};

using RGB_Color = std::vector<int>;
using Move	= std::vector<std::pair<int, int> >;
using Pos	= std::vector<std::pair<int, int> >;

const std::map<Color, RGB_Color> colors = {
  {WHITE,	{255, 255, 255}},
  {BLUE,	{23, 184, 209}},
  {YELLOW,	{247, 247, 45}},
  {PURPLE,	{141, 23, 209}},
  {PINK,	{242, 104, 233}},
  {ORANGE,	{247, 200, 45}},
  {GREEN,	{91, 209, 23}},
  {RED,		{209, 48, 23}},
  {GREY,	{225, 225, 225}},
  {BLACK,	{0, 0, 0}}
};

struct Tetromino {
public:
  Tetromino(Color color, const Pos &start,
	    unsigned int center = 2, bool move = true);
  Tetromino() = default;
  void	reset();
  bool	rotate(const int board[][V_CELL_NUMBER]);
  void	saveBlocks();

  Color			_color;
  Pos			_default;
  unsigned int		_center;
  bool			_move;
  std::vector<SDL_Rect>	_blocks;
  std::vector<SDL_Rect> _savedBlocks;
  unsigned int		_nextMove = 0;
private:
  void	rollBack();
};

const std::vector<Tetromino> tetrominos{{
    BLUE, { // O
      {4, 0}, {5, 0}, {4, 1}, {5, 1}
    }, 0, false
	 }, {
    YELLOW, { // T
      {5, 0}, {4, 1}, {5, 1}, {6, 1}
    }
  }, {
    PURPLE, { // I
      {3, 0}, {4, 0}, {5, 0}, {6, 0}
    }
  }, {
    PINK, { // reverse L
      {4, 0}, {5, 0}, {6, 0}, {6, 1}
    }, 1
  }, {
    ORANGE, { // L
      {4, 0}, {5, 0}, {6, 0}, {4, 1}
    }, 1
  }, {
    GREEN, { // Z
      {4, 0}, {5, 0}, {5, 1}, {6, 1}
    }
  }, {
    RED, { // S
      {5, 0}, {6, 0}, {4, 1}, {5, 1}
    }, 0
  }
};
