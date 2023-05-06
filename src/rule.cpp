
#include "rule.h"
#include "particle.h"
#include <cmath>
#include <iostream>

/******************************************************************************/
/*                                class Rule                                  */
/******************************************************************************/

Rule::Rule(std::shared_ptr<ParticleCollection> p1, std::shared_ptr<ParticleCollection> p2) :
  pc1(p1), pc2(p2)
{
  strength = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

std::shared_ptr<Rule> Rule::create(std::shared_ptr<ParticleCollection> p1, std::shared_ptr<ParticleCollection> p2) {
  std::shared_ptr<Rule> rule = std::make_shared<Rule>(p1, p2);
  return rule;
}

void Rule::apply() {
  pc1->applyRule(pc2, *this);
}

void Rule::applyToParticle(Particle& p1, const Particle& p2) const {
  float vx = p1.getVx();
  float vy = p1.getVy();

  float distanceSqr = p1.getDistanceSqr(p2);
  if (distanceSqr < maxDistance * maxDistance) {
    float distance = std::sqrt(distanceSqr);

    float dx = p2.getPosX() - p1.getPosX();
    float dy = p2.getPosY() - p1.getPosY();

    if (distance != 0) {
      float fx = (strength / distance) * dx;
      float fy = (strength / distance) * dy;
      vx = (vx + fx) * forceDamping;
      vy = (vy + fy) * forceDamping;
      p1.setV(vx, vy);
    }
    p1.applyValocity();
  }
}

/******************************************************************************/
/*                          class RuleCollection                              */
/******************************************************************************/

void RuleCollection::addRule(std::shared_ptr<Rule> rule) {
  rules.push_back(rule);
}

void RuleCollection::applyRules() {
  for (std::shared_ptr<Rule> rule : rules) {
    rule->apply();
  }
}

/******************************************************************************/
/*                           class RuleHandler                                */
/******************************************************************************/

void RuleHandler::addRuleCollection(std::shared_ptr<RuleCollection> collection) {
  collections.push_back(collection);
}

void RuleHandler::applyRules() {
  for (std::shared_ptr<RuleCollection> collection : collections) {
    collection->applyRules();
  }
}
