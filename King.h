#pragma once
#include <string>
#include <vector>
using namespace std;

// Forward declarations
class population;
class Economy;
class Army;
class Resources;

// Leadership base class
class Leader 
{
protected:
    string name;
    int charisma;
    int intelligence;
    int experience;

public:
    // Constructor
    Leader(string n, int c, int i, int e);

    // Virtual destructor
    virtual ~Leader();

    // Getters & Setters
    string getName();
    int  getCharisma();
    int getIntelligence();
    int getExperience();

    void setName(string n);
    void setCharisma(int c);
    void setIntelligence(int i);
    void setExperience(int e);

    // Virtual methods
    virtual void makeDecision() = 0;
    virtual int calculateInfluence() const = 0;
    virtual void displayStatus() const = 0;
};

// King class derived from Leader
class King : public Leader 
{
private:
    int reign;
    double popularity;
    bool isAlive;
    int decisionStyle; // 0-military, 1-economic, 2-diplomatic

public:
    double getPopularity();
    // Constructor
    King(string n, int c, int i, int e);

    // Implementation of virtual methods
    void makeDecision() override;
    int calculateInfluence() const override;
    void displayStatus() const override;

    // King-specific methods
    void setTaxPolicy(double rate);
    bool surviveAssassinationAttempt(double probability);
    void adjustPopularity(int amount);
    // Additional methods...
};

// Politics system
class Politics 
{
public:
    King* currentKing;
    vector<Leader*> candidates;
    int stabilityIndex;
    int electionCycle;

    // References to other systems
    population** populationSystem;
    Economy* economySystem;
    Army* armySystem;
    Resources* resourceSystem;


    // Constructor & Destructor
    Politics();
    ~Politics();

    // Getters & Setters
    
    // ...

    // Link to other systems
    void linkPopulation(population* pop, int index);
    void linkEconomy(Economy* eco);
    void linkArmy(Army* army);
    void linkResources(Resources* res);

    // Election methods
    void conductElection();
    void addCandidate(Leader* candidate);
    void removeCandidate(int index);

    // Political events
    void handleCoup(double probability);
    void handleAssassination(double probability);
    void calculateStability();

    // Time-based methods
    void simulateDailyUpdate();

    // Display methods
    void displayPoliticalStatus() const;
};