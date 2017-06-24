#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <functional>
#include <chrono>
#include <thread>

#include "RandomGenerator.hpp"
#include "Rendering.hh"
#include "AudioManager.hh"
#include "Scoring.hh"

constexpr int FAST_PLACING_REFRESH_RATE = 4;

class Tetris {
public:
  Tetris();

  void	run();

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
  RandomGenerator &_rg	= RandomGenerator::getInstance();
  int		_board[H_CELL_NUMBER + 1][V_CELL_NUMBER] = { {WHITE} }; // WHITE = 0
  AudioManager	_audioManager;
  Tetromino	_tetromino;
  Tetromino	_nextTetromino;
  Rendering	_rendering;
  Scoring	_scoring;
  unsigned int	_score = 0;
  int		_current_time;
  unsigned int	_linesCleared;
  bool		_moved;
  unsigned int	_refreshRate;
};
