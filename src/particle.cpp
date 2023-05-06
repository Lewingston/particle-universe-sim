#include "particle.h"
#include <iostream>

/******************************************************************************/
/*                             class Particle                                 */
/******************************************************************************/

int Particle::xPosMin = 0;
int Particle::yPosMin = 0;
int Particle::xPosMax = 1600;
int Particle::yPosMax = 900;
int Particle::pSize = 3;

Particle::Particle(int posX, int posY) :
  posX(static_cast<float>(posX)),
  posY(static_cast<float>(posY)),
  drawPosX(static_cast<float>(posX)),
  drawPosY(static_cast<float>(posY))
{
}

Particle::Particle() :
  Particle((std::rand() % (xPosMax - pSize - xPosMin)) + xPosMin, (std::rand() % (yPosMax - pSize - yPosMin)) + yPosMin)
{
}

void Particle::setPos(float posX, float posY) {
  this->posX = posX;
  this->posY = posY;
}

void Particle::setV(float vx, float vy) {
  this->vX = vx;
  this->vY = vy;
}

void Particle::applyValocity() {

  posX = posX + vX;
  posY = posY + vY;

  if (posX < static_cast<float>(xPosMin)) { posX = static_cast<float>(xPosMin); vX*=-1; }
  if (posX > static_cast<float>(xPosMax - pSize)) { posX = static_cast<float>(xPosMax-pSize); vX*=-1; }

  if (posY < static_cast<float>(yPosMin)) { posY = static_cast<float>(yPosMin); vY*=-1; }
  if (posY > static_cast<float>(yPosMax - pSize)) { posY = static_cast<float>(yPosMax - pSize); vY*=-1; }

}

void Particle::updateRenderPosition() {
  drawPosX = posX;
  drawPosY = posY;
}

/******************************************************************************/
/*                        class ParticleCollection                            */
/******************************************************************************/

ParticleCollection::ParticleCollection(unsigned int count, sf::Color color) {
    for (unsigned int ii = 0; ii < count; ii++) {
      particles.push_back(Particle());
    }
    this->color = color;
}

std::shared_ptr<ParticleCollection> ParticleCollection::create(unsigned int count, sf::Color color) {
  std::shared_ptr<ParticleCollection> collection = std::make_shared<ParticleCollection>(count, color);
  ParticleHandler::instance.addParticleCollection(collection);
  return collection;
}

void ParticleCollection::reposition() {
  for (unsigned int ii = 0; ii < particles.size(); ii++) {
    particles[ii] = Particle();
  }
}

void ParticleCollection::drawParticles(sf::RenderWindow& window) const {

  sf::RectangleShape rectangle(Particle::getParticleSize());
  rectangle.setFillColor(color);

  for (const Particle& p : particles) {
    rectangle.setPosition(p.getRenderPos());
    window.draw(rectangle);
  }
}

void ParticleCollection::updatePositions() {
  for (Particle& p : particles)
    p.updateRenderPosition();
}

void ParticleCollection::applyRule(std::shared_ptr<ParticleCollection> otherCollection, const Rule& rule) {
  for (Particle& p : particles) {
    for (const Particle& p2 : otherCollection->particles) {
      rule.applyToParticle(p, p2);
    }
  }
}

/******************************************************************************/
/*                        class ParticleHandler                               */
/******************************************************************************/

ParticleHandler ParticleHandler::instance;

void ParticleHandler::addParticleCollection(std::shared_ptr<ParticleCollection> collection) {
  collections.push_back(collection);
}

void ParticleHandler::updatePositions() {
  std::lock_guard<std::mutex> guard(updatePositionMutex);
  for (std::shared_ptr<ParticleCollection> collection : collections)
    collection->updatePositions();
}

void ParticleHandler::drawParticles(sf::RenderWindow& window) {
  std::lock_guard<std::mutex> guard(updatePositionMutex);
  for (std::shared_ptr<ParticleCollection> collection : collections) {
    collection->drawParticles(window);
  }
}
