#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stddef.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define T_SHAPE 0
#define LL_SHAPE 1
#define LR_SHAPE 2
#define SQUARE_SHAPE 3
#define I_SHAPE 4

#define EMPTY_CELL '.'
#define OCCUPIED_CELL '+'

const char SHAPES[5][64] = {
    ".+.."
        "+++."
        "...."
        "...."

        ".+.."
        ".++."
        ".+.."
        "...."

        "...."
        "+++."
        ".+.."
        "...."

        "...."
        "+++."
        ".+.."
        "....",

    // L facing left
    "..+."
        "..+."
        "+++."
        "...."

        "+..."
        "++++"
        "...."
        "...."

        "++.."
        "+..."
        "+..."
        "...."

        "++++"
        "+..."
        "...."
        "....",

    // L facing right
    "+..."
        "+..."
        "++.."
        "...."

        "++++"
        "+..."
        "...."
        "...."

        "++.."
        ".+.."
        ".+.."
        "...."

        "..+."
        "+++."
        "...."
        "....",

    // SQUARE SHAPE
    "++.."
        "++.."
        "...."
        "...."

        "++.."
        "++.."
        "...."
        "...."

        "++.."
        "++.."
        "...."
        "...."

        "++.."
        "++.."
        "...."
        "....",

    // I SHAPE
    "+..."
        "+..."
        "+..."
        "+..."

        "++++"
        "...."
        "...."
        "...."

        "+..."
        "+..."
        "+..."
        "+..."

        "++++"
        "...."
        "...."
        "...."
};

typedef size_t Shape;
typedef size_t Rotation;

    static char
ShapeCellAt(Shape shape, Rotation rotation, int x, int y)
{
    return SHAPES[shape][(rotation * 16) + (y * 4) + x];
}

typedef struct {
    Shape shape;
    Rotation rotation;
    int x;
    int y;
} Chunk;

#define ROWS 20
#define COLS 15
#define TOTAL_CELLS (ROWS * COLS)

static int board[TOTAL_CELLS];

    static void
InitBoard()
{
    SDL_memset(board, 0, sizeof(board));
}

    static int
CellAt(int x, int y)
{
    return board[y * ROWS + x];
}

#define CELL_MARGIN 3
#define CELL_SIZE 24

    static void
DrawCell(SDL_Renderer* renderer, float x, float y)
{
    SDL_FRect rect = {
        .x = (float) (CELL_SIZE * x) + (CELL_MARGIN * x), .y = (float) (CELL_SIZE * y) + (CELL_MARGIN * y),
        .w = CELL_SIZE - 1, .h = CELL_SIZE - 1
    };
    SDL_RenderFillRect(renderer, &rect);
}

    static void
DrawBoard(SDL_Renderer* renderer)
{
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (CellAt(x, y) == 0) {
                SDL_SetRenderDrawColor(renderer, 0xee, 0xdd, 0xff, 0xff);
            } else {
                SDL_SetRenderDrawColor(renderer, 0xee, 0x30, 0x11, 0xff);
            }

            DrawCell(renderer, x, y);
        }
    }
}    

Chunk player;

static void InitPlayer()
{
    player = (Chunk) {
        .shape = T_SHAPE,
            .rotation = 0,
            .x = 0,
            .y = 0
    };
}

    static void
DrawPlayer(SDL_Renderer* renderer)
{
    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            if (ShapeCellAt(player.shape, player.rotation, x, y) == OCCUPIED_CELL) {
                SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
                DrawCell(renderer, x, y);
            }
        }
    }
}

int main(int argc, char* args[])
{
    SDL_Init( SDL_INIT_VIDEO );

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer("Untitled", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    InitBoard();

    bool quit = false;
    SDL_Event e;
    SDL_zero(e);
    while (quit == false)
    {
        while (SDL_PollEvent(&e) == true)
        {
            if (e.type == SDL_EVENT_QUIT)
                quit = true;
            else if (e.type == SDL_EVENT_KEY_DOWN)
                if (e.key.scancode == SDL_SCANCODE_ESCAPE)
                    quit = true;
                else if(e.key.scancode == SDL_SCANCODE_X)
                    player.rotation = (player.rotation + 1) % 4;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);

        DrawBoard(renderer);
        DrawPlayer(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
