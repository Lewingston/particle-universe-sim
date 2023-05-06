#pragma once

class Misc {

  public:

    [[nodiscard]] static int getRandomInt(int min, int max) {
      return min + (std::rand() % (max - min + 1));
    }

    [[nodiscard]] static unsigned int getRandomUnsignedInt(unsigned int min, unsigned int max) {
        return min + (static_cast<unsigned int>(std::rand()) % (max - min + 1));
    }

    [[nodiscard]] static float getRandomFloat(float min, float max) {
      return min + static_cast<float>(std::rand()) / (RAND_MAX/(max-min));
    }

  private:


};
