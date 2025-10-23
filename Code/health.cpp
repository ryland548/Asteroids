#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Health {
    public:
        int health = 4;
        int x = 1000;
        int y = 0;
        float fontSize = 140;
        std::string fontPath = "./Assets/Fonts/AsteroidsFont.ttf";
        sf::Font font;
        sf::Text text;
        sf::Color redColor = sf::Color(239, 113, 113);

    void load() {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Error loading Score Font.\n";
        }
        text.setFont(font);
        text.setString(std::to_string(health));
        text.setCharacterSize(fontSize);
        text.setFillColor(redColor);
        text.setPosition(x, y);
    }

    void draw(sf::RenderWindow &window, int changedHealth) {
        if (changedHealth != health && changedHealth >= 0) {
            health = changedHealth;
        }
        text.setString(std::to_string(health));
        window.draw(text);
    }
};
