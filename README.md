# space-invaders-cpp
An object-oriented 2D arcade game built in C++, featuring multi-level modes, custom shooting mechanics, and distinct enemy behaviours.

## Game Modes
* **Arcade Mode**: A structured three-level campaign. Each level has unique enemy formations followed by a Boss Battle. You win when the Level 3 Boss is destroyed.
* **Survival Mode**: An endless loop of escalating waves with no bosses. The enemy count, speed, and fire rate increase with each wave.

## Enemy Types
All enemies are built using inheritance from a shared base class:
* **The Drone**: Moves straight downward at a constant speed and fires at random intervals.
* **The Viper**: Descends while continuously oscillating left and right in a sine-wave pattern.
* **The Seeker**: A kamikaze unit that locks onto the player's X coordinate and accelerates straight downward.

## Advanced Mechanics
* **Boss Mechanics**: Level 1 Cruiser fires walls of lasers with safe gaps. Level 2 Twin Cannons uses object composition, in which the core is immune until both turrets are destroyed. Level 3 Mothership spawns Seekers and fires a massive screen-wide laser.
* **Score Multiplier**: Points increase when enemies are killed within 3 seconds of each other (1x, 2x, 4x). Taking damage resets it to 1x.
* **Power-Up System**: Defeated enemies have a chance to drop upgrades like a Spread Module, Piercing Module, Energy Shield, or an EMP Screen Nuke.
* **Evasive Dash**: Teleports the ship a short distance with 0.5 seconds of invincibility and a 3-second cooldown.

## Credits
* **Sprites and Art**: Sourced and created with the help of my project partner.
