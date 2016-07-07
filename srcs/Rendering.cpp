#include "Rendering.hh"

Rendering::Rendering() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
    throw "SDL could not initialize: " + std::string(SDL_GetError());
  if (TTF_WasInit() == false && TTF_Init() == -1)
    throw "Can't initialize SDF TTF: " + std::string(TTF_GetError());
  _window = SDL_CreateWindow("Minima tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (_window == NULL)
    throw "_Window could not be created: " + std::string(SDL_GetError());
  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  if (_renderer == NULL)
    throw "Renderer could not be created: " + std::string(SDL_GetError());
  //Celestial, Italipixel
  if ((_font = TTF_OpenFont("./fonts/Celestial.ttf", 16)) == NULL)
    throw "Can't open font: " + std::string(TTF_GetError());
  TTF_SetFontKerning(_font, 0);
  setColor(WHITE);
  SDL_RenderClear(_renderer);
  drawGrid();
  drawRightSquare();
  renderText();
  SDL_RenderPresent(_renderer);
}

void		Rendering::renderText(const std::string &txt, int x, int y) {
  SDL_Surface*	surface = TTF_RenderText_Blended(_font, txt.c_str(), _black);
  SDL_Texture*	texture	= SDL_CreateTextureFromSurface(_renderer, surface);
  SDL_Rect	messageRect = {x, y, 0, 0};

  SDL_QueryTexture(texture, NULL, NULL, &messageRect.w, &messageRect.h);
  SDL_RenderCopy(_renderer, texture, NULL, &messageRect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void		Rendering::renderText() {
  renderText("Next : ", BOARD_WIDTH + 27, 1);
  renderText("Score : 0", BOARD_WIDTH + 27, NEXT_SQUARE_HEIGHT + 30);
}

void			Rendering::drawScore(unsigned int score) {
  std::ostringstream	converter;
  SDL_Rect		square = {BOARD_WIDTH + 99, NEXT_SQUARE_HEIGHT + 32,
				  101, 20};
  setColor(WHITE);
  SDL_RenderFillRect(_renderer, &square);
  converter << score;
  renderText(converter.str(), BOARD_WIDTH + 100, NEXT_SQUARE_HEIGHT + 30);
}

void	Rendering::drawRightSquare() {
  setColor(BLACK);
  // left border
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH + 27, 25,
		     BOARD_WIDTH + 27, 25 + NEXT_SQUARE_HEIGHT);
  // right border
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH + 27 + NEXT_SQUARE_LENGTH, 25,
		     BOARD_WIDTH + 27 + NEXT_SQUARE_LENGTH, 25 + NEXT_SQUARE_HEIGHT);
  // upper border
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH + 27, 25,
		     BOARD_WIDTH + 27 + NEXT_SQUARE_LENGTH, 25);
  // down border
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH + 27, NEXT_SQUARE_HEIGHT + 25,
		     BOARD_WIDTH + 27 + NEXT_SQUARE_LENGTH, NEXT_SQUARE_HEIGHT + 25);
}

void		Rendering::drawNextTetromino(const Tetromino &tetromino) {
  SDL_Rect	square = {
    BOARD_WIDTH + 28, 26,
    NEXT_SQUARE_LENGTH - 1, NEXT_SQUARE_HEIGHT - 1
  };

  // clear previous tetromino
  setColor(WHITE);
  SDL_RenderFillRect(_renderer, &square);
  square.h = CELL_SIZE - 1;
  square.w = CELL_SIZE - 1;
  // draw next tetromino
  setColor(tetromino._color);
  for (const auto &block : tetromino._blocks) {
    square.x = BOARD_WIDTH + 25 + (block.x - 2) * CELL_SIZE;
    square.y = 25 + (1 + block.y) * CELL_SIZE;
    SDL_RenderFillRect(_renderer, &square);
  }
}

void		Rendering::drawGrid() {
  unsigned int	y, x;

  setColor(GREY);
  for (y = 1; y < V_CELL_NUMBER; y++)
    for (x = 1; x < H_CELL_NUMBER; x++) {
      SDL_RenderDrawLine(_renderer, x * CELL_SIZE, 0, x * CELL_SIZE, V_CELL_NUMBER * CELL_SIZE);
      SDL_RenderDrawLine(_renderer, 0, y * CELL_SIZE, H_CELL_NUMBER * CELL_SIZE, y * CELL_SIZE);
    }
  setColor(BLACK);
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH, BOARD_HEIGHT, BOARD_WIDTH, 0);
  SDL_RenderDrawLine(_renderer, BOARD_WIDTH + 1, BOARD_HEIGHT, BOARD_WIDTH + 1, 0);
}

void		Rendering::setColor(Color color) {
  RGB_Color	c = colors.at(color);

  SDL_SetRenderDrawColor(_renderer, c[0], c[1], c[2], 255);
}

void	Rendering::drawBlock(const SDL_Rect &block) const {
  SDL_Rect square = {
    block.x * CELL_SIZE + 1,
    block.y * CELL_SIZE + 1,
    CELL_SIZE - 1, CELL_SIZE - 1
  };

  SDL_RenderFillRect(_renderer, &square);
}

void	Rendering::refresh() {
  SDL_RenderPresent(_renderer);
}

void		Rendering::drawBoard(const int board[][V_CELL_NUMBER]) {
  int		x, y;
  SDL_Rect	block;

  for (y = 0; y < V_CELL_NUMBER; y++)
    for (x = 0; x < H_CELL_NUMBER; x++) {
      block = {x, y, CELL_SIZE - GRID_THICKNESS, CELL_SIZE - GRID_THICKNESS};
      setColor((Color)board[x][y]);
      drawBlock(block);
    }
}

void	Rendering::clearPreviousTetromino(const Tetromino &tetromino) {
  setColor(WHITE);
  for (const auto &block : tetromino._savedBlocks)
    drawBlock(block);
}

void	Rendering::drawCurrentTetromino(const Tetromino &tetromino) {
  setColor(tetromino._color);
  for (const auto &block : tetromino._blocks)
    drawBlock(block);
}

Rendering::~Rendering() {
  TTF_CloseFont(_font);
  TTF_Quit();
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
