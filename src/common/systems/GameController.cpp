#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

#include "GameController.hpp"

#include "../util/game_settings.hpp"
#include "../events/EventController.hpp"


// Game state is stepped at least every <MIN_TIME_STEP> milliseconds. 
sf::Int32 MIN_TIME_STEP = (sf::Int32)((1/60.)*1000.);

GameController::GameController(size_t max_player_count, size_t max_mine_count):
  mPhysicsController(new PhysicsController()),
  mLevelController(new LevelController(mPhysicsController)) {
    mLevelController->loadLevel(max_player_count, max_mine_count);
    mClock.restart();
}

GameController::~GameController() {}

void GameController::update() {
    ClientInputs cis = collectInputs();
    
    // Take a variable amount of game state steps depending on how long the last frame took. See:
    // https://web.archive.org/web/20190403012130/https://gafferongames.com/post/fix_your_timestep/ 
    sf::Int32 frame_time = mClock.restart().asMilliseconds();
    mTimeAccumulator += frame_time;
    while (mTimeAccumulator >= MIN_TIME_STEP) {
        computeGameState(cis, MIN_TIME_STEP);
        mTimeAccumulator -= MIN_TIME_STEP;
        mElapsedTime += MIN_TIME_STEP;
    }

    setNetworkState();
    incrementTick();

    std::cout << "Steps per second: " << static_cast<float>(mStepCount)/(mElapsedTime/1000) << std::endl;
}

void GameController::computeGameState(const ClientInputs& cis, sf::Int32 delta_ms) {
    updateGameObjects(cis);
    mPhysicsController->update(delta_ms);
    updateGameObjectsPostPhysics();
    EventController::getInstance().forceProcessEvents();
    mStepCount++;
}


void GameController::updateGameObjects(const ClientInputs& cis) {
    updatePlayers(cis);
    updateGroups();
}

void GameController::updatePlayers(const ClientInputs& cis) {
    for (const auto& client_id_and_unreliable_update : cis.client_id_and_unreliable_updates) {
        int player_id = mClientToPlayer[client_id_and_unreliable_update.client_id];
        std::shared_ptr<Player> player = mLevelController->getPlayer(player_id);
        player->setDirection(client_id_and_unreliable_update.client_unreliable_update.direction);
    }
    for (const auto& client_id_and_reliable_update : cis.client_id_and_reliable_updates) {
        int player_id = mClientToPlayer[client_id_and_reliable_update.client_id];
        std::shared_ptr<Player> player = mLevelController->getPlayer(player_id);
        player->setGroupable(client_id_and_reliable_update.client_reliable_update.groupable);
    }
}

void GameController::updateGroups() {
    for (auto group : mLevelController->getGroups()) {
        group->update();
    }
}

void GameController::updateGameObjectsPostPhysics() {
    for (auto group : mLevelController->getGroups()) {
        group->matchRigid();
    }

    for (auto mine : mLevelController->getMines()) {
        mine->matchRigid();
    }
}

unsigned int GameController::createPlayerWithGroup() {
    int new_player_id = mLevelController->createPlayer();
    mLevelController->createGroup(new_player_id);
    return new_player_id;
}

void GameController::applyGameState(GameState game_state) {
    for (auto gu : game_state.group_updates) {
        mLevelController->getGroup(gu.group_id)->applyUpdate(gu);
    }
    for (auto mu : game_state.mine_updates) {
        mLevelController->getMine(mu.mine_id)->applyUpdate(mu);
    }
    for (auto pu : game_state.player_updates) {
        mLevelController->getPlayer(pu.player_id)->applyUpdate(pu);
    }
}
