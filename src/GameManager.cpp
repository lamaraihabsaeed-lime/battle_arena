#include <iostream>
#include "GameManager.h"
using namespace std;

{
GameManager::GameManager(Player* p,Enemy* e) : player(p), enemy(e), state(GameState::Playing)
{
   
}
  
void GameManager::handleCombat()

{
  float dx = player->getX() - enemy->getX();
  float dy = player->getY() - enemy->getY();

  if(abs(dx) < 10 && abs(dy) < 10)
  {
    player->attack(enemy);
    enemy->attack(player);
  }
}


void GameManager::update()
{
 if (state != PLAYING)
 {
  return;
 }
 player -> move();
 enemy->updateAI(player);
 handleCombat();
 if(!player->isAlive() || !enemy->isAlive())
 {
  state = GAME_OVER;
 }

}

GameManager::~GameManager(){

  cout << "GameManager destructor is called";

}

}// GameManager.cpp - Game manager implementation// GameManager.cpp - Game manager implementation
