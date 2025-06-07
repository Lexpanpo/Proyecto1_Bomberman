# Proyecto I
* [Github link](https://github.com/Lexpanpo/Proyecto1_Bomberman)
* [1 min video](https://youtu.be/6VbNzwcDbOw)

## Team Members
* Pol Cuenca Dueñas, [Lexpanpo](https://github.com/Lexpanpo)
* Andrea Vélez Sempere, [veawenn](https://github.com/veawenn)
* Dani Castillero Mariani, [DaniCasti42](https://github.com/DaniCasti42)

## Bomberman
Bomberman is a classic action-maze game originally released for the NES in 1985. Players control a robot named Bomberman who must navigate grid-based levels, placing bombs to destroy obstacles and defeat enemies. The goal is to clear each stage by eliminating all enemies and finding the hidden exit. With its simple yet addictive gameplay, Bomberman laid the foundation for a beloved franchise known for its strategic bomb-dropping mechanics and multiplayer fun.

## How to Play Bomberman
### Objective:
Clear each maze-like stage by defeating all enemies using bombs, then find the hidden exit under a destructible block.
* Move Bomberman through the grid-based maze.
* Drop bombs to destroy soft blocks and defeat enemies.
* Avoid getting caught in your own bomb blasts or touching enemies.
* Once all enemies are defeated, find the exit (hidden under a soft block) to move to the next level.
* Collect power-ups hidden in blocks to increase your abilities.

Use the arrow keys to move Bomberman up, down, left, and right. 
Press space to place a bomb. The ENTER key is used to start or close the game.
The key C erases all the soft blocks from the map, aswell as the key K kills all the enemies in the level. Lastly, the key P inflicts self-damage to the player.

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Bomberman Remake - UPC</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');

    body {
      font-family: 'Press Start 2P', cursive;
      background-color: #111;
      color: #fff200;
      margin: 0;
      padding: 20px;
      background-image: url('https://i.imgur.com/5r1e0Cz.png');
      background-size: cover;
      background-repeat: no-repeat;
      background-attachment: fixed;
    }

    header {
      background-color: #d90429;
      color: white;
      padding: 20px;
      text-align: center;
      border-radius: 10px;
      border: 4px solid #fff;
      box-shadow: 0px 0px 15px #fff200;
    }

    section {
      background: rgba(0, 0, 0, 0.85);
      padding: 20px;
      margin-top: 20px;
      border-radius: 10px;
      border: 2px solid #fff200;
      box-shadow: 0px 0px 10px #fff200;
    }

    h1, h2 {
      color: #ffffff;
    }

    ul {
      list-style-type: square;
      padding-left: 20px;
    }

    a {
      color: #00ffff;
      text-decoration: none;
    }

    a:hover {
      text-decoration: underline;
    }

    iframe {
      width: 100%;
      max-width: 560px;
      height: 315px;
      display: block;
      margin: 0 auto;
      border: 4px solid #fff200;
    }
  </style>
</head>
<body>
  <header>
    <h1>Bomberman Remake - UPC</h1>
    <p><strong>University Project - Team UPC</strong></p>
  </header>

  <section>
    <h2>Welcome</h2>
    <p>This project is a faithful remake of <strong>Bomberman</strong> developed in <strong>C++</strong> using the <strong>Raylib</strong> library as part of our university coursework at <strong>UPC</strong>.</p>
    <p><em>🎮 Student project — no commercial intent. Enjoy the nostalgia!</em></p>
  </section>

  <section>
    <h2>Game Overview</h2>
    <p>A simplified Bomberman-style game featuring classic mechanics:</p>
    <ul>
      <li>Strategic bomb placement to defeat enemies</li>
      <li>Maze layout with destructible walls</li>
      <li>Enemies with basic AI</li>
      <li>Power-ups (bomb range, extra bomb)</li>
      <li>Collision detection and game states</li>
    </ul>
    <p><strong>Win:</strong> Eliminate all enemies or reach the exit<br>
       <strong>Lose:</strong> Run out of lives or get hit by an explosion</p>
  </section>

  <section>
    <h2>Debug Features</h2>
    <ul>
      <li>Toggleable hitboxes and collision zones</li>
      <li>Visual overlays for bomb range and enemy paths</li>
    </ul>
  </section>

  <section>
    <h2>Meet the Team</h2>
    <ul>
      <li><strong>[Name 1]:</strong> Lead Programmer, Bomb Mechanics</li>
      <li><strong>[Name 2]:</strong> Level Design & Enemy AI</li>
      <li><strong>[Name 3]:</strong> UI, Power-Up System & Debug Tools</li>
    </ul>
    <p>Students of Game Design and Programming at UPC.</p>
  </section>

  <section>
    <h2>Individual Contributions</h2>
    <ul>
      <li><strong>[Name 1]:</strong> Bomb logic, movement, collisions (⏱️ X hours)</li>
      <li><strong>[Name 2]:</strong> Maze design, destructibles, AI (⏱️ Y hours)</li>
      <li><strong>[Name 3]:</strong> UI, power-ups, debug overlays (⏱️ Z hours)</li>
    </ul>
  </section>

  <section>
    <h2>Gameplay & Debug Video</h2>
    <iframe src="https://www.youtube.com/embed/YOUR_VIDEO_ID" allowfullscreen></iframe>
    <p>Includes:</p>
    <ul>
      <li>Intro: UPC logo, team credits</li>
      <li>Gameplay: bomb mechanics, enemy interaction</li>
      <li>Debug: hitboxes, AI paths, bomb ranges</li>
    </ul>
  </section>

  <section>
    <h2>Download the Game</h2>
    <p><a href="ENLACE_A_TU_ZIP">👉 Download the latest build (.zip)</a></p>
    <p>Compatible with Windows. Unzip and run <code>Bomberman.exe</code>.<br>
    Supports keyboard and gamepad.</p>
  </section>
</body>
</html>
