#include "Tetromino.hh"

Tetromino::Tetromino(Color color, const Pos &start, unsigned int center, bool move)
  : _color(color), _default(start), _center(center), _move(move) {
  reset();
}

void		Tetromino::reset() {
  _blocks.clear();
  for (const auto &pos :_default)
    _blocks.push_back({pos.first, pos.second, CELL_SIZE - GRID_THICKNESS, CELL_SIZE - GRID_THICKNESS});
}

void		Tetromino::saveBlocks() {
  _savedBlocks = _blocks;
}

bool		Tetromino::rotate(const int board[][V_CELL_NUMBER]) {
  SDL_Rect	center = _blocks[_center];
  int		tx, ty;

  if (_move == false) // block does not move, like the O block
    return false;
  saveBlocks(); // save tetromino before any change
  for (auto &block : _blocks) {
    if (block.x == center.x && block.y == center.y)
      continue;
    // translate
    tx = block.x - center.x;
    ty = (block.y - center.y) * - 1; // * -1 because y axis is reversed
    //  rotate
    block.x = (int)std::round(tx * std::cos(-M_PI/2) - ty * std::sin(-M_PI/2));
    block.y = ((int)std::round(tx * std::sin(-M_PI/2) + ty * std::cos(-M_PI/2))) * - 1;
    // untranslate
    block.x += center.x;
    block.y += center.y;
    // if the tetromino is OOB or rotate on an existing block, undo modifications
    if (block.x >= H_CELL_NUMBER || block.x < 0 ||
	block.y >= V_CELL_NUMBER || block.y < 0 ||
	board[block.x][block.y] != WHITE) {
      rollBack();
      return false;
    }
  }
  return true;
}

void	Tetromino::rollBack() {
  _blocks = _savedBlocks;
}
