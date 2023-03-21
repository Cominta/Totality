#pragma once

#include "state.h"
#include "button.h"
#include "gameState.h"
#include "textBox.h"

class MainState : public State
{
    private:
        std::map<std::string, Button*> buttons; 
        TextBox* tbSeed;

        sf::RectangleShape background;

        sf::Text title;
        sf::Text credits;
        sf::Font font;

        void filterSeedStr();
        void convert(int num, std::string& str);

    public:
        MainState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~MainState();

        void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
        void render() override; 
        TextBox* getTb() {return this->tbSeed;}
};