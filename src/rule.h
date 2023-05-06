#pragma once

#include <memory>
#include <vector>

class ParticleCollection;
class Particle;

class Rule {

  public:

    Rule(std::shared_ptr<ParticleCollection> p1, std::shared_ptr<ParticleCollection> p2);
    virtual ~Rule() = default;

    static std::shared_ptr<Rule> create(std::shared_ptr<ParticleCollection> p1, std::shared_ptr<ParticleCollection> p2);

    void apply();
    void applyToParticle(Particle& p1, const Particle& p2) const;

    void setStrength(float strength) { this->strength = strength; }
    void setMaxDistance(float distance) { this->maxDistance = distance; }
    void setForceDamping(float damping) { this->forceDamping = damping; }

  private:

    float strength = 0.0f;
    float maxDistance = 120;
    float forceDamping = 0.5;
    std::shared_ptr<ParticleCollection> pc1;
    std::shared_ptr<ParticleCollection> pc2;

};

class RuleCollection {

  public:

    void addRule(std::shared_ptr<Rule> rule);
    void applyRules();
    void deleteAllRules() { rules.clear(); }
    [[nodiscard]] std::vector<std::shared_ptr<Rule>> getRules() const { return rules; }

  private:

    std::vector<std::shared_ptr<Rule>> rules;

};

class RuleHandler {

  public:

    RuleHandler() = default;
    virtual ~RuleHandler() = default;

    void addRuleCollection(std::shared_ptr<RuleCollection> collection);
    void applyRules();
    void deleteAllRules() { collections.clear(); }
    [[nodiscard]] std::vector<std::shared_ptr<RuleCollection>> getCollections() const { return collections; }

  private:

    std::vector<std::shared_ptr<RuleCollection>> collections;

};
