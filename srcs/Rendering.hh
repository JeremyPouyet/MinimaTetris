#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Tetromino.hh"

class Rendering {
public:
  bool	initialize();
  void	refresh();

  void	drawNextTetromino(Tetromino &tetromino);
  void	drawCurrentTetromino(Tetromino &tetromino);
  void	drawBoard(const int board[][V_CELL_NUMBER]);
  void	clearPreviousTetromino(Tetromino &tetromino);
  ~Rendering();

private:
  SDL_Window	*_window	= NULL;
  SDL_Renderer	*_renderer	= NULL;
  TTF_Font	*_font		= NULL;

  void	setColor(Color color);
  void	drawBlock(SDL_Rect &block) const;
  void	drawRightSquare();
  void	drawGrid();
  void	renderText();
};
