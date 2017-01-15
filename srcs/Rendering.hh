#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>

#include "Scoring.hh"
#include "Tetromino.hh"

constexpr int BOARD_WIDTH	= CELL_SIZE * H_CELL_NUMBER;
constexpr int BOARD_HEIGHT	= CELL_SIZE * V_CELL_NUMBER;
constexpr int RIGHT_SIDE	= 200;
constexpr int SCREEN_WIDTH	= BOARD_WIDTH + RIGHT_SIDE;
constexpr int SCREEN_HEIGHT	= BOARD_HEIGHT;
constexpr int NEXT_SQUARE_LENGTH = 6 * CELL_SIZE;
constexpr int NEXT_SQUARE_HEIGHT = 4 * CELL_SIZE;
constexpr int FONT_SIZE		= 17;
constexpr int MIDDLE_MARGIN	= 27;
constexpr int RIGHT_COLUMN_START = BOARD_WIDTH + MIDDLE_MARGIN;

class Rendering {
public:
  Rendering();
  void	refresh();
  void	drawNextTetromino(const Tetromino &tetromino);
  void	drawCurrentTetromino(const Tetromino &tetromino);
  void	drawBoard(const int board[][V_CELL_NUMBER]);
  void	clearPreviousTetromino(const Tetromino &tetromino);
  void	drawScore(unsigned int score);
  void	drawBestScores(const std::vector<Score> &scores);

  ~Rendering();

private:
  void	setColor(const Color color);
  void	drawBlock(const SDL_Rect &block) const;
  void	drawRightSquare();
  void	drawGrid();
  void	renderText();
  void	renderText(const std::string &txt, int x, int y);

  SDL_Window		*_window	= NULL;
  SDL_Renderer		*_renderer	= NULL;
  TTF_Font		*_font		= NULL;
  const SDL_Color	_black		= {0, 0, 0, 0};
  const std::string	_windowTitle	= "MinimaTetris";
  const std::string	_fontTitle	= "./fonts/Celestial.ttf";
};
