# MageMan 2D Game

MageMan is a 2D game developed in C++ using SFML. The game features a complete animation system, collision system and more. The game includes a menu and several levels. The game has a variety of controls, including the ability to move, jump, shoot fireballs, take a screenshot, and toggle different display options.

<img src="screenshots/game.png?raw=true" alt= “a” width="320" height="240">
<img src="screenshots/game2.png?raw=true" alt= “a” width="320" height="240">

## Prerequisites

Before running the game, you must have SFML installed on your system. You can download and install SFML from the official website: https://www.sfml-dev.org/.

## Installation

To run the game, you can clone the repository and then run the solution. Here are the steps to do so:

1. Clone the repository to your local machine using Git or by downloading the ZIP file.
2. Open the solution in your C++ IDE of choice.
3. Build and run the solution.

## Usage

When you run the game, you will be taken to the menu. To navigate the menu, use the W key to move up, the S key to move down, Spacebar to select a level, and Escape to quit the game.

Once you select a level, you will be taken to the gameplay screen. Here are the controls for the gameplay screen:

- `W`: Jump
- `A`: Move left
- `D`: Move right
- `Spacebar`: Shoot a fireball
- `X`: Take a screenshot which will be saved next to the game files
- `G`: Toggle drawing of grids
- `C`: Toggle drawing of collision boxes
- `T`: Toggle drawing of textures
- `Escape`: to exit the level 

In addition to these controls, you can also drag and drop blocks by using the left mouse button to drag and moving the mouse, and then pressing the left mouse button again to drop the block.

You can also add your own textures and animations to the game and edit levels by editing their respective text files contained in the Assets folder and the Levels folder. the text files syntax will be described below

## Text Files Format

### Assets text file

the assets file consists of three different parts: Textures, Animations and Fonts which have the following formats:

```txt
Texture N P:
    N: texture name
    P: texture path

Animation N Tex F D:
    N: animation name
    Tex: texture used for this animation
    F: Number of frames in the animation
    D: duration of each frame

Font N P:
    N: font name
    P: font path
```

NOTE: you must ADD a texture before using it for animation

### Levels text files

each level text file consists of three different parts: level background, tiles to be placed, and player specification.
there are two types of tiles, a normal tile denoted by `Tile` and a decorative tile denoted by `Dec`, which doesn't have a bounding box for collisions. You can customize the levels by changing the arrangement of the tiles in the text files using the following syntax:

```txt
Background N    NOTE: must be placed at start of file
    N: background name

Tile N GX GY:
    N: animation name
    GX: grid X position
    GY: grid Y position

Dec N GX GY:
    N: animation name
    GX: grid X position
    GY: grid Y position

Player GX GY CX CY SPD MAX_SPD JMP G WEAP
    GX: grid X position for player to spawn
    GY: grid Y position for player to spawn
    CX: collision width
    CY: collision height
    SPD: player speed
    MAX_SPD: maximum player speed
    JNP: player jump speed
    G: player gravity
    WEAP: Animation to use for the weapon
```

## TODO

- add enemies spawning system
- add sound effects to the game
- create a level designer/editor accessable from the menu

## Credits

Floor Textures and decorations are taken from oak_woods free texture pack on itch.io
other textures are taken from OpenGameArt.org
