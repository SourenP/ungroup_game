#ifndef Group_hpp
#define Group_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Player.hpp"
#include "../common/Circle.hpp"

class Group : public GameObject {

    public:
        Group(int id, sf::Vector2f position);
        ~Group();
    
        void update();
        void addMember(Player* player);
        static void handleCollisions(std::vector<Group*>& groups);
    
        // Getters
        int getId() const;
        Circle* getCircle();

    private:
        int mId;
        float mSize;
        std::vector<Player*> mMembers;
        bool shouldDeactivate();
        Circle* mCircle;
};

#endif /* Group_hpp */