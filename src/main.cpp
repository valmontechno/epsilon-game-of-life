#include "eadkpp.h"

using namespace EADK;

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Game of life";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

const Color Black(0x414041);
const Color White(0xeeeeee);

bool grid[Screen::Height][Screen::Width];

uint8_t countNeighbors(uint16_t x, uint16_t y) {
  uint8_t neighbors = 0;
  for (int8_t i = -1; i < 2; i++)
  {
    for (int8_t j = -1; j < 2; j++)
    {
      if (i == 0 && j == 0) continue;
      if (grid[(y + i) % Screen::Height][(x + j) % Screen::Width]){
        neighbors++;
      }
    }
  }
  return neighbors;
}

int main() {

  for (uint16_t i = 0; i < Screen::Height; i++)
  {
    for (uint16_t j = 0; j < Screen::Width; j++)
    {
      grid[i][j] = EADK::random() & 1;
    }
  }

  Keyboard::State keyState = Keyboard::scan();

  while (! (keyState.keyDown(Keyboard::Key::Home) || keyState.keyDown(Keyboard::Key::Power) || keyState.keyDown(Keyboard::Key::Back))) {
    keyState = Keyboard::scan();

    bool nextGrid[Screen::Height][Screen::Width];

    for (uint16_t y = 0; y < Screen::Height; y++)
    {
      for (uint16_t x = 0; x < Screen::Width; x++)
      {
        uint8_t neighbors = countNeighbors(x, y);
        bool nextState = grid[y][x] ? (neighbors == 2 || neighbors == 3) : (neighbors == 3);

        nextGrid[y][x] = nextState;

        Display::pushRectUniform(Rect(x, y, 1, 1), nextState ? Black : White);
      }
    }

    for (uint16_t i = 0; i < Screen::Height; i++)
    {
      for (uint16_t j = 0; j < Screen::Width; j++)
      {
        grid[i][j] = nextGrid[i][j];
      }
    }

  }

  do {
     keyState = Keyboard::scan();
  } while (keyState.keyDown(Keyboard::Key::Home) || keyState.keyDown(Keyboard::Key::Power) || keyState.keyDown(Keyboard::Key::Back));

  return 0;
}