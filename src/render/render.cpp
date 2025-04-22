#include <SDL2/SDL.h>
#include <cmath>

#include "../map/map.hpp"
#include "../game/player.hpp"

// https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
// https://www.doomworld.com/forum/topic/146286-why-is-software-rendering-so-different-from-hardware-rendering/

void render(SDL_Window *window, SDL_Surface *win_surface, const Player &player)
{
  // Fill the background with a light gray (used as floor/sky for now)
  SDL_FillRect(win_surface, nullptr, SDL_MapRGB(win_surface->format, 240, 240, 240));
  
  int screen_width = 0;
  int screen_height = 0;

  // Get current window dimensions
  SDL_GetWindowSize(window, &screen_width, &screen_height);

  // Cast one ray per vertical screen column
  for(int x = 0; x < screen_width; ++x) {
    
    // Map x coordinate to a value between -1 and 1 (center of screen is 0)
    float cameraX = 2 * x / float(screen_width) - 1;

    // Calculate ray angle: player direction + angle offset based on FOV
    float direction = player.dir + cameraX * (player.fov / 2.0f);
    
    // Ray direction vector
    float rayDirX = std::cos(direction);
    float rayDirY = std::sin(direction);

    // Current map square the player is in
    int mapX = player.x;
    int mapY = player.y;

    // Length of ray from one x/y side to the next x/y side
    float deltaDistX = std::abs(1.0f / rayDirX);
    float deltaDistY = std::abs(1.0f / rayDirY);

    int stepX = 0;
    int stepY = 0;
    
    float sideDistX = 0.0;
    float sideDistY = 0.0;

    // Calculate step direction and initial side distances for X
    if(rayDirX < 0) {
      stepX = -1;
      sideDistX = (player.x - mapX) * deltaDistX;
    }
    else {
      stepX = 1;
      sideDistX = (mapX + 1.0f - player.x) * deltaDistX;
    }

    // Same for Y
    if(rayDirY < 0) {
      stepY = -1;
      sideDistY = (player.y - mapY) * deltaDistY;
    }
    else {
      stepY = 1;
      sideDistY = (mapY + 1.0f - player.y) * deltaDistY;
    }

    bool hit = false;
    int side = 0; // Was the wall hit on an x-side (0) or y-side (1)?

    // Perform DDA: step through the grid until a wall is hit
    while(!hit) {
      if(sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }
      else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }

      // Check if the ray has hit a wall
      if(map[mapY][mapX] > 0) {
        hit = true;
      }
    }

    // Calculate distance to wall (perpendicular to camera plane)
    float perpWallDist = 0.0;
      
    if (side == 0)
      perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayDirX;
    else
      perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayDirY;

    // Calculate height of the line to draw
    int lineHeight = (int)(screen_height / perpWallDist);

    // Calculate lowest and highest pixel to fill in the current stripe
    int drawStart = -lineHeight / 2 + screen_height / 2;
    if (drawStart < 0) drawStart = 0;

    int drawEnd = lineHeight / 2 + screen_height / 2;
    if (drawEnd >= screen_height) drawEnd = screen_height - 1;

    // Choose wall color (darker if it's a Y-side for shading effect)
    Uint32 wallColor = SDL_MapRGB(win_surface->format, 100, 100, 100);
    if (side == 1) {
      wallColor = SDL_MapRGB(win_surface->format, 70, 70, 70);
    }

    // Draw the vertical line (wall slice) to the pixel buffer
    for (int y = drawStart; y < drawEnd; ++y) {
      Uint32 *pixels = (Uint32*)win_surface->pixels;
      pixels[y * (win_surface->pitch / 4) + x] = wallColor;
    }
  }
}
