
#include "statusbar.h"

#include "fmt/format.h"

#include <iostream>

extern unsigned char res_fonts_ShareTechMono_Regular_ttf[];
extern unsigned int res_fonts_ShareTechMono_Regular_ttf_len;

StatusBar::StatusBar(int posX, int posY, int width, int height) :
  posX(posX), posY(posY), width(width), height(height) {

  if (!font.loadFromMemory(&res_fonts_ShareTechMono_Regular_ttf, res_fonts_ShareTechMono_Regular_ttf_len)) {
    std::cerr << "Failed to load font from program data" << std::endl;
    fontLoaded = false;
  } else {
    fontLoaded = true;
  }

};

void StatusBar::draw(sf::RenderWindow& window) {
  sf::RectangleShape line(sf::Vector2f(static_cast<float>(width), 1.0f));
  line.setPosition(sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY)));
  line.setFillColor(sf::Color::White);
  window.draw(line);

  if (fontLoaded) {

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    float dy = static_cast<float>(posY) + 5;
    float dx = 10;
    text.setString(fmt::format("FPS: {:.2f}", params.fps));
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 150;
    text.setString(fmt::format("CPS: {:.2f}", params.cps));
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 180;
    text.setString("Colors: " + std::to_string(params.colorCount));
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 120;
    text.setString("Total Particles: " + std::to_string(params.particleCount));
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 350;
    text.setString("[R] Reset particles");
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 190;
    text.setString("[T] Generate new rules");
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

    dx += 215;
    text.setString("[ENTER] Generate new rules and particles");
    text.setPosition(sf::Vector2f(dx, dy));
    window.draw(text);

  }
}
