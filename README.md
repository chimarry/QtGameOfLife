# Conway game of life

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

A library that simulates steps of [Conway Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), implemented using OpenCL and Qt framework.

## Game rules
Game space is represented with a two-dimensional net of cells, where each cell can be in state DEAD or ALIVE. Each cell can interact with neighbours. In one iteration of the game, the state of cells are changed using the following rules:
1) if the cell is alive and has less than two alive neighbours, it becomes dead,
2) if the cell is alive and has two or three alive neighbours, it is passed to the next iteration,
3) if the cell is dead and has exactly three alive neighbours, it becomes alive,
4) if the cell is alive and has more than three alive neighbours, it becomes dead

## Features
- Enable user to create two-dimensional space for game, with custom width and height
- User should be able to get a specific part of the game's space as a black-white image of custom size
- Enable user to go to a custom iteration of the game

## Implementation details
- Each feature is implemented as a separate kernel,
- If dimensions of space are big, divide job in separate kernel calls,
- Allocate space for the game on the device,
- Using Qt framework, create client's application that uses previously created library, simulates Game of Life with previously described options (export as an image, go to specific iteration, create a matrix with custom dimensions...).