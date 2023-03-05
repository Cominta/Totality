#include "quadtree.h"

Quadtree::Quadtree(sf::RenderWindow* window, int size, float x, float y, float width, float height)
    : window(window), size(size), x(x), y(y), width(width), height(height)
{
    this->boundary = new HitboxSquare(window, this->width, this->height);
    this->boundary->setPosition(this->x, this->y);

    this->divided = false;
}

Quadtree::~Quadtree()
{
    this->clear();
}

void Quadtree::clear()
{
    this->units.clear();
    this->divided = false;

    if (this->lt != nullptr)
    {
        delete this->lt;
    }

    if (this->rt != nullptr)
    {
        delete this->rt;
    }

    if (this->rb != nullptr)
    {
        delete this->rb;
    }

    if (this->lb != nullptr)
    {
        delete this->lb;
    }

    this->lt = nullptr;
    this->rt = nullptr;
    this->rb = nullptr;
    this->lb = nullptr;
}

void Quadtree::insert(BaseUnit* unit)
{
    if (!this->boundary->intersects(unit->getHitbox())) // AABB
    {
        return;
    }

    if (this->units.size() < this->size)
    {
        this->units.emplace_back(unit);
    }

    else
    {
        if (!divided)
        {
            this->subdivide();
            this->divided = true;
        }

        this->lt->insert(unit);
        this->rt->insert(unit);
        this->rb->insert(unit);
        this->lb->insert(unit);
    }
}

void Quadtree::subdivide()
{
    int newWidth = this->width / 2;
    int newHeight = this->height / 2;

    this->lt = new Quadtree(this->window, 4, this->x - this->width / 4, this->y - this->height / 4, newWidth, newHeight);
    this->rt = new Quadtree(this->window, 4, this->x + this->width / 4, this->y - this->height / 4, newWidth, newHeight);
    this->rb = new Quadtree(this->window, 4, this->x + this->width / 4, this->y + this->height / 4, newWidth, newHeight);
    this->lb = new Quadtree(this->window, 4, this->x - this->width / 4, this->y + this->height / 4, newWidth, newHeight);
}

void Quadtree::query(HitboxSquare* range, std::vector<BaseUnit*>& found)
{
    if (!this->boundary->intersects(range))
    {
        return;
    }

    else
    {
        for (auto &point : this->units)
        {
            if (range->intersects(point->getHitbox()))
            {
                found.emplace_back(point);
            }
        }

        if (divided)
        {
            this->lt->query(range, found);
            this->rt->query(range, found);
            this->rb->query(range, found);
            this->lb->query(range, found);
        }
    }
}

void Quadtree::update()
{

}

void Quadtree::render()
{
    this->boundary->render();

    if (divided)
    {
        this->lt->render();
        this->rt->render();
        this->rb->render();
        this->lb->render();
    }
}