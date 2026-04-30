
#pragma once
#include "GameEngine.h"
#include "Population.h"
#include "Resource.h"
#include "Army.h"
#include "Economy.h"
#include "King.h"
#include <string>
using namespace std;

// Forward declarations
class population;
class Army;
class Economy;

// Resource types
enum ResourceType { FOOD, WOOD, STONE, IRON, GOLD, WEAPONS };

// Resource class
class Resource 
{
private:
    ResourceType type;
    int quantity;
    int maxStorage;
    int productionRate;
    int consumptionRate;

public:
    Resource(ResourceType t, int q, int maxS, int prodR, int consR);

    // Getters/Setters
    ResourceType getType() const;
    int getQuantity() const;
    void setQuantity(int q);
    int getMaxStorage() const;
    int getProductionRate() const;
    int getConsumptionRate() const;
    string getTypeName() const;

    // Methods
    void add(int amount);
    void remove(int amount);
    void updateProduction(int laborers);
    bool isLowProdcution() const;
    bool isLowconsumption()const;
};

// Production facility base class
class Facility 
{
protected:
    int workers;
    int productivityRate;
    bool operational;

public:
    Facility(int w, int p);

    // Getters/Setters
    int getWorkers() const;
    void setWorkers(int w);
    int getProductivity() const;
    bool isOperational() const;

    // Methods
    virtual void upgrade(int investment);
    virtual double produce(int amount) = 0;  // Pure virtual
};

// Farm for food production
class Farm : public Facility 
{
private:
    bool hasIrrigation;
    double weatherImpact;

public:
    Farm(int workers, int productivity, bool irrigation, double impact);

    void setWeatherImpact(double impact);
    double produce(int amount) override;
};

// Mine for stone, iron, gold
class Mine : public Facility 
{
private:
    ResourceType resourceType;
    double depletion;

public:
    Mine(ResourceType type, int workers, int productivity);

    ResourceType getResourceType() const;
    double produce(int amount) override;
    void checkDepletion();
};

// Forest for wood
class Forest : public Facility {
private:
    int regrowthRate;

public:
    Forest(int g, int workers, int productivity);

    double produce(int amount) override;
    void plant(int area);
};

// Trade route class
class TradeRoute 
{
private:
    string destination;
    bool secure;
    int capacity;
    double taxRate;

public:
    TradeRoute(string dest, int cap, double tax);

    bool isSecure() const;
    void setSecure(bool s);
    int getCapacity() const;
    double getTaxRate() const;

    bool attemptTrade(ResourceType type, int amount,Resource*r);
};

// Weather system
class Weather 
{
private:
    string condition;
    int severity;
    int duration;

public:
    Weather();

    double getFarmingImpact() const;
    void simulateChange();
    bool isDisaster() const;
};

// Main Resources class
class Resources 
{
private:
    // Individual resources
    Resource* resources[6];  // Array for all resource types

    // Production facilities
    Farm* farm;
    Mine* stoneMine;
    Mine* ironMine;
    Mine* goldMine;
    Forest* forest;

    // Trade and weather
   
    Weather* weather;

    // System connections
    population** populationSystem;
    Army* armySystem;
    Economy* economySystem;

public:
    TradeRoute** tradeRoutes;
    int tradeRouteCount;
    Resources();
    ~Resources();

    // Resource getters
    Resource* getResource(ResourceType type) const;

    // Facility getters
    Farm* getFarm() const;
    Mine* getStoneMine() const;
    Mine* getIronMine() const;
    Mine* getGoldMine() const;
    Forest* getForest() const;

    // System connections
    void linkPopulation(population* pop);
    void linkArmy(Army* army);
    void linkEconomy(Economy* eco);

    // Resource methods
    bool consume(ResourceType type, int amount);
    void produce(ResourceType type , int amount);

    // Trade methods
    void addTradeRoute(string destination, int capacity, double tax);
    bool trade(ResourceType type, int amount, double price);


    // Status methods
    bool canSustainPopulation() const;
    bool canSustainArmy() const;
    void displayStatus() const;

    // Daily update (for game loop)
    void dailyUpdate();
};