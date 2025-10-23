#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

struct Velocity {
    double velocityX = 0.0;
    double velocityY = 0.0;
};

class Ship {
    public:
        double x = 550.0f;
        double y = 415.0f;
        float scaleX = 5;
        float scaleY = 5;
        double dir = 0.0f;
        int acceleration = 30;
        float turnSpeed = 300;
        bool start = false;
        int health = 4;
        bool dead = false;
        const float friction = 0.9;
        std::string shipTexturePath = "./Assets/Images/ship.png";
        sf::Texture shipTexture;
        sf::Sprite shipSprite;
        sf::SoundBuffer shootBuffer;
        sf::Sound shootSound;
        std::string shootSoundPath = "./Assets/Audio/fire.wav";

        sf::SoundBuffer thrustBuffer;
        sf::Sound thrustSound;
        std::string thrustSoundPath = "./Assets/Audio/thrust.wav";
        Velocity velocity = Velocity();

    void load() {
        if (!shipTexture.loadFromFile(shipTexturePath)) {
            std::cerr << "Error loading Ship Texture.\n";
        }
        shipSprite.setTexture(shipTexture);
        shipSprite.setPosition(x, y);
        shipSprite.setRotation(dir + 90);
        shipSprite.setScale(scaleX, scaleY);
        shipSprite.setOrigin(shipSprite.getTexture()->getSize().x / 2.0, shipSprite.getTexture()->getSize().y / 2.0);

        if (!shootBuffer.loadFromFile(shootSoundPath)) {
            std::cerr << "Error loading Sound";
        }
        shootSound.setBuffer(shootBuffer);

        if (!thrustBuffer.loadFromFile(thrustSoundPath)) {
            std::cerr << "Error loading Sound";
        }
        thrustSound.setBuffer(thrustBuffer);
    }

    void draw(sf::RenderWindow &window) {
        if (!start) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                start = true;
            }
        }
        shipSprite.setPosition(x, y);
        shipSprite.setRotation(dir + 90);
        window.draw(shipSprite);
    }

    void move(float dtSeconds) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !dead) {
            if (thrustSound.getStatus() != sf::Sound::Playing) {
                thrustSound.play();
            }
            double dirRadians = dir * (M_PI / 180.0);
            double deltaX = acceleration * cos(dirRadians);
            double deltaY = acceleration * sin(dirRadians);

            velocity.velocityX += deltaX;
            velocity.velocityY += deltaY;
        } else {
            thrustSound.stop();
        }

        velocity.velocityX *= std::pow(friction, dtSeconds);
        velocity.velocityY *= std::pow(friction, dtSeconds);

        x += velocity.velocityX * dtSeconds;
        y += velocity.velocityY * dtSeconds;
    }

    void rotate(float dtSeconds) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !dead) {
            dir += turnSpeed * dtSeconds;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !dead) {
            dir -= turnSpeed * dtSeconds;
        }
    }

    void screenWrap(int screenWidth, int screenHeight) {
        if (x > screenWidth) {
            x = 0;
        } else if (x < 0) {
            x = screenWidth;
        } else if (y > screenHeight) {
            y = 0;
        } else if (y < 0) {
            y = screenHeight;
        }
    }

    bool shoot(sf::Event event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space && !dead) {
                shootSound.play();
                return true;
            }
        }
        return false;
    }


    void die() {
        if (health <= 0) {
            dead = true;
        }
    }

    void lowerHealth() {
        health -= 1;
        die();
    }
};
