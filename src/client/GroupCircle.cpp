#include <stdio.h>
#include <iostream>
#include "GroupCircle.hpp"

GroupCircle::GroupCircle(int id, sf::Vector2f position) {
    mCircle = new Circle(0.f, position);
}

GroupCircle::~GroupCircle() {
    //dtor
}

Circle* GroupCircle::getCircle() {
    return mCircle;
}