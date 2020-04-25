//
// Created by Josh on 4/22/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include "alien_wave.h"
#include "direction.h"
#include "player.h"
namespace space_invader {
class Engine {
 public:
  Engine::Engine();
  AlienWave GetAlienWave() const;
  Player Engine::GetPlayer() const;
  //Engine(int width, int height);
  Location Engine::MoveAlienWave(bool toRight, int count);
  Location Engine::MovePlayer(const Direction direction);
  void Step();
  void PlayerStep();

  // Changes the direction of the player for the next time step.
  void SetDirection(Direction);
 private:
  AlienWave alien_wave;
  int height = 16;
  int width = 16;
  Direction direction_;
  Player player;
  bool isFirst = true;
  int count = 0;
  bool toRight = true;

};
}


#endif  // FINALPROJECT_ENGINE_H
