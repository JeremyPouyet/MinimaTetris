#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <cmath>

constexpr int H_CELL_NUMBER	= 10;
constexpr int V_CELL_NUMBER	= 24;
constexpr int GRID_THICKNESS = 1;
constexpr int CELL_SIZE	= 25;

enum Color {
  WHITE = 0,	BLUE,
  YELLOW,	PURPLE,
  PINK,		ORANGE,
  GREEN,	RED,
  GREY,		BLACK
};

using RGB_Color = std::vector<int>;
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
  Tetromino(Color color, const Pos &start, unsigned int center, bool move = true);
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
private:
  void	rollBack();
};
