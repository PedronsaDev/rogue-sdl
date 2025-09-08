#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

#define GRID_WIDTH 80
#define GRID_HEIGHT 24

struct MainCharacter
{
    int health;
    int mana;
    int pos_x;
    int pos_y;
    char last_floor_tile;

} player;

char d_grid[GRID_WIDTH][GRID_HEIGHT];

void RG_InitializeGrid()
{
    for (int x = 0; x < GRID_WIDTH; x++)
    {
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            d_grid[x][y] = ' ';
        }
    }
}

void RG_SetCell(int x, int y, char c)
{
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
    {
        return;
    }
    d_grid[x][y] = c;
}

char RG_GetCell(int x, int y)
{
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
    {
        return ' ';
    }
    return d_grid[x][y];
}

void RG_DrawStats(SDL_Renderer *renderer)
{
    // SDL_FRect rect;

    // SDL_SetRenderDrawColor(renderer, 245, 245, 245, SDL_ALPHA_OPAQUE);
    // rect.x = 0;
    // rect.y = WINDOW_HEIGHT - 50;
    // rect.w = WINDOW_WIDTH;
    // rect.h = 50;
    // SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, 10, WINDOW_HEIGHT - 40, "Health: 100");
    SDL_RenderDebugText(renderer, 150, WINDOW_HEIGHT - 40, "Mana: 50");
}

void RG_DrawRoom(SDL_Renderer *renderer)
{
    const int roomSizeX = 20;
    const int roomSizeY = 5;

    for (int x = 0; x < roomSizeX; x++)
    {
        for (int y = 0; y < roomSizeY; y++)
        {
            if (x == 0 || x == roomSizeX - 1 || y == 0 || y == roomSizeY - 1)
            {
                RG_SetCell(x + 10, y + 5, '#');

                if (x == roomSizeX / 2 && y == 0)
                {
                    RG_SetCell(x + 10, y + 5, '+');
                }
                
            }
            else
            {
                RG_SetCell(x + 10, y + 5, '.');
            }
        }
    }
}

void RG_DrawGrid(SDL_Renderer *renderer)
{

    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            float cell_w = (float)WINDOW_WIDTH / GRID_WIDTH;
            float cell_h = (float)(WINDOW_HEIGHT - 50) / GRID_HEIGHT;

            SDL_FRect cell = {
                i * cell_w,
                j * cell_h,
                cell_w,
                cell_h};

            // Cell background
            // SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
            // SDL_RenderFillRect(renderer, &cell);

            // SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
            // SDL_RenderRect(renderer, &cell);

            char text[2] = {d_grid[i][j], 0};
            int char_size = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
            int tx = (int)(cell.x + (cell_w - char_size) / 2);
            int ty = (int)(cell.y + (cell_h - char_size) / 2);

            SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
            SDL_RenderDebugText(renderer, tx, ty, text);
        }
    }
}

void RG_MovePlayer(int dx, int dy)
{
    int new_x = player.pos_x + dx;
    int new_y = player.pos_y + dy;

    if (new_x < 0 || new_x >= GRID_WIDTH || new_y < 0 || new_y >= GRID_HEIGHT)
    {
        return;
    }

    if (d_grid[new_x][new_y] == '#')
    {
        return;
    }

    RG_SetCell(player.pos_x, player.pos_y, player.last_floor_tile);
    player.pos_x = new_x;
    player.pos_y = new_y;
    player.last_floor_tile = RG_GetCell(player.pos_x, player.pos_y);
    RG_SetCell(player.pos_x, player.pos_y, '@');
}

void RG_DrawGame()
{
    const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    RG_DrawGrid(renderer);
    
    // RG_DrawStats(renderer);

    SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Debug Texture", "1.0", "com.example.renderer-debug-text");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/debug-text", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    RG_InitializeGrid();

    RG_DrawRoom(renderer);

    player.health = 100;
    player.mana = 50;
    player.pos_x = GRID_WIDTH / 2;
    player.pos_y = GRID_HEIGHT / 2;
    player.last_floor_tile = '.';
    RG_SetCell(player.pos_x, player.pos_y, '@');

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    SDL_Keycode sym = event->key.key;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (sym == SDLK_W)
        {
           RG_MovePlayer(0, -1);
        }
        else if (sym == SDLK_S)
        {
            RG_MovePlayer(0, 1);
        }
        else if (sym == SDLK_A)
        {
            RG_MovePlayer(-1, 0);
        }
        else if (sym == SDLK_D)
        {
            RG_MovePlayer(1, 0);
        }
        else if (sym == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
            
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    RG_DrawGame();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}