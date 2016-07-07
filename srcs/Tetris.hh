#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <functional>

#include "RandomGenerator.hpp"
#include "Rendering.hh"
#include "AudioManager.hh"

class Tetris {
public:
  Tetris();

  int	run();
  void	auto_move_down();

private:

  bool	floor_standing()	const;
  void	check_lines();
  bool	move_down();
  void	new_tetromino();
  bool	fast_placing();
  bool	move_left();
  bool	move_right();
  bool	rotate();

  std::map<int, std::function<bool()> > _functions;
  RandomGenerator &_rg		= RandomGenerator::getInstance();
  int		_board[H_CELL_NUMBER + 1][V_CELL_NUMBER] = { {WHITE} }; // WHITE = 0
  Tetromino	_tetromino;
  Tetromino	_nextTetromino;
  int		_linesCleared	= 0;
  Rendering	_rendering;
  AudioManager	_audioManager;
  SDL_TimerID	_timerID;
  bool		_timerRunning	= false;
  int		_current_time	= 700;
  unsigned int	_score		= 0;
};
