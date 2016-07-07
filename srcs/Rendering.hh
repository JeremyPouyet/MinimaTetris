#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>

#include "Tetromino.hh"

class Rendering {
public:
  Rendering();
  void	refresh();

  void	drawNextTetromino(const Tetromino &tetromino);
  void	drawCurrentTetromino(const Tetromino &tetromino);
  void	drawBoard(const int board[][V_CELL_NUMBER]);
  void	clearPreviousTetromino(const Tetromino &tetromino);
  void	drawScore(unsigned int score);

  ~Rendering();

private:
  SDL_Window		*_window	= NULL;
  SDL_Renderer		*_renderer	= NULL;
  TTF_Font		*_font		= NULL;
  const SDL_Color	_black		= {0, 0, 0, 0};

  void	setColor(Color color);
  void	drawBlock(const SDL_Rect &block) const;
  void	drawRightSquare();
  void	drawGrid();
  void	renderText();
  void	renderText(const std::string &txt, int x, int y);
};
