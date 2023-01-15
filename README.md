# Tank-Test
My answer to C++ Fast Track for Games Programming Part 11

https://www.3dgep.com/cpp-fast-track-11-tiles/

# Controls
WASD/Arrow keys to turn and move.

Zoom with + or - keys (3 levels).

# Notes
Wanted to get more comfortable using const variables and functions.

Modified BUas template DrawScaled() to handle a Sprite() with multiple frames.

Wanted to change size of drawings without having the tank and tilemap class be aware of the new size.
Became tricky with the tank, as its sprite is not the same size as the tilemap sprites (50x50 vs 32x32).

Approach was to think of each tilemap tile as 1 pixel in size, and created the tank's collision corner values within that space.
These values do not change as the zoom level changes, keeping the tank in the same relative position on the map as zoom changes.
Tank sprite also had to be offset so it was drawn within its collision box.
