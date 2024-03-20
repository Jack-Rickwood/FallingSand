# FallingSand

A falling sand cellular automata in the style of noita and powder toy.

![Image1](images/Image1.png)

## Controls
Changing brush:
- Tab = reset
- Minus = small brush
- Equals = large brush
- Right bracker = sparse brush

Changing particle type:
- S = sand
- W = water
- D = dirt
- R = stone
- A = acid
- Q - smoke

## Features to add
Features:
- Rigidbodies
- New Particles:
    - Grass?
    - Smoke
    - Fire
    - Fungus
- Variation in particle color
- Make embers cool over time (so they dissapear)

## Issues
- Change getPath to only return adjacent space moves
- Disperse sand in water to look more natural (higher chance to swap if more dense?)
- Water doesn't spread fast enough (change direction (< 50%) instead of random dir?)
- Some materials should be destroyed by acid before others
- Code is very ugly
- Fix placing. Some probable issues:
    - Particles should have outward (and maybe upward?) starting velocity
    - Velocity should not cut off if it has to move diagonally, only if it has no options left
