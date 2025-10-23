#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Score {
    public:
        int score = 0;
        int x = 100;
        int y = 0;
        float fontSize = 140;
        std::string fontPath = "./Assets/Fonts/AsteroidsFont.ttf";
        sf::Font font;
        sf::Text text;

    void load() {
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Error loading Score Font.\n";
        }
        text.setFont(font);
        text.setString(std::to_string(score));
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
    }

    void draw(sf::RenderWindow &window, int changedscore) {
        if (changedscore != score) {
            score = changedscore;
        }
        text.setString(std::to_string(score));
        window.draw(text);
    }
};
