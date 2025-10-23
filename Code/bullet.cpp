#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

class Bullet {
    public:
        double x = 550.0f;
        double y = 415.0f;
        float scaleX = 4;
        float scaleY = 4;
        double dir = 0.0f;
        bool remove = false;
        int speed = 1500;
        int startOffset = 50;
        bool loaded = false;
        std::string bulletTexturePath = "./Assets/Images/bullet.png";
        static sf::Texture bulletTexture;
        sf::Sprite bulletSprite;

    void load() {
        double dirRadians = dir * (M_PI / 180.0);
        double deltaX = startOffset * cos(dirRadians);
        double deltaY = startOffset * sin(dirRadians);
        x += deltaX;
        y += deltaY;
        loaded = true;
        if (!bulletTexture.loadFromFile(bulletTexturePath)) {
            std::cerr << "Error loading Ship Texture.\n";
        }
        bulletSprite.setTexture(bulletTexture);
        bulletSprite.setPosition(x, y);
        bulletSprite.setRotation(dir + 90);
        bulletSprite.setScale(scaleX, scaleY);
        bulletSprite.setOrigin(bulletSprite.getTexture()->getSize().x / 2.0, bulletSprite.getTexture()->getSize().y / 2.0);
    }

    void draw(sf::RenderWindow &window) {
        if (!remove) {
            bulletSprite.setPosition(x, y);
            bulletSprite.setRotation(dir + 90);
            window.draw(bulletSprite);
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
sf::Texture Bullet::bulletTexture;
