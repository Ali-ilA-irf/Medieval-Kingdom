#pragma once
#include <string>
using namespace std;

// Forward declarations
class population;
class Resources;
class Economy;
class Politics;

// Soldier types
enum class SoldierType { INFANTRY, CAVALRY, ARCHER, SIEGE };

// Unit class represents a group of soldiers of the same type
class Unit {
private:
    SoldierType type;
    int count;
    int strength;
    int morale;
    bool isPaid;
    int trainingLevel;

public:
    // Constructor
    Unit(SoldierType t, int c, int s, int m);

    // Getters
    SoldierType getType() const;
    int getCount() const;
    int getStrength() const;
    int getMorale() const;
    bool getIsPaid() const;
    int getTrainingLevel() const;

    // Setters
    void setCount(int c);
    void setStrength(int s);
    void setMorale(int m);
    void setIsPaid(bool p);
    void setTrainingLevel(int t);

    // Methods
    void trainUnit(int days);
    void battle(int enemyStrength);
    int calculatePower() const;
    bool checkDesertion() const;
};

// General class represents a military leader
class General {
private:
    string name;
    int leadership;
    int experience;
    int loyalty;
    bool isCorrupt;

public:
    // Constructor
    General(string n, int lead, int exp, int loy);

    // Getters
    string getName() const;
    int getLeadership() const;
    int getExperience() const;
    int getLoyalty() const;
    bool getIsCorrupt() const;

    // Setters
    void setLeadership(int l);
    void setExperience(int e);
    void setLoyalty(int l);
    void setIsCorrupt(bool c);

    // Methods
    void gainExperience(int amount);
    bool attemptCoup() const;
    int provideMoraleBoost() const;
};

// Training facility for soldiers
class Barracks {
private:
    int capacity;
    int trainers;
    int quality;
    bool operational;

public:
    // Constructor
    Barracks(int c, int t, int q);

    // Getters
    int getCapacity() const;
    int getTrainers() const;
    int getQuality() const;
    bool getOperational() const;

    // Methods
    int trainSoldiers(int recruits, int days);
    void upgradeBarracks(int investment);
    void displayBarracksStatus() const;
};

// Weapon production facility
class Blacksmith {
private:
    int smiths;
    int production;
    int weaponQuality;
    int resourcesAvailable;

public:
    // Constructor
    Blacksmith(int s, int p, int q);

    // Methods
    int produceWeapons(int ironAmount);
    void upgradeForge(int investment);
    void displayBlacksmithStatus() const;
};

// Main Army class that manages all military aspects
class Army {
private:
    // Units
    Unit* infantry;
    Unit* cavalry;
    Unit* archers;
    Unit* siegeUnits;

    // Army state
    int totalSoldiers;
    int morale;
    int supplies;
    int daysWithoutPay;
    bool atWar;

    // Linked systems
    population** populationSystem;
    Resources* resourceSystem;
    Economy* economySystem;
    Politics* politicsSystem;

    // Military facilities
    Barracks* barracks;
    Blacksmith* blacksmith;
    General* general;

    // Economic factors
    double recruitmentRate;
    double militaryBudget;
    double maintenanceCost;
    double maxSizePercent;

public:
    // Constructor & Destructor
    Army(int infantryCount, int cavalryCount, int archerCount, int siegeCount);
    ~Army();

    // Getters
    int getTotalSoldiers() const;
    int getMorale() const;
    bool getAtWar() const;
    General* getGeneral() const;
    double getMilitaryBudget() const;
    double getMaintenanceCost() const;

    // Setters
    void setMorale(int m);
    void setAtWar(bool w);
    void setMilitaryBudget(double budget);

    // System linking methods
    void linkPopulation(population* pop, int index);
    void linkResources(Resources* res);
    void linkEconomy(Economy* eco);
    void linkPolitics(Politics* pol);

    // Army management methods
    int calculateMaxArmySize() const;
    double calculateMaintenance() const;
    void appointGeneral(string name, int leadership, int experience, int loyalty);
    void recruitSoldiers(double amount, SoldierType type);
    void trainArmy(int days);
    void payArmy(bool paid);
    void feedArmy();

    // Battle methods
    bool declareWar(int enemyStrength);
    void battle(int enemyStrength, int duration);
    int calculateArmyStrength() const;

    // Crisis management
    void handleDesertion();
    void suppressRiots(int i);

    // Game loop update
    void simulateDailyUpdate();

    // Display methods
    void displayArmyStatus() const;
    void displayUnitStatus(SoldierType type) const;
};