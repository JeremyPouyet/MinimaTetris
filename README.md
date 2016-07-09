# MinimaTetris ?

Last night, it was hard for me to sleep, so I created this little Tetris. It's not finished yet, and I'll update it later with an Artificial Intelligence.

## Install:

```
$> sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
$> git clone https://github.com/JeremyPouyet/MinimaTetris.git
$> cd MinimaTetris/ && make
$> ./MinimaTetris
```

## How to play:

The game start directly after the launch, then:  

* left arrow: move current tetromino left  
* right arrow: move current tetromino right  
* down arrow: move current tetromino down  
* up arrow: rotate current tetromino  
* space: fast placing, current tetromino will move down until it encounter another tetromino  

After a game over, the game will be relaunched automatically.
