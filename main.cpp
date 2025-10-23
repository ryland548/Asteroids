#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "./Code/ship.cpp"
#include "./Code/bullet.cpp"
#include "./Code/asteroid.cpp"
#include "./Code/health.cpp"
#include "./Code/score.cpp"
#include "./Code/startMsg.cpp"
#include "./Code/deathMsg.cpp"

void restart(Ship &ship, std::vector<Asteroid> &asteroids, std::vector<Bullet> &bullets, Health &health, Score &score, int &scoreInt) {
    ship.x = 550.0f;
    ship.y = 415.0f;
    ship.dir = 0;
    ship.start = false;
    ship.dead = false;
    ship.health = 4;
    ship.velocity.velocityX = 0;
    ship.velocity.velocityY = 0;

    asteroids.clear();
    bullets.clear();
    health.health = 4;
    score.score = 0;
    scoreInt = 0;
}

void spawnAsteroid(std::vector<Asteroid> &asteroids, Ship &ship, std::mt19937 &gen) {
    Asteroid asteroid = Asteroid();
    std::uniform_int_distribution<> distrib(1, 4);
    int side = distrib(gen);
    if (side == 1) { // Right
        asteroid.x = 1300;
        std::uniform_int_distribution<> distribY(0, 830);
        asteroid.y = distribY(gen);
    } else if (side == 2) { // Left
        asteroid.x = -200;
        std::uniform_int_distribution<> distribY(0, 830);
        asteroid.y = distribY(gen);
    } else if (side == 3) { // Top
        asteroid.y = -200;
        std::uniform_int_distribution<> distribX(0, 1100);
        asteroid.x = distribX(gen);
    } else if (side == 4) { // Bottom
        asteroid.y = 1030;
        std::uniform_int_distribution<> distribX(0, 1100);
        asteroid.x = distribX(gen);
    }
    float dx = ship.x - asteroid.x;
    float dy = ship.y - asteroid.y;
    float angle = std::atan2(dy, dx);
    float angleDir = angle * 180.f / M_PI;
    asteroid.dir = angleDir;
    asteroids.push_back(asteroid);
}

int main() {
    int score = 0;
    const float asteroidSpawnCooldown = 1.f;

    const int screenWidth = 1100;
    const int screenHeight = 830;
    Ship ship = Ship();
    ship.load();
    Health healthScore = Health();
    healthScore.load();
    Score scoreText = Score();
    scoreText.load();
    StartMessage startMsg = StartMessage();
    startMsg.load();
    DeathMessage deathMsg = DeathMessage();
    deathMsg.load();

    sf::SoundBuffer explodeBuffer;
    sf::Sound explodeSound;
    std::string explodeSoundPath = "./Assets/Audio/bang.wav";
    if (!explodeBuffer.loadFromFile(explodeSoundPath)) {
        std::cerr << "Error loading Sound";
    }
    explodeSound.setBuffer(explodeBuffer);

    std::vector<Bullet> bullets;
    std::vector<Asteroid> asteroids;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Asteroids");
    window.setVerticalSyncEnabled(true);
    sf::Clock deltaClock;
    sf::Clock spawnClock;
    std::random_device rd;
    std::mt19937 gen(rd());
    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        float dtSeconds = deltaTime.asSeconds();
        sf::Event event;
        window.clear();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (ship.shoot(event) && ship.start) {
                Bullet bullet = Bullet();
                bullet.x = ship.x;
                bullet.y = ship.y;
                bullet.dir = ship.dir;
                bullets.push_back(bullet);
            }
        }
        sf::Time elapsed = spawnClock.getElapsedTime();
        float elapsedSeconds = elapsed.asSeconds();
        if (elapsedSeconds >= asteroidSpawnCooldown && !ship.dead && ship.start) {
            spawnAsteroid(asteroids, ship, gen);
            spawnClock.restart();
        }

        for (Bullet &bullet: bullets) {
            if (!bullet.loaded) {
                bullet.load();
            }
            bullet.draw(window);
            bullet.move(dtSeconds);
        }
        std::vector<Asteroid> newAsteroids;
        for (Asteroid &asteroid: asteroids) {
            if (!asteroid.loaded) {
                asteroid.load();
            }
            sf::FloatRect shipBounds = ship.shipSprite.getGlobalBounds();
            shipBounds.width  /= 7.f;
            shipBounds.height /= 7.f;
            if (shipBounds.intersects(asteroid.asteroidSprite.getGlobalBounds())) {
                asteroid.remove = true;
                ship.lowerHealth();
            }
            for (Bullet &bullet: bullets) {
                sf::FloatRect asteroidBounds = asteroid.asteroidSprite.getGlobalBounds();
                asteroidBounds.width  /= 1.5;
                asteroidBounds.height /= 1.5;
                if (bullet.bulletSprite.getGlobalBounds().intersects(asteroidBounds)) {
                    bullet.remove = true;
                    if (asteroid.scaleX <= 2) {
                        asteroid.remove = true;
                    } else {
                        std::uniform_int_distribution<> distribDir(0, 360);
                        for (short i = 0; i < 2; i++) {
                            explodeSound.play();
                            Asteroid asteroidClone = Asteroid();
                            asteroidClone.scaleX = asteroid.scaleX / 2;
                            asteroidClone.scaleY = asteroid.scaleY / 2;
                            score++;
                            asteroidClone.x = asteroid.x;
                            asteroidClone.y = asteroid.y;
                            asteroidClone.dir = distribDir(gen);
                            newAsteroids.push_back(asteroidClone);
                            asteroid.remove = true;
                        }
                    }
                }
            }
            asteroid.draw(window);
            asteroid.move(dtSeconds);
        }
        for (Asteroid &newAsteroid: newAsteroids) {
            asteroids.push_back(newAsteroid);
        }
        bullets.erase(
            std::remove_if(
                bullets.begin(), bullets.end(), [](const Bullet& bullet) { return bullet.remove; }
            ),
            bullets.end()
        );
        asteroids.erase(
            std::remove_if(
                asteroids.begin(), asteroids.end(), [](const Asteroid& asteroid) { return asteroid.remove; }
            ),
            asteroids.end()
        );
        healthScore.draw(window, ship.health);
        scoreText.draw(window, score);
        ship.draw(window);
        if (!ship.start) {
            startMsg.draw(window);
        }
        if (ship.dead) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                restart(ship, asteroids, bullets, healthScore, scoreText, score);
            }
            deathMsg.draw(window);
        }
        if (ship.start) {
            ship.move(dtSeconds);
            ship.rotate(dtSeconds);
        }
        ship.screenWrap(screenWidth, screenHeight);

        window.display();
    }
    return 0;
}
