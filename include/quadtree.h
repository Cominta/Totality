#ifndef QUADTREE_H
#define QUADTREE_H

#include "hitboxSquare.h"
#include <vector>
#include "BaseUnit.h"

class Quadtree
{
    private:
        int size;
        float x;
        float y;
        float width;
        float height;
        bool divided;

        HitboxSquare* boundary;
        sf::RenderWindow* window;
        std::vector<BaseUnit*> units;

        Quadtree* lt = nullptr; // left-top
        Quadtree* rt = nullptr; // right-top
        Quadtree* rb = nullptr; // right-bottom
        Quadtree* lb = nullptr; // left-bottom

    public:
        Quadtree(sf::RenderWindow* window, int size, float x, float y, float width, float height);
        ~Quadtree();

        void clear();
        void insert(BaseUnit* unit);
        void subdivide();
        void query(HitboxSquare* range, std::vector<BaseUnit*>& found);

        void update();
        void render();
};

#endif