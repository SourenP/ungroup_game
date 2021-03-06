#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "PhysicsController.hpp"

#include "../events/CollisionEvent.hpp"
#include "../events/EventController.hpp"
#include "../util/game_settings.hpp"
#include "PhysicsDef.hpp"
#include "VectorUtil.hpp"

PhysicsController::PhysicsController() {
}

CircleRigidBody& PhysicsController::add(std::unique_ptr<CircleRigidBody> circle_rigid_body) {
    CircleRigidBody& circle_rigid_body_ref = *circle_rigid_body;
    m_circleRigidBodies.push_back(std::move(circle_rigid_body));
    return circle_rigid_body_ref;
}

void PhysicsController::update(sf::Int32 delta_ms) {
    step(delta_ms);
    resolveCollisions();
    resolveMapBounds();
}

/**
 * Update positions of all physics bodies.
 */
void PhysicsController::step(sf::Int32 delta_ms) {
    for (auto& crb : m_circleRigidBodies) {
        crb->step(delta_ms);
    }
}

/**
 * Detects and resolves collision between all active CircleRigidBodies by pairs.
 */
void PhysicsController::resolveCollisions() {
    auto inactive_crbs = std::stable_partition(
        m_circleRigidBodies.begin(), m_circleRigidBodies.end(),
        [](const std::unique_ptr<CircleRigidBody>& crb) { return crb->isActive(); });

    for (auto circle_a_it = m_circleRigidBodies.begin(); circle_a_it != inactive_crbs;
         ++circle_a_it) {
        for (auto circle_b_it = m_circleRigidBodies.begin(); circle_b_it != inactive_crbs;
             ++circle_b_it) {
            CircleRigidBody& circle_a = **circle_a_it;
            CircleRigidBody& circle_b = **circle_b_it;

            if (CollisionUtil::areIntersecting(circle_a, circle_b)) {
                Collision collision = CollisionUtil::getCollision(circle_a, circle_b);
                if (collision.collided == false) {
                    continue;
                }
                resolveCollision(circle_a, circle_b, collision);
                fireCollisionEvent(collision);
            }
        }
    }
}

void PhysicsController::resolveCollision(CircleRigidBody& circle_a, CircleRigidBody& circle_b,
                                         const Collision& collision) {
    // Move bodies out of one another
    circle_a.move(collision.resolution.first);
    circle_b.move(collision.resolution.second);

    // Apply impulses resulting from collision
    auto impulses = CollisionUtil::getImpulses(circle_a, circle_b, collision);

    impulses.first.magnitude *= PhysicsDef::IMPULSE_MILTIPLIER;
    impulses.second.magnitude *= PhysicsDef::IMPULSE_MILTIPLIER;

    circle_a.applyImpulse(impulses.first);
    circle_b.applyImpulse(impulses.second);
}

void PhysicsController::resolveMapBounds() {
    auto inactive_crbs = std::stable_partition(
        m_circleRigidBodies.begin(), m_circleRigidBodies.end(),
        [](const std::unique_ptr<CircleRigidBody>& crb) { return crb->isActive(); });

    for (auto circle_it = m_circleRigidBodies.begin(); circle_it != inactive_crbs; ++circle_it) {
        CircleRigidBody& circle = **circle_it;
        if (circle.isMovable() && !CollisionUtil::isInBounds(circle, GAME_SETTINGS.GAME_CENTER,
                                                             GAME_SETTINGS.GAME_BOUNDS_RADIUS)) {
            circle.move(CollisionUtil::getBoundsCorrection(circle, GAME_SETTINGS.GAME_CENTER,
                                                           GAME_SETTINGS.GAME_BOUNDS_RADIUS));
            circle.setVelocity({0, 0});
        }
    }
}

void PhysicsController::fireCollisionEvent(const Collision& collision) {
    EventController::getInstance().forceQueueEvent(
        std::move(std::unique_ptr<CollisionEvent>(new CollisionEvent(collision))));
}
