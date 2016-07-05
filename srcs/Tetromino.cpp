#include "Tetromino.hh"

Tetromino::Tetromino(Color color, const Pos &start, const std::vector<Move> &moves) :
  _color(color), _default(start), _moves(moves) {
  reset();
}

void Tetromino::reset() {
  _blocks.clear();
  for (const auto &pos :_default)
    _blocks.push_back({pos.first, pos.second,
	  CELL_SIZE - GRID_THICKNESS,
	  CELL_SIZE - GRID_THICKNESS});
  _nextMove = 0;
}

void Tetromino::saveBlocks() {
  _savedBlocks = _blocks;
}

int Tetromino::getNextMove() const {
  if (_moves.size() == 0)
    return -1;
  if (_nextMove + 1 >= _moves.size())
    return 0;
  return _nextMove + 1;
}
