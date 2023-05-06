
#include "SFML/Graphics.hpp"

#include "particle.h"
#include "statusbar.h"
#include "simulationhandler.h"
#include "misc.h"

#include <iostream>

int main() {

  // seed ranomd number generator
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  // create window
  const unsigned int screenWidth = 1600;
  const unsigned int screenHeight = 930;
  sf::RenderWindow window(sf::VideoMode(screenWidth,screenHeight), "PSIM");
  window.setFramerateLimit(60);

  // init status bar
  const unsigned int statusBarHeight = 30;
  StatusBar statusBar(0, screenHeight - statusBarHeight + 1, screenWidth, statusBarHeight);

  // set basic simulation parameters
  Particle::setMinDimension(0, 0);
  Particle::setMaxDimension(screenWidth, screenHeight-statusBarHeight);
  Particle::setParticleSize(3);

  // init particles and rules
  SimulationHandler simulation;
  simulation.setParticleRange(500, 1500);
  simulation.generateRandomUniverse(static_cast<unsigned int>(Misc::getRandomInt(3, 6)));

  float fps = 0.0f;
  std::chrono::time_point<std::chrono::steady_clock> fpsTimer;

  simulation.start();

  while(window.isOpen()) {

    int milliseconds = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - fpsTimer).count());
    fps = (fps * 0.9f) + (1000.0f / static_cast<float>(milliseconds) * 0.1f);
    fpsTimer = std::chrono::steady_clock::now();

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
          simulation.stop();
          simulation.generateRandomUniverse(static_cast<unsigned int>(Misc::getRandomInt(3, 6)));
          simulation.start();
        } else if (event.key.code == sf::Keyboard::T) {
          simulation.shuffleRules();
        } else if (event.key.code == sf::Keyboard::R) {
          simulation.shuffleParticles();
        }
      }
    }

    //RuleHandler::instance.applyRules();
    //ParticleHandler::instance.updatePositions();
    statusBar.setStatus({fps, simulation.getCps(), simulation.getColorCount(), simulation.getTotalParticleCount()});

    window.clear();
    statusBar.draw(window);
    ParticleHandler::instance.drawParticles(window);
    window.display();

  }

  simulation.stop();

  return 0;
}
