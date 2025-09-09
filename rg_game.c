#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define GRID_WIDTH 80
#define GRID_HEIGHT 24

struct Tile {
  char symbol;
  SDL_Color color;
  bool walkable;
};

struct PlayerCharacter {
  struct Tile tile;
  int health;
  int mana;
  int pos_x;
  int pos_y;
  char last_floor_tile;
};

struct Tile d_grid[GRID_WIDTH][GRID_HEIGHT];

void InitializeGrid() {
  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      struct Tile newTile;

      newTile.color = (SDL_Color){255, 255, 255, 255};
      newTile.walkable = true;
      newTile.symbol = '.';

      d_grid[x][y] = newTile;
    }
  }
}

#pragma region Game_Functions

#pragma endregion
#pragma region Main_Functions
void Awake() { InitializeGrid(); }

void Start() {}

void Update() {}

#pragma endregion

#pragma region SDL_Functions

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Example Renderer Debug Texture", "1.0",
                     "com.example.renderer-debug-text");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("examples/renderer/debug-text", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  Awake();

  Start();

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  SDL_Keycode sym = event->key.key;

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (sym == SDLK_W) {
    } else if (sym == SDLK_S) {
    } else if (sym == SDLK_A) {
    } else if (sym == SDLK_D) {
    } else if (sym == SDLK_ESCAPE) {
      return SDL_APP_SUCCESS;
    }
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  Update();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}

#pragma endregion