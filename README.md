# SlitherlinkNX

## Introduction

Slitherlink is a logic puzzle game created in 1989 by Japanese publisher Nikoli,
famous for creating Sudoku, among many other puzzle games.
See https://en.wikipedia.org/wiki/Slitherlink for more info.

I discovered the game a few years ago on the Nintendo DS with 'Puzzle Series
Vol. 5: Slitherlink', which was never released outside of Japan. It's such a
good game I became addicted to it, just like with Picross and Nurikabe.

This is my version of the game for the Nintendo Switch. It's a rewrite of one
of my previous projects for the Dingoo A320. It features 456 levels in four
sizes: 5x5 (easy), 7x7, 10x10, 20x20 (hard). There's also a save state function,
undo button, and more.

## Rules

In SlitherlinkNX, you draw lines on a grid to create a meandering path, following
the rules:

* The path must form a __single loop__, without crossing itself or branching.
* The numbers indicate how many lines surround each cell. Empty cells may be
  surrounded by any number of lines.

There is one unique solution for each level.
You can draw small x's on segments that cannot be connected.
You should be able to find the solution by logical deduction only, without guessing.

For Slitherlink beginners, I strongly recommend to consult the Wikipedia page,
especially chapter 'Solution methods', which is a big help at the start:
https://en.wikipedia.org/wiki/Slitherlink#Solution_methods

## Controls

* d-pad: move cursor
* X/A/B/Y: draw a line, then a cross, then nothing, in the corresponding direction
* L/ZL + X/A/B/Y: draw a cross, then a line, then nothing, in the corresponding direction
* R/ZR: undo previous action
* +: open menu, allowing to: retry, save state, or quit

## Installation

Download the latest release, unpack, and copy directory 'SlitherlinkNX' to the
SD card, in /switch/. Then, launch with the homebrew menu.

## Compilation

You'll need devkitpro, and the packages switch-sdl2, switch-sdl2_image,
switch-sdl2_mixer. Then: 'make -f makefile.switch'.
It's based on SDL2, so it can be built for PC too.

## History

v1.1:

* Recompile with latest libnx to fix the buttons
* Undo = R/ZR, loop back = L/ZL

v1.0

* First release

## Credits

* Development: Tardigrade (http://beyondds.free.fr/)
* Original game: Nikoli (http://www.nikoli.com/)
* Puzzles: Krazydad (http://www.krazydad.com/slitherlink/)
* Font: Chopin Script
* Sounds: Junggle
* Background: aopsan
