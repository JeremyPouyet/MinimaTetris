#include "Tetris.hh"

Tetris::Tetris() :
  _functions({
      {SDLK_DOWN,	std::bind(&Tetris::move_down, this)},
      {SDLK_LEFT,	std::bind(&Tetris::move_left, this)},
      {SDLK_RIGHT,	std::bind(&Tetris::move_right, this)},
      {SDLK_UP,		std::bind(&Tetris::rotate, this)},
      {SDLK_SPACE,	std::bind(&Tetris::fast_placing, this)}
    }),
  _tetromino(tetrominos[_rg.i_between(0, tetrominos.size() - 1)]),
  _nextTetromino(tetrominos[_rg.i_between(0, tetrominos.size() - 1)]) {
}

bool	Tetris::floor_standing() {
  for (const auto &block : _tetromino._blocks)
    if (block.y == V_CELL_NUMBER - 1 || _board[block.x][block.y + 1] != WHITE)
      return true;
  return false;
}

void		Tetris::show_board() const {
  unsigned int	x, y;

  for (y = 0; y < V_CELL_NUMBER; y++) {
    for (x = 0; x < H_CELL_NUMBER + 1; x++)
      std::cout << _board[x][y] << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void		Tetris::check_lines() {
  int		y, y1, x, height = 0;
  int		start = 0;
  std::map<int, bool> done;

  for (const auto &block : _tetromino._blocks) {
    if (done.count(block.y) != 0)
      continue;
    done[block.y] = true;
    ++height; // compute tetromino's height
    if (block.y > start) // search highest block
      start = block.y;
  }
  for (y = start; y > start - height;) {
    if (_board[H_CELL_NUMBER][y] == H_CELL_NUMBER) { // if line is full
      ++_linesCleared;
      _board[H_CELL_NUMBER][y] = 0;
      for (y1 = y; y1 > 1; y1--)
	for (x = 0; x < H_CELL_NUMBER + 1; x++)
	  _board[x][y1] = _board[x][y1 - 1];
    }
    else
      --y;
  }
  for (x = 0; x < H_CELL_NUMBER + 1; x++)
    _board[x][0] = WHITE;
  if (_linesCleared > 0) {
    _score		+= _linesCleared * _linesCleared;
    _linesCleared	= 0;
    _rendering.drawBoard(_board);
    _rendering.drawScore(_score);
    _rendering.refresh();
    _audioManager.play("destroy");
  }
  else
    _audioManager.play("pose");
}

/*
** move current tetromni down
** @return wether the tetromino has moved
*/
bool	Tetris::move_down() {
  if (floor_standing()) {
    for (const auto &block : _tetromino._blocks) {
      ++_board[H_CELL_NUMBER][block.y];
      _board[block.x][block.y] = _tetromino._color;
    }
    check_lines();
    new_tetromino();
    return false;
  }
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    ++block.y;
  return true;
}

/*
** move current tetromino down every N ms
*/
void	Tetris::auto_move_down() {
  if (move_down() == false)
    return;
  _rendering.clearPreviousTetromino(_tetromino);
  _rendering.drawCurrentTetromino(_tetromino);
  _rendering.refresh();
}

static Uint32 __c_auto_move_down(Uint32 interval, void *that) {
  Tetris *tetris = static_cast<Tetris *>(that);
  tetris->auto_move_down();
  return interval;
}

void	Tetris::new_tetromino() {
  if (_timerRunning) {
    SDL_RemoveTimer(_timerID);
    _timerRunning = false;
  }
  _tetromino.reset();
  _tetromino	= _nextTetromino;
  _nextTetromino = tetrominos[_rg.i_between(0, tetrominos.size() - 1)];
  _timerID	= SDL_AddTimer(_current_time, __c_auto_move_down, this);
  _timerRunning	= true;
  _current_time -= 3;
  _rendering.drawNextTetromino(_nextTetromino);
}

bool	Tetris::fast_placing() {
  SDL_RemoveTimer(_timerID);
  _timerRunning = false;
  while (move_down()) {
    _rendering.clearPreviousTetromino(_tetromino);
    _rendering.drawCurrentTetromino(_tetromino);
    _rendering.refresh();
  }
  return true;
}

/*
** move current tetromino left
** @return wether the tetromino has moves
*/
bool	Tetris::move_left() {
  for (const auto &block : _tetromino._blocks)
    if (block.x <= 0 || _board[block.x - 1][block.y] != WHITE)
      return false;
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    --block.x;
  _audioManager.play("move");
  return true;
}

/*
** move current tetromino right
** @return wether the tetromino has moves
*/

bool	Tetris::move_right() {
  for (const auto &block : _tetromino._blocks)
    if (block.x >= H_CELL_NUMBER - 1 || _board[block.x + 1][block.y] != WHITE)
      return false;
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    ++block.x;
  _audioManager.play("move");
  return true;
}

/*
** rotate the current tetromino
** @return wether the rotation has been done
*/
bool		Tetris::rotate() {
  Move		move;
  int		x, y, nextMove	= _tetromino.getNextMove();
  unsigned int	i;

  if (nextMove == -1)
    return false;
  move = _tetromino._moves[nextMove];
  for (i = 0; i < _tetromino._blocks.size(); i++) {
    x = _tetromino._blocks[i].x + move[i].first;
    y = _tetromino._blocks[i].y + move[i].second;
    if (x < 0 || x >= H_CELL_NUMBER || y < 0
	|| y >= V_CELL_NUMBER || _board[x][y] != WHITE)
      return false;
  }
  _tetromino.saveBlocks();
  for (i = 0; i < _tetromino._blocks.size(); i++) {
    _tetromino._blocks[i].x += move[i].first;
    _tetromino._blocks[i].y += move[i].second;
  }
  _tetromino._nextMove = nextMove;
  _audioManager.play("rotate");
  return true;
}

int		Tetris::run() {
  SDL_Event	e;
  bool		quit	= false;
  bool		moved	= false;

  if (_rendering.initialize() == false)
    return 1;
  new_tetromino();
  while (quit == false && SDL_WaitEvent(&e) >= 0) {
    if (e.type == SDL_QUIT)
      quit = true;
    else if (e.type == SDL_KEYDOWN && _functions.count(e.key.keysym.sym) > 0) {
      moved = _functions[e.key.keysym.sym]();
      if (moved) {
	_rendering.clearPreviousTetromino(_tetromino);
	_rendering.drawCurrentTetromino(_tetromino);
	_rendering.refresh();
      }
    }
  }
  SDL_RemoveTimer(_timerID);
  return 0;
}
