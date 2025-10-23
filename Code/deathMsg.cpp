#include <iostream>
#include <SFML/Graphics.hpp>

class DeathMessage {
    public:
        int x = 380;
        int y = 600;
        float fontSize = 50;
        std::string fontPath = "./Assets/Fonts/AsteroidsFont.ttf";
        sf::Font font;
        sf::Text text;

    void load() {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Error loading Score Font.\n";
        }
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        text.setString("Press R to Restart");
    }

    void draw(sf::RenderWindow &window) {
        window.draw(text);
    }
};
