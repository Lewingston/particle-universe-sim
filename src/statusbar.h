#pragma once

#include "SFML/Graphics.hpp"

class StatusBar {

  public:

    struct StatusParameter {
      float fps;
      float cps;
      unsigned int colorCount;
      unsigned int particleCount;
    };

    StatusBar(int posX, int posY, int width, int height);

    virtual ~StatusBar() = default;

    void draw(sf::RenderWindow& window);
    void setStatus(const StatusParameter params) { this->params = params; }

  private:

    sf::Font font;
    bool fontLoaded = false;

    StatusParameter params;

    int posX;
    int posY;
    int width;
    int height;

};
