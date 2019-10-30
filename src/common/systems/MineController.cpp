#include "MineController.hpp"

#include <exception>

#include "../factories/IdFactory.hpp"

MineController::MineController(std::vector<std::shared_ptr<Mine>>& mines,
                               ResourceStore& resource_store)
    : mMines(mines), mResourceStore(resource_store) {
    for (auto& mine : mMines) {
        mine->setShader(mResourceStore.getShader("default"));
    }
}

MineController::~MineController() {}

uint32_t MineController::createMine() {
    size_t next_mine_index = nextMineIndex++;
    if (next_mine_index >= mMines.size()) {
        throw std::out_of_range("Exceeded max number of mines.");
    }

    Mine& new_mine = *mMines[next_mine_index];
    uint32_t new_mine_id = new_mine.getId();
    new_mine.setActive(true);
    return new_mine_id;
}

void MineController::draw(sf::RenderTarget& target) {
    for (auto& mine : mMines) {
        mine->draw(target);
    }
}

void MineController::update() {
    // noop
}

void MineController::updatePostPhysics() {
    for (auto& mine : mMines) {
        mine->matchRigid();
    }
}

std::shared_ptr<Mine>& MineController::getMine(uint32_t mine_id) {
    return mMines[IdFactory::getInstance().getIndex(mine_id)];
}