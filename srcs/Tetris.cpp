#include "Tetris.hh"

static const std::vector<Tetromino> tetrominos{
  {BLUE, { {4, 0}, {5, 0}, {4, 1}, {5, 1} }, 0, false }, // O
  {YELLOW, { {5, 0}, {4, 1}, {5, 1}, {6, 1} }, 2 }, // T
  {PURPLE, { {3, 0}, {4, 0}, {5, 0}, {6, 0} }, 2 }, // I
  {PINK, { {4, 0}, {5, 0}, {6, 0}, {6, 1} }, 1 }, // reverse L
  {ORANGE, { {4, 0}, {5, 0}, {6, 0}, {4, 1} }, 1 }, // L
  {GREEN, { {4, 0}, {5, 0}, {5, 1}, {6, 1} }, 2 }, // Z
  {RED, { {5, 0}, {6, 0}, {4, 1}, {5, 1} }, 0}  // S
};

Tetris::Tetris() :
  _functions({
      {SDLK_DOWN,	std::bind(&Tetris::move_down,	this)},
      {SDLK_LEFT,	std::bind(&Tetris::move_left,	this)},
      {SDLK_RIGHT,	std::bind(&Tetris::move_right,	this)},
      {SDLK_UP,		std::bind(&Tetris::rotate,	this)},
      {SDLK_SPACE,	std::bind(&Tetris::fast_placing, this)},
      {SDLK_KP_PLUS,	std::bind(&AudioManager::increaseVolume, &_audioManager)},
      {SDLK_KP_MINUS,	std::bind(&AudioManager::decreaseVolume, &_audioManager)}
    }),
  _nextTetromino(tetrominos[_rg.i_between(0, tetrominos.size() - 1)])
{}

void	Tetris::reset() {
  unsigned int x, y;

  for (x = 0; x < H_CELL_NUMBER + 1; x++)
    for (y = 0; y < V_CELL_NUMBER; y++)
      _board[x][y] = WHITE;
  if (_score > 0)
    _scoring.addScore(_score);
  _score	= 0;
  _linesCleared = 0;
  _moved	= false;
  _refreshRate	= 20;
  _current_time = _defaultTime;
  _rendering.drawBoard(_board);
  _rendering.drawScore(_score);
  _rendering.drawBestScores(_scoring.getBestScores(10));
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
  unsigned int	y, y1, x;

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
    _audioManager.play("destroy");
  }
  else
    _audioManager.play("pose");
}

/*
** move current tetromino down
** @return wether the tetromino has moved
*/
void	Tetris::move_down() {
  if (floor_standing()) {
    for (const auto &block : _tetromino._blocks) {
      ++_board[H_CELL_NUMBER][block.y]; // increase the number of block in line block.y
      _board[block.x][block.y] = _tetromino._color;
    }
    check_lines();
    new_tetromino();
    _rendering.drawBoard(_board);
    _rendering.drawScore(_score);
    _rendering.refresh();
    _moved = false;
    return;
  }
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    ++block.y;
  _moved = true;
}

bool	Tetris::gameOver() const {
  for (const auto &block : _tetromino._blocks)
    if (_board[block.x][block.y] != WHITE)
      return true;
  return false;
}

void	Tetris::new_tetromino() {
  _tetromino.reset();
  _tetromino		= _nextTetromino;
  _nextTetromino	= tetrominos[_rg.i_between(0, tetrominos.size() - 1)];
  _current_time	-= 2;
  _rendering.drawNextTetromino(_nextTetromino);
}

void	Tetris::fast_placing() {
  unsigned int currentTime, lastTime = 0;

  do {
    currentTime = SDL_GetTicks();
    if (currentTime <= lastTime + FAST_PLACING_REFRESH_RATE)
      continue;
    lastTime = currentTime;
    move_down();
    if (_moved == true) {
      _rendering.clearPreviousTetromino(_tetromino);
      _rendering.drawCurrentTetromino(_tetromino);
      _rendering.refresh();
    }
  } while (_moved == true);
}

/*
** move current tetromino left
** @return wether the tetromino has moves
*/
void	Tetris::move_left() {
  for (const auto &block : _tetromino._blocks)
    if (block.x <= 0 || _board[block.x - 1][block.y] != WHITE)
      return;
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    --block.x;
  _audioManager.play("move");
  _moved = true;
}

/*
** move current tetromino right
** @return wether the tetromino has moves
*/
void	Tetris::move_right() {
  for (const auto &block : _tetromino._blocks)
    if (block.x >= H_CELL_NUMBER - 1 || _board[block.x + 1][block.y] != WHITE)
      return;
  _tetromino.saveBlocks();
  for (auto &block : _tetromino._blocks)
    ++block.x;
  _audioManager.play("move");
  _moved = true;
}

/*
** rotate the current tetromino
** @return wether the rotation has been done
*/
void		Tetris::rotate() {
  if (_tetromino.rotate(_board) == true) {
    _audioManager.play("rotate");
    _moved = true;
  }
}

void		Tetris::run() {
  SDL_Event	e;
  bool		quit(false);
  unsigned int	lastTime = 0, autoMoveTime = 0, currentTime;

  reset();
  do {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
	quit = true;
      currentTime = SDL_GetTicks();
      if (currentTime <= lastTime + _refreshRate)
	continue;
      if (e.type == SDL_KEYDOWN && _functions.count(e.key.keysym.sym) > 0)
	_functions.at(e.key.keysym.sym)();
      lastTime = currentTime;
    }
    currentTime = SDL_GetTicks();
    if (currentTime > autoMoveTime + _current_time) {
      move_down();
      autoMoveTime = currentTime;
    }
    if (_moved) {
      _rendering.clearPreviousTetromino(_tetromino);
      _rendering.drawCurrentTetromino(_tetromino);
      _rendering.refresh();
    }
    else if (gameOver())
      reset();
    _moved = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  } while (quit == false);
}
