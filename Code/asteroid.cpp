#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Asteroid {
    public:
        double x = 550.0f;
        double y = 415.0f;
        float scaleX = 8;
        float scaleY = 8;
        double dir = 0.0f;
        float spawnLifeTime = 25.0f;
        bool remove = false;
        int speed = 100;
        bool loaded = false;
        sf::Clock lifetimeClock;
        std::string asteroidTexturePath = "./Assets/Images/asteroid.png";
        static sf::Texture asteroidTexture;
        sf::Sprite asteroidSprite;

    void load() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(50, 150);
        speed = distrib(gen);
        loaded = true;
        if (!asteroidTexture.loadFromFile(asteroidTexturePath)) {
            std::cerr << "Error loading Asteroid Texture.\n";
        }
        asteroidSprite.setTexture(asteroidTexture);
        asteroidSprite.setPosition(x, y);
        asteroidSprite.setRotation(dir + 90);
        asteroidSprite.setScale(scaleX, scaleY);
        asteroidSprite.setOrigin(asteroidSprite.getTexture()->getSize().x / 2.0, asteroidSprite.getTexture()->getSize().y / 2.0);
        lifetimeClock.restart();
    }

    void draw(sf::RenderWindow &window) {
        if (!remove) {
            asteroidSprite.setPosition(x, y);
            asteroidSprite.setRotation(dir + 90);
            window.draw(asteroidSprite);
        }

        if (lifetimeClock.getElapsedTime().asSeconds() >= spawnLifeTime) {
            remove = true;
        }
    }

    void move(float dtSeconds) {
        double dirRadians = dir * (M_PI / 180.0);
        double deltaX = speed * cos(dirRadians);
        double deltaY = speed * sin(dirRadians);
        x += deltaX * dtSeconds;
        y += deltaY * dtSeconds;
    }
};
sf::Texture Asteroid::asteroidTexture;
