#include "GameController.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "../events/EventController.hpp"
#include "../factories/IdFactory.hpp"
#include "../util/game_def.hpp"
#include "../util/game_settings.hpp"
#include "GameObjectStore.hpp"

GameController::GameController(LevelKey level_key) :
    m_physicsController(new PhysicsController()),
    m_gameObjectStore(new GameObjectStore(*m_physicsController)),
    m_gameObjectController(new GameObjectController(*m_gameObjectStore, level_key)),
    m_gameStateCore({.tick = 0, .status = GameStatus::not_started, .winner_player_id = 0}) {
}

GameController::~GameController() {
}

void GameController::step() {
    preUpdate();

    InputDef::PlayerInputs pi = getPlayerInputs();

    // Take a variable amount of game state steps depending on how long the last frame took
    if (!GAME_SETTINGS.USE_REMAINDER_DELTA_TIME) {
        m_timeAccumulator = 0;
    }
    m_timeAccumulator += m_clock.restart().asMilliseconds();
    sf::Int32 update_time = 0;
    while (m_timeAccumulator >= MIN_TIME_STEP) {
        update(pi, MIN_TIME_STEP);

        // Update metrics
        m_gameStepMetric.update();
        m_gameUpdateMetric.update();

        // Clear updates since they've already been consumed
        pi.player_reliable_inputs.clear();
        pi.player_unreliable_inputs.clear();

        m_gameUpdateMetric.pushCount();

        m_timeAccumulator -= MIN_TIME_STEP;
        m_elapsedTime += MIN_TIME_STEP;
        update_time += MIN_TIME_STEP;
    }
    m_gameStepMetric.pushCount();
    postUpdate(update_time);
}

void GameController::computeGameState(const InputDef::PlayerInputs& pi, sf::Int32 delta_ms) {
    m_gameObjectController->update(pi);
    m_physicsController->update(delta_ms);
    EventController::getInstance().forceProcessEvents();
    incrementTick();
}
