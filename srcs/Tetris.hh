#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <functional>

#include "RandomGenerator.hpp"
#include "Rendering.hh"
#include "AudioManager.hh"
#include "Scoring.hh"

class Tetris {
public:
  Tetris();

  void	run();
  void	auto_move_down();

private:
  bool	floor_standing()	const;
  void	check_lines();
  void	move_down();
  void	new_tetromino();
  void	fast_placing();
  void	move_left();
  void	move_right();
  void	rotate();
  void	reset();
  bool	gameOver()		const;

  static constexpr unsigned int	_defaultTime = 700;
  const std::map<int, std::function<void()> > _functions;
  RandomGenerator &_rg		= RandomGenerator::getInstance();
  int		_board[H_CELL_NUMBER + 1][V_CELL_NUMBER] = { {WHITE} }; // WHITE = 0
  AudioManager	_audioManager;
  Tetromino	_tetromino;
  Tetromino	_nextTetromino;
  Rendering	_rendering;
  SDL_TimerID	_timerID;
  Scoring	_scoring;
  unsigned int	_linesCleared	= 0;
  bool		_timerRunning	= false;
  int		_current_time	= _defaultTime;
  unsigned int	_score		= 0;
  bool		_moved		= false;
};
