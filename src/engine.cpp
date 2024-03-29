//
// Created by Josh on 4/22/2020.
//

#include "engine.h"
#include "Action.h"
#include "alien.h"
#include "projectile.h"

namespace space_invader {
Engine::Engine():
player(Location(18,45)),
action_player{Action::kStop},
action_player_projectile{Action::kStop},
action_alien_projectile{Action::kShoot},
action_nyan_cat{Action::kStop},
nyan_cat(Location(-3, 4)){
  player_projectile = new Projectile(player.GetLocation());
}

AlienWave Engine::GetAlienWave() const {
  return alien_wave;
}
Player Engine::GetPlayer() const {
  return player;
}
Projectile* Engine::GetProjectile() const {
  return player_projectile;
}
Projectile* Engine::GetAlienProjectile() const {
  return alien_projectile;
}
NyanCat Engine::GetNyanCat() const {
  return nyan_cat;
}
int Engine::GetScore() const {
  return score;
}
int Engine::GetHighScore() const {
  return high_score;
}


void Engine::AlienStep() {
  spawn_nyan_cat++;
  for (Alien& alien : alien_wave) {
    Location alien_loc = MoveAlienWave(toRight, count_to_border);
    Location new_alien_loc = (alien.GetLocation() + alien_loc);
    alien.SetLocation(new_alien_loc);
    if (alien.GetLocation().Col() == player.GetLocation().Col()
    && alien.IsVisibile()) {
      player.SetVisibility(false);
    }
  }
  count_to_border++;
  if (count_to_border == kDistanceToOppositeBorder) {
    toRight = !toRight;
    count_to_border = 0;
  }
}

void Engine::PlayerStep() {
  Location player_location = MovePlayer();
  Location new_player_loc =
      (player.GetLocation() + player_location) % Location(kDimension, kDimension);
  player.SetLocation(new_player_loc);
  if (action_player_projectile != Action::kShoot) {
    Location scaled(player.GetLocation().Row() + 1,
        player.GetLocation().Col());
    player_projectile->SetLocation(scaled);
  }
}

void Engine::PlayerProjectileStep() {
  if (action_player_projectile == Action::kShoot) {
    player_projectile->SetVisibility(true);
    Location projectile_location = MoveProjectile();
    Location new_projectile_loc =
        (player_projectile->GetLocation() + projectile_location);
    player_projectile->SetLocation(new_projectile_loc);
    projectile_distance++;
    for (Alien& alien : alien_wave) {
      Location alien_hitbox_1(alien.GetLocation().Row() + 1,
          alien.GetLocation().Col() + 1);
      Location alien_hitbox_2(alien.GetLocation().Row() + 2,
          alien.GetLocation().Col() + 1);
      if ((alien.GetLocation() == player_projectile->GetLocation()
      || alien_hitbox_1 == player_projectile->GetLocation()
      || alien_hitbox_2 == player_projectile->GetLocation())
      && alien.IsVisibile()) {
        score += 10;
        alien.SetVisibility(false);
        player_projectile->SetVisibility(false);
        delete player_projectile;
        Projectile* new_projectile = new Projectile(player.GetLocation());
        player_projectile = new_projectile;
        action_player_projectile = Action::kReload;
        projectile_distance = 0;
      }
    }
    if (projectile_distance == kDimension) {
      player_projectile->SetVisibility(false);
      delete player_projectile;
      Projectile* new_projectile = new Projectile(player.GetLocation());
      player_projectile = new_projectile;
      action_player_projectile = Action::kReload;
      projectile_distance = 0;
    }
    Location nyan_cat_hitbox_1(nyan_cat.GetLocation().Row() + 1,
        nyan_cat.GetLocation().Col());
    Location nyan_cat_hitbox_2(nyan_cat.GetLocation().Row() + 2,
        nyan_cat.GetLocation().Col());
    Location nyan_cat_hitbox_3(nyan_cat.GetLocation().Row() + 3,
        nyan_cat.GetLocation().Col());
    Location nyan_cat_hitbox_4(nyan_cat.GetLocation().Row() + 4,
        nyan_cat.GetLocation().Col());
    Location nyan_cat_hitbox_5(nyan_cat.GetLocation().Row() + 5,
        nyan_cat.GetLocation().Col());
    Location nyan_cat_hitbox_6(nyan_cat.GetLocation().Row() + 6,
        nyan_cat.GetLocation().Col());
    if (player_projectile->GetLocation() == nyan_cat.GetLocation()
    || player_projectile->GetLocation() == nyan_cat_hitbox_1
    || player_projectile->GetLocation() == nyan_cat_hitbox_2
    || player_projectile->GetLocation() == nyan_cat_hitbox_3
    || player_projectile->GetLocation() == nyan_cat_hitbox_4
    || player_projectile->GetLocation() == nyan_cat_hitbox_5
    || player_projectile->GetLocation() == nyan_cat_hitbox_6) {
     score += 50;
     nyan_cat.SetVisibility(false);
     player_projectile->SetVisibility(false);
     delete player_projectile;
     Projectile* new_projectile = new Projectile(player.GetLocation());
     player_projectile = new_projectile;
     action_player_projectile = Action::kReload;
     projectile_distance = 0;
    }
  }
}

void Engine::AlienProjectileStep() {
  Location projectile_location = MoveAlienProjectile();
  Location new_projectile_loc =
      (alien_projectile->GetLocation() + projectile_location);
  Location player_hitbox_1(player.GetLocation().Row() + 1,
      player.GetLocation().Col());
  Location player_hitbox_2(player.GetLocation().Row() + 2,
      player.GetLocation().Col());
  alien_projectile->SetLocation(new_projectile_loc);
  if (alien_projectile->GetLocation() == player.GetLocation()
  || alien_projectile->GetLocation() == player_hitbox_1
  || alien_projectile->GetLocation() == player_hitbox_2) {
    player.SetVisibility(false);
  }
  alien_projectile_distance++;
  if (alien_projectile_distance == kDimension) {
    int random_int = std::rand() % kNumberOfCols;
    int shoot_row = kNumberOfRows - 1;
    for (int i = 0; i < kNumberOfRows; i++) {
      if (alien_wave.GetAlien(shoot_row, random_int).IsVisibile() == false) {
        shoot_row--;
      }
    }
    alien_projectile_distance = 12;
    if (shoot_row != -1) {
      delete alien_projectile;
      Projectile* new_alien_projectile = new Projectile(
          alien_wave.GetAlien(shoot_row, random_int).GetLocation());
      Location scaled(new_alien_projectile->GetLocation().Row() + 1,
          new_alien_projectile->GetLocation().Col() + 2);
      new_alien_projectile->SetLocation(scaled);
      alien_projectile = new_alien_projectile;
      alien_projectile_distance = 0;
    }
  }
}

void Engine::NyanCatStep() {
  //18~

  if (spawn_nyan_cat == random_nyan_spawn) {
    action_nyan_cat = Action::kNyanRight;
  }
  if (action_nyan_cat == Action::kNyanRight) {
    nyan_cat_distance++;
  }
  if (nyan_cat_distance == kDimension + 3) {
    action_nyan_cat = Action::kStop;
    nyan_cat.SetLocation({-3, 4});
    nyan_cat_distance = 0;
  }
  Location nyan_cat_location = MoveNyanCat();
  Location new_nyan_cat_location = (nyan_cat.GetLocation() + nyan_cat_location);
  nyan_cat.SetLocation(new_nyan_cat_location);
}

void Engine::SetUpWave() {
  for (int i = 0; i < kNumberOfRows; i++) {
    for (int j = 0; j < kNumberOfCols; j++) {
      Location location(j * kNumberOfCols,
          i * kNumberOfCols + kNumberOfCols);
      alien_wave.FillWave(Alien(location));
    }
  }
  Location scaled(alien_wave.GetAlien(2, 1).GetLocation().Row() + 1,
      alien_wave.GetAlien(2, 1).GetLocation().Col() + 2);
  alien_projectile = new Projectile(scaled);
}

void Engine::SetDirection(Action action) {
  action_player = action;
}

void Engine::SetProjectileDirection(Action action) {
  action_player_projectile = action;
}

Location Engine::MoveAlienWave(bool toRight, int count_to_border) {
  if (toRight == false && count_to_border == 0) {
    return {0, +1};
  } else if (toRight == true && count_to_border == 0) {
    return {0, +1};
  } else if (toRight == true) {
    return {+1, 0};
  }
  return {-1, 0};
}

Location Engine::MoveProjectile() {
  if (action_player_projectile == Action::kShoot) {
    return {0, -1};
  }
  return {0, 0};
}

Location Engine::MoveAlienProjectile() {
    return {0, +1};
}

Location Engine::MoveNyanCat() {
  if (action_nyan_cat == Action::kNyanRight) {
    return {+1, 0};
  } else if (action_nyan_cat == Action::kStop) {
    return {0, 0};
  }
  return {0, 0};
}

Location Engine::MovePlayer() {
  if (action_player == Action::kRight) {
    return {+1, 0};
  } else if (action_player == Action::kLeft) {
    return {-1, 0};
  }
  return {0,0};
}
void Engine::ResetGame() {
  int n = 0;
  for (Alien& alien : alien_wave) {
    alien.SetVisibility(true);
    alien.SetLocation(GetInitialAlienPosition(n));
    n++;
  }
  random_nyan_spawn = std::rand() % 40 + 18;
  count_to_border = 0;
  projectile_distance = 0;
  alien_projectile_distance = 0;
  spawn_nyan_cat = 0;
  nyan_cat_distance = 0;
  if (score > high_score) {
    high_score = score;
  }
  score = 0;
  action_nyan_cat = Action::kStop;
  nyan_cat.SetVisibility(true);
  nyan_cat.SetLocation({-3, 4});
  player.SetVisibility(true);
  toRight = true;


}

void Engine::NextWave() {
  int n = 0;
  for (Alien& alien : alien_wave) {
    alien.SetVisibility(true);
    alien.SetLocation(GetInitialAlienPosition(n));
    n++;
  }
  random_nyan_spawn = std::rand() % 40 + 18;
  count_to_border = 0;
  projectile_distance = 0;
  alien_projectile_distance = 0;
  spawn_nyan_cat = 0;
  nyan_cat_distance = 0;
  action_nyan_cat = Action::kStop;
  nyan_cat.SetVisibility(true);
  toRight = true;
}

Location Engine::GetInitialAlienPosition(int n) {
  Location temp(0,0);
  int count = 0;
  for (int i = 0; i < kNumberOfRows; i++) {
    for (int j = 0; j < kNumberOfCols; j++) {
      if (count == n) {
        Location initial(j * kNumberOfCols,
            i * kNumberOfCols + kNumberOfCols);
        temp = initial;
      }
      count++;
    }
  }
  return temp;
}
}
