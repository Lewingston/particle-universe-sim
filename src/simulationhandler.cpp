

#include "simulationhandler.h"
#include "particle.h"
#include "misc.h"

void SimulationHandler::setParticleRange(unsigned int min, unsigned int max) {
  particlesPerColorMin = min;
  particlesPerColorMax = max;
}

void SimulationHandler::generateRandomUniverse(unsigned int colorCount) {

  std::vector<std::shared_ptr<ParticleCollection>> particles;
  this->colorCount = colorCount;
  totalParticleCount = 0;

  ParticleHandler::instance.deleteAllParticles();
  ruleHandler.deleteAllRules();

  // generate particles
  for (unsigned int ii = 0; ii < colorCount; ii++) {
    sf::Color color(static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255));
    unsigned int particleCount = Misc::getRandomUnsignedInt(particlesPerColorMin, particlesPerColorMax);
    auto p = ParticleCollection::create(particleCount, color);
    totalParticleCount += particleCount;
    particles.push_back(p);
  }

  // generate rules
  for (auto p1 : particles) {
    std::shared_ptr<RuleCollection> collection = std::make_shared<RuleCollection>();
    for (auto p2 : particles) {
      std::shared_ptr<Rule> rule = Rule::create(p1, p2);
      setRandomRuleParameters(rule);
      collection->addRule(rule);
    }
    ruleHandler.addRuleCollection(collection);
  }
}

void SimulationHandler::setRandomRuleParameters(std::shared_ptr<Rule> rule) {
  rule->setStrength(Misc::getRandomFloat(-1.0, 1.0));
  rule->setMaxDistance(Misc::getRandomFloat(40, 180));
  rule->setForceDamping(Misc::getRandomFloat(0.2f, 0.8f));
}

void SimulationHandler::start() {

  if (runSimulationTask)
    return;

  runSimulationTask = true;
  simulationTask = std::thread([this]{simulationTaskFunction();});
}

void SimulationHandler::stop() {

  if (!runSimulationTask)
    return;

  runSimulationTask = false;
  simulationTask.join();
}

void SimulationHandler::simulationTaskFunction() {
  cps = 0.0f;
  std::chrono::time_point<std::chrono::steady_clock> cpsTimer;

  while (runSimulationTask) {

    int milliseconds = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - cpsTimer).count());
    cps = (cps * 0.9f) + (1000.0f / static_cast<float>(milliseconds) * 0.1f);
    cpsTimer = std::chrono::steady_clock::now();

    ruleHandler.applyRules();
    ParticleHandler::instance.updatePositions();
  }
}

void SimulationHandler::startParalel() {

  // do not start any new tasks if the simulation is allready running
  if (runSimulationTask)
    return;

  runSimulationTask = true;

  for (std::shared_ptr<RuleCollection> collection : ruleHandler.getCollections()) {
    paralelTasks.push_back(std::thread(
      [this] (std::shared_ptr<RuleCollection> collection) {
        paralelSimulationTaskFunction(collection);
      }, collection
    ));
  }
}

void SimulationHandler::stopParalel() {

  // no need to stop anything if the simulation is not running
  if (!runSimulationTask)
    return;

  runSimulationTask = false;

  for (std::thread& task : paralelTasks) {
    task.join();
  }

  paralelTasks.clear();
}

void SimulationHandler::paralelSimulationTaskFunction(std::shared_ptr<RuleCollection> collection) {
  while (runSimulationTask) {
    collection->applyRules();
    //syncParalel();
  }
}

void SimulationHandler::shuffleRules() {

  stop();

  for (std::shared_ptr<RuleCollection> collection : ruleHandler.getCollections()) {
    for (std::shared_ptr<Rule> rule : collection->getRules()) {
      setRandomRuleParameters(rule);
    }
  }

  start();
}

void SimulationHandler::shuffleParticles() {

  stop();

  for (std::shared_ptr<ParticleCollection> collection : ParticleHandler::instance.getCollections())
    collection->reposition();

  start();
}
