
#pragma once

#include "SFML/Graphics.hpp"

#include "rule.h"

#include <memory>
#include <mutex>

class Particle {

  public:

    Particle(int posX, int posY);
    Particle();
    virtual ~Particle() = default;

    [[nodiscard]] sf::Vector2f getPosition() const { return sf::Vector2f(posX, posY); }
    [[nodiscard]] sf::Vector2f getRenderPos() const { return sf::Vector2f(drawPosX, drawPosY); }
    [[nodiscard]] float getPosX() const { return posX; }
    [[nodiscard]] float getPosY() const { return posY; }
    [[nodiscard]] float getVx() const { return vX; }
    [[nodiscard]] float getVy() const { return vY; }
    void setV(float vx, float vy);
    void setPos(float posX, float posY);
    void updateRenderPosition();
    void applyValocity();

    [[nodiscard]] float getDistanceSqr(const Particle& p) const {
      return (p.posX-posX)*(p.posX-posX) + (p.posY-posY)*(p.posY-posY);
    }

    [[nodiscard]] static sf::Vector2f getParticleSize() { return sf::Vector2f(static_cast<float>(pSize), static_cast<float>(pSize)); }
    static void setMaxDimension(int x, int y) { xPosMax = x; yPosMax = y; }
    static void setMinDimension(int x, int y) { xPosMin = x; yPosMin = y;}
    static void setParticleSize(int size) { pSize = size; }

  private:

    float posX;
    float posY;
    float drawPosX;
    float drawPosY;
    float vX = 0;
    float vY = 0;

    static int xPosMin;
    static int xPosMax;
    static int yPosMin;
    static int yPosMax;

    static int pSize;

};

class ParticleCollection {

  public:

    ParticleCollection(unsigned int count, sf::Color color);
    virtual ~ParticleCollection() = default;

    static std::shared_ptr<ParticleCollection> create(unsigned int count, sf::Color color);

    void reposition();
    void drawParticles(sf::RenderWindow& window) const;
    void updatePositions();

    void applyRule(std::shared_ptr<ParticleCollection> otherCollection, const Rule& rule);

  private:

    std::vector<Particle> particles;
    sf::Color color;

};

class ParticleHandler {

  public:

    static ParticleHandler instance;

    void addParticleCollection(std::shared_ptr<ParticleCollection> collection);
    void drawParticles(sf::RenderWindow& window);
    void updatePositions();
    void deleteAllParticles() { collections.clear(); }
    [[nodiscard]] std::vector<std::shared_ptr<ParticleCollection>> getCollections() const { return collections; }

  private:

    std::mutex updatePositionMutex;

    std::vector<std::shared_ptr<ParticleCollection>> collections;

};
