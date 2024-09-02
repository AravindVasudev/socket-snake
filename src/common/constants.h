#pragma once

/*******************************************************************************
 * Game Constants
 ******************************************************************************/
constexpr int FRAME_RATE = 3; // 10 FPS

// Game maze size.
constexpr int WIDTH = 40;
constexpr int HEIGHT = 31;

// Snake starting position.
constexpr int SERVER_SNAKE_X = 20;
constexpr int SERVER_SNAKE_Y = 20;

constexpr int CLIENT_SNAKE_X = 25;
constexpr int CLIENT_SNAKE_Y = 25;

/*******************************************************************************
 * Server Constants
 ******************************************************************************/

constexpr int BACKLOG = 10; // The game only works with ONE client.