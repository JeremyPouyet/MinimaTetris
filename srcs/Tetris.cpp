#include "Tetris.hh"

Tetris::Tetris() try : // function try block to handle error in Rendering()
  _functions({
      {SDLK_DOWN,	std::bind(&Tetris::move_down, this)},
      {SDLK_LEFT,	std::bind(&Tetris::move_left, this)},
      {SDLK_RIGHT,	std::bind(&Tetris::move_right, this)},
      {SDLK_UP,		std::bind(&Tetris::rotate, this)},
      {SDLK_SPACE,	std::bind(&Tetris::fast_placing, this)}
    }),
    _nextTetromino(tetrominos[_rg.i_between(0, tetrominos.size() - 1)]),
    _rendering()
    {
    }
catch (const char *ex) {
  throw ex;
}

void	Tetris::reset() {
  unsigned int x, y;

  for (x = 0; x < H_CELL_NUMBER + 1; x++)
    for (y = 0; y < V_CELL_NUMBER; y++)
      _board[x][y] = WHITE;
  _score	= 0;
  _linesCleared = 0;
  _current_time = _defaultTime;
  _rendering.drawBoard(_board);
  _rendering.drawScore(_score);
  new_tetromino();
  _rendering.refresh();
}

bool	Tetris::floor_standing() const {
  for (const auto &block : _tetromino._blocks)
    if (block.y == V_CELL_NUMBER - 1 || _board[block.x][block.y + 1] != WHITE)
      return true;
  return false;
}

void		Tetris::check_lines() {
  int		y, y1, x;

  // for each line
  for (y = V_CELL_NUMBER - 1; y > 0 && _board[H_CELL_NUMBER][y] != 0;) {
    if (_board[H_CELL_NUMBER][y] == H_CELL_NUMBER) { // if line is full
      ++_linesCleared;
      _board[H_CELL_NUMBER][y] = 0;
      // replace each cell by its y - 1 neighbour, or by WHITE if y1 == 0
      for (y1 = y; y1 > 1; y1--)
	for (x = 0; x < H_CELL_NUMBER + 1; x++)
	  _board[x][y1] = (y1 > 0) ? _board[x][y1 - 1] : WHITE;
    }
    else
      --y;
  }

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
** move current tetromino down
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

bool	Tetris::gameOver() const {
  for (const auto &block : _tetromino._blocks) {
    if (_board[block.x][block.y] != WHITE)
      return true;
  }
  return false;
}

void	Tetris::new_tetromino() {
  if (_timerRunning) {
    SDL_RemoveTimer(_timerID);
    _timerRunning = false;
  }
  _tetromino.reset();
  _tetromino	= _nextTetromino;
  if (gameOver() == true) {
    reset();
    return;
  }
  _nextTetromino = tetrominos[_rg.i_between(0, tetrominos.size() - 1)];
  _timerID	= SDL_AddTimer(_current_time, __c_auto_move_down, this);
  _timerRunning	= true;
  _current_time -= 2;
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
  if (_tetromino.rotate(_board) == true) {
    _audioManager.play("rotate");
    return true;
  }
  return false;
}

int		Tetris::run() {
  SDL_Event	e;
  bool		quit	= false;
  bool		moved	= false;

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
