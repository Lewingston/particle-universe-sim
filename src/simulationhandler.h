#pragma once

#include "rule.h"

#include <thread>
#include <vector>

class SimulationHandler {

  public:

    void setParticleRange(unsigned int min, unsigned int max);
    void generateRandomUniverse(unsigned int count);

    void start();
    void stop();

    void startParalel();
    void stopParalel();

    void shuffleRules();
    void shuffleParticles();

    [[nodiscard]] float getCps() const { return cps; }
    [[nodiscard]] unsigned int getColorCount() const { return colorCount; }
    [[nodiscard]] unsigned int getTotalParticleCount() const { return totalParticleCount; }

  private:

    void setRandomRuleParameters(std::shared_ptr<Rule> rule);

    void simulationTaskFunction();
    void paralelSimulationTaskFunction(std::shared_ptr<RuleCollection> collection);

    void syncParalel();
    std::vector<std::thread> paralelTasks;

    unsigned int colorCount = 0;
    unsigned int totalParticleCount = 0;

    unsigned int particlesPerColorMin = 1;
    unsigned int particlesPerColorMax = 100;

    bool runSimulationTask = false;
    std::thread simulationTask;

    float cps = 0.0f;

    RuleHandler ruleHandler;

};
