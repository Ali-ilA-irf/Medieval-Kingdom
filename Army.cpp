#include "Army.h"
#include "Population.h"
#include "Resource.h"
#include "Economy.h"
#include <iostream>
#include <cstdlib>

// ==================== Unit Class Implementation ====================

Unit::Unit(SoldierType t, int c, int s, int m)
    : type(t), count(c), strength(s), morale(m), isPaid(true), trainingLevel(1)
{
}

SoldierType Unit::getType() const
{
    return type;
}

int Unit::getCount() const
{
    return count;
}

int Unit::getStrength() const
{
    return strength;
}

int Unit::getMorale() const
{
    return morale;
}

bool Unit::getIsPaid() const
{
    return isPaid;
}

int Unit::getTrainingLevel() const
{
    return trainingLevel;
}

void Unit::setCount(int c)
{
    count = (c < 0) ? 0 : c;
}

void Unit::setStrength(int s)
{
    strength = s;
}

void Unit::setMorale(int m)
{
    morale = (m < 0) ? 0 : (m > 100) ? 100 : m;
}

void Unit::setIsPaid(bool p)
{
    isPaid = p;
}

void Unit::setTrainingLevel(int t)
{
    trainingLevel = (t < 1) ? 1 : (t > 10) ? 10 : t;
}

void Unit::trainUnit(int days)
{
    if (days <= 0)
        return;

    int oldTraining = trainingLevel;
    trainingLevel += (days / 10);
    if (trainingLevel > 10)
        trainingLevel = 10;

    int oldStrength = strength;
    strength += (trainingLevel - oldTraining) * 5;

    cout << "Unit trained for " << days << " days. Training level increased from "
        << oldTraining << " to " << trainingLevel << ". Strength increased from "
        << oldStrength << " to " << strength << "." << endl;
}

void Unit::battle(int enemyStrength)
{
    int powerDifference = calculatePower() - enemyStrength;

    int casualties = 0;
    if (powerDifference < 0)
    {
        casualties = count * (0.1 + (abs(powerDifference) / 1000.0));
    }
    else
    {
        casualties = count * (0.05 - (powerDifference / 2000.0));
        if (casualties < 0) casualties = 0;
    }

    casualties = min(casualties, count);
    setCount(count - casualties);

    if (powerDifference > 0)
    {
        setMorale(morale + 10);
        cout << "Battle won! Morale increased." << endl;
    }
    else
    {
        setMorale(morale - 20);
        cout << "Battle lost! Morale decreased." << endl;
    }

    cout << "Battle casualties: " << casualties << " soldiers." << endl;
    cout << "Remaining soldiers: " << count << endl;
}

int Unit::calculatePower() const
{
    double moraleMultiplier = morale / 100.0;
    double paidMultiplier = isPaid ? 1.0 : 0.5;
    return static_cast<int>(count * strength * trainingLevel * moraleMultiplier * paidMultiplier);
}

bool Unit::checkDesertion() const
{
    if (morale < 30 && !isPaid)
    {
        cout << "Low morale and unpaid soldiers may lead to desertion!" << endl;
        return true;
    }
    return false;
}

// ==================== General Class Implementation ====================
using namespace std;
General::General(string n, int lead, int exp, int loy)
    : name(n), leadership(lead), experience(exp), loyalty(loy), isCorrupt(false)
{
}

string General::getName() const
{
    return name;
}

int General::getLeadership() const
{
    return leadership;
}

int General::getExperience() const
{
    return experience;
}

int General::getLoyalty() const
{
    return loyalty;
}

bool General::getIsCorrupt() const
{
    return isCorrupt;
}

void General::setLeadership(int l)
{
    leadership = (l < 1) ? 1 : (l > 100) ? 100 : l;
}

void General::setExperience(int e)
{
    experience = (e < 0) ? 0 : e;
}

void General::setLoyalty(int l)
{
    loyalty = (l < 0) ? 0 : (l > 100) ? 100 : l;
}

void General::setIsCorrupt(bool c)
{
    isCorrupt = c;
}

void General::gainExperience(int amount)
{
    int oldExperience = experience;
    experience += amount;
    int oldLeadership = leadership;

    if (experience / 100 > oldExperience / 100)
    {
        leadership += 5;
        if (leadership > 100) leadership = 100;
    }

    cout << "General " << name << " gained " << amount << " experience points." << endl;
    if (leadership > oldLeadership) 
    {
        cout << "Leadership increased from " << oldLeadership << " to " << leadership << endl;
    }
}

bool General::attemptCoup() const
{
    int coupChance = (leadership / 2) + (experience / 100) - (loyalty / 2);
    coupChance = (coupChance < 5) ? 5 : (coupChance > 95) ? 95 : coupChance;

    int roll = rand() % 100;
    bool coupSuccess = roll < coupChance;

    cout << "General " << name << " is attempting a coup..." << endl;
    cout << "Coup chance: " << coupChance << "%, roll: " << roll << endl;

    if (coupSuccess)
    {
        cout << "COUP SUCCESSFUL! The general has seized power!" << endl;
    }
    else
    {
        cout << "Coup attempt failed!" << endl;
    }

    return coupSuccess;
}

int General::provideMoraleBoost() const
{
    int moraleBoost = leadership / 10;
    cout << "General " << name << " provides a morale boost of " << moraleBoost << " points." << endl;
    return moraleBoost;
}

// ==================== Barracks Class Implementation ====================

Barracks::Barracks(int c, int t, int q)
    : capacity(c), trainers(t), quality(q), operational(true)
{
}

int Barracks::getCapacity() const
{
    return capacity;
}

int Barracks::getTrainers() const
{
    return trainers;
}

int Barracks::getQuality() const
{
    return quality;
}

bool Barracks::getOperational() const
{
    return operational;
}

int Barracks::trainSoldiers(int recruits, int days)
{
    if (!operational) 
    {
        cout << "Barracks not operational, cannot train soldiers." << endl;
        return 0;
    }

    if (recruits > capacity)
    {
        cout << "Barracks capacity exceeded, can only train " << capacity << " recruits." << endl;
        recruits = capacity;
    }
    double trainingEffectiveness = ((double)trainers / recruits) * (quality / 10.0);
    trainingEffectiveness = (trainingEffectiveness > 1.0) ? 1.0 : trainingEffectiveness;

    int trainedSoldiers = static_cast<int>(recruits * trainingEffectiveness);

    cout << "Training " << recruits << " recruits for " << days << " days." << endl;
    cout << "Training effectiveness: " << (trainingEffectiveness * 100) << "%" << endl;
    cout << "Successfully trained " << trainedSoldiers << " soldiers." << endl;

    return trainedSoldiers;
}

void Barracks::upgradeBarracks(int investment)
{
    if (investment <= 0)
    {
        cout << "Invalid investment amount." << endl;
        return;
    }

    int oldCapacity = capacity;
    int oldQuality = quality;

    capacity += (investment / 100);
    quality += (investment / 200);

    if (quality > 10) quality = 10;

    cout << "Barracks upgraded with investment of " << investment << " gold." << endl;
    cout << "Capacity increased from " << oldCapacity << " to " << capacity << endl;
    cout << "Quality increased from " << oldQuality << " to " << quality << endl;
}

void Barracks::displayBarracksStatus() const
{
    cout << "----- Barracks Status -----" << endl;
    cout << "Operational: " << (operational ? "Yes" : "No") << endl;
    cout << "Capacity: " << capacity << " recruits" << endl;
    cout << "Trainers: " << trainers << endl;
    cout << "Quality: " << quality << "/10" << endl;
    cout << "--------------------------" << endl;
}

// ==================== Blacksmith Class Implementation ====================

Blacksmith::Blacksmith(int s, int p, int q)
    : smiths(s), production(p), weaponQuality(q), resourcesAvailable(0)
{
}

int Blacksmith::produceWeapons(int ironAmount)
{
    resourcesAvailable = ironAmount;
    int weaponsProduced = static_cast<int>(smiths * production * (resourcesAvailable / 100.0));

    cout << "Blacksmith producing weapons..." << endl;
    cout << "Smiths: " << smiths << ", Production rate: " << production << endl;
    cout << "Iron available: " << resourcesAvailable << endl;
    cout << "Weapons produced: " << weaponsProduced << " units" << endl;
    cout << "Weapon quality: " << weaponQuality << "/10" << endl;

    resourcesAvailable = 0;
    return weaponsProduced;
}

void Blacksmith::upgradeForge(int investment)
{
    if (investment <= 0) 
    {
        cout << "Invalid investment amount." << endl;
        return;
    }

    int oldProduction = production;
    int oldQuality = weaponQuality;

    production += (investment / 150);
    weaponQuality += (investment / 300);

    if (weaponQuality > 10) weaponQuality = 10;

    cout << "Blacksmith upgraded with investment of " << investment << " gold." << endl;
    cout << "Production increased from " << oldProduction << " to " << production << endl;
    cout << "Weapon quality increased from " << oldQuality << " to " << weaponQuality << endl;
}

void Blacksmith::displayBlacksmithStatus() const
{
    cout << "----- Blacksmith Status -----" << endl;
    cout << "Smiths: " << smiths << endl;
    cout << "Production Rate: " << production << endl;
    cout << "Weapon Quality: " << weaponQuality << "/10" << endl;
    cout << "Current Resources: " << resourcesAvailable << endl;
    cout << "-----------------------------" << endl;
}

// ==================== Army Class Implementation ====================

Army::Army(int infantryCount, int cavalryCount, int archerCount, int siegeCount)
    : totalSoldiers(infantryCount + cavalryCount + archerCount + siegeCount),
    morale(70), supplies(100), daysWithoutPay(0), atWar(false),
    populationSystem(nullptr), resourceSystem(nullptr), economySystem(nullptr), politicsSystem(nullptr),
    barracks(nullptr), blacksmith(nullptr), general(nullptr),
    recruitmentRate(0.05), militaryBudget(5000), maintenanceCost(500), maxSizePercent(0.1)
{
    // Initialize units
    infantry = new Unit(SoldierType::INFANTRY, infantryCount, 10, 70);
    cavalry = new Unit(SoldierType::CAVALRY, cavalryCount, 20, 75);
    archers = new Unit(SoldierType::ARCHER, archerCount, 15, 65);
    siegeUnits = new Unit(SoldierType::SIEGE, siegeCount, 30, 60);

    barracks = new Barracks(200, 20, 5);
    blacksmith = new Blacksmith(10, 5, 5);
    populationSystem = new population * [4];
    for (int i = 0; i < 4; i++) 
    {
        populationSystem[i] = nullptr;
    }
    general = new General("Asim Munir", 80, 100, 70);
    maintenanceCost = calculateMaintenance();

    cout << "Army initialized with " << totalSoldiers << " total soldiers." << endl;
}

Army::~Army()
{
    delete infantry;
    delete cavalry;
    delete archers;
    delete siegeUnits;
    delete barracks;
    delete blacksmith;
    if (general) delete general;

    cout << "Army system destroyed." << endl;
}

int Army::getTotalSoldiers() const
{
    return totalSoldiers;
}

int Army::getMorale() const
{
    return morale;
}

bool Army::getAtWar() const
{
    return atWar;
}

General* Army::getGeneral() const
{
    return general;
}

double Army::getMilitaryBudget() const
{
    return militaryBudget;
}

double Army::getMaintenanceCost() const
{
    return maintenanceCost;
}

void Army::setMorale(int m)
{
    morale = (m < 0) ? 0 : (m > 100) ? 100 : m;
}

void Army::setAtWar(bool w)
{
    atWar = w;
    if (atWar)
    {
        cout << "The kingdom is now at war!" << endl;
    }
    else 
    {
        cout << "The kingdom is now at peace." << endl;
    }
}

void Army::setMilitaryBudget(double budget)
{
    militaryBudget = budget;
    cout << "Military budget set to " << militaryBudget << " gold." << endl;
}

void Army::linkPopulation(population* pop, int index)
{
    if (index >= 0 && index < 4) {
        populationSystem[index] = pop;
        cout << "Population system linked to army at index " << index << endl;
    }
}

void Army::linkResources(Resources* res)
{
    resourceSystem = res;
    cout << "Resource system linked to army." << endl;
}

void Army::linkEconomy(Economy* eco)
{
    economySystem = eco;
    cout << "Economy system linked to army." << endl;
}

void Army::linkPolitics(Politics* pol)
{
    politicsSystem = pol;
    cout << "Politics system linked to army." << endl;
}

int Army::calculateMaxArmySize() const
{
    if (populationSystem && populationSystem[0])
    {
        int totalPopulation = 0;
        for (int i = 0; i < 4; i++) 
        {
            if (populationSystem[i]) 
            {
                totalPopulation += populationSystem[i]->gettotalpopulaiton();
            }
        }
        return static_cast<int>(totalPopulation * maxSizePercent);
    }
    else
    {
        return 1000;
    }
}

double Army::calculateMaintenance() const
{
    double infantryCost = infantry->getCount() * 0.5;
    double cavalryCost = cavalry->getCount() * 1.5;
    double archerCost = archers->getCount() * 0.8;
    double siegeCost = siegeUnits->getCount() * 2.0;

    double barracksCost = barracks->getOperational() ? 50 : 0;
    double blacksmithCost = 30;

    double totalCost = infantryCost + cavalryCost + archerCost + siegeCost + barracksCost + blacksmithCost;

    
    if (general)
    {
        totalCost += 100 + (general->getLeadership() * 2);
    }

    return totalCost;
}

void Army::appointGeneral(string name, int leadership, int experience, int loyalty)
{
    if (general) 
    {
        cout << "Dismissing current general " << general->getName() << endl;
        delete general;
    }

    general = new General(name, leadership, experience, loyalty);
    cout << "Appointed " << name << " as general with leadership " << leadership
        << ", experience " << experience << ", and loyalty " << loyalty << endl;

    int boost = general->provideMoraleBoost();
    setMorale(morale + boost);
}

void Army::recruitSoldiers(double amount, SoldierType type)
{
    int maxSize = calculateMaxArmySize();
    if (totalSoldiers + amount > maxSize)
    {
        cout << "Cannot recruit more than max army size (" << maxSize << ")."
            << " Current size: " << totalSoldiers << endl;
        amount = maxSize - totalSoldiers;
        if (amount <= 0) 
        {
            cout << "Army is already at maximum size." << endl;
            return;
        }
    }

    if (!barracks->getOperational())
    
    {
        cout << "Barracks not operational, cannot recruit soldiers." << endl;
        return;
    }

    double recruitmentCost = amount * 10.0; 
    if (economySystem) 
    {
        if (economySystem->getTreasury() < recruitmentCost)
        {
            cout << "Not enough gold in treasury for recruitment." << endl;
            return;
        }
        economySystem->setTreasury(economySystem->getTreasury() - recruitmentCost);
        cout << "Spent " << recruitmentCost << " gold on recruitment." << endl;
    }

    int trainedAmount = barracks->trainSoldiers(amount, 30);

    switch (type)
    {
    case SoldierType::INFANTRY:
        infantry->setCount(infantry->getCount() + trainedAmount);
        cout << "Added " << trainedAmount << " infantry soldiers." << endl;
        break;
    case SoldierType::CAVALRY:
        cavalry->setCount(cavalry->getCount() + trainedAmount);
        cout << "Added " << trainedAmount << " cavalry soldiers." << endl;
        break;
    case SoldierType::ARCHER:
        archers->setCount(archers->getCount() + trainedAmount);
        cout << "Added " << trainedAmount << " archer soldiers." << endl;
        break;
    case SoldierType::SIEGE:
        siegeUnits->setCount(siegeUnits->getCount() + trainedAmount);
        cout << "Added " << trainedAmount << " siege unit operators." << endl;
        break;
    }

    totalSoldiers += trainedAmount;

    maintenanceCost = calculateMaintenance();
}

void Army::trainArmy(int days)
{
    cout << "Training the entire army for " << days << " days..." << endl;

    infantry->trainUnit(days);
    cavalry->trainUnit(days);
    archers->trainUnit(days);
    siegeUnits->trainUnit(days);

    if (general) 
    {
        general->gainExperience(days);
    }

    setMorale(morale + (days / 10));

    double trainingCost = days * totalSoldiers * 0.1;
    if (economySystem) {
        if (economySystem->getTreasury() >= trainingCost)
        {
            economySystem->setTreasury(economySystem->getTreasury() - trainingCost);
            cout << "Spent " << trainingCost << " gold on training." << endl;
        }
        else
        {
            cout << "Warning: Not enough gold for optimal training." << endl;
            setMorale(morale - 5); 
        }
    }
}

void Army::payArmy(bool paid)
{
    infantry->setIsPaid(paid);
    cavalry->setIsPaid(paid);
    archers->setIsPaid(paid);
    siegeUnits->setIsPaid(paid);

    if (paid)
    {
        if (economySystem)
        {
            double payCost = maintenanceCost;
            if (economySystem->getTreasury() >= payCost) 
            {
                economySystem->setTreasury(economySystem->getTreasury() - payCost);
                cout << "Paid army " << payCost << " gold." << endl;
                daysWithoutPay = 0;
                setMorale(morale + 10); 
            }
            else 
            {
                cout << "Not enough gold to pay the army!" << endl;
                paid = false;
            }
        }
    }

    if (!paid)
    {
        daysWithoutPay++;
        int moraleDecrease = daysWithoutPay * 5;
        setMorale(morale - moraleDecrease);
        cout << "Army not paid for " << daysWithoutPay << " days. Morale decreased by "
            << moraleDecrease << " points." << endl;

        if (infantry->checkDesertion() || cavalry->checkDesertion() ||
            archers->checkDesertion() || siegeUnits->checkDesertion()) 
        {
            handleDesertion();
        }
    }
}

void Army::feedArmy()
{
    if (resourceSystem)
    {
        int foodNeeded = totalSoldiers / 10; 
        bool fedWell = resourceSystem->consume(FOOD, foodNeeded);

        if (fedWell)
        {
            cout << "Army consumed " << foodNeeded << " food units." << endl;
            setMorale(morale + 5); 
        }
        else
        {
            cout << "Not enough food for the army!" << endl;
            setMorale(morale - 10); 

            if (infantry->checkDesertion() || cavalry->checkDesertion() ||
                archers->checkDesertion() || siegeUnits->checkDesertion()) 
            {
                handleDesertion();
            }
        }
    }
}

bool Army::declareWar(int enemyStrength)
{
    if (atWar)
    {
        cout << "Already at war!" << endl;
        return false;
    }

    int armyStrength = calculateArmyStrength();
    cout << "Considering war declaration..." << endl;
    cout << "Our strength: " << armyStrength << ", Enemy strength: " << enemyStrength << endl;

    if (armyStrength < enemyStrength) 
    
    {
        cout << "Our army is weaker than the enemy. War declaration not recommended." << endl;
        if (general) 
        {
            cout << "General " << general->getName() << " advises against declaring war." << endl;
        }
        return false;
    }

    setAtWar(true);

    setMorale(morale + 15);

    cout << "WAR DECLARED! Initial morale boost applied." << endl;
    return true;
}

void Army::battle(int enemyStrength, int duration)
{
    if (!atWar)
    {
        cout << "Cannot battle when not at war." << endl;
        return;
    }

    cout << "\n==== BATTLE BEGINS ====" << endl;

    int ourStrength = calculateArmyStrength();
    cout << "Our army strength: " << ourStrength << endl;
    cout << "Enemy strength: " << enemyStrength << endl;

    if (general) 
    {
        int leadershipBonus = general->getLeadership() * 10;
        ourStrength += leadershipBonus;
        cout << "General " << general->getName() << " provides " << leadershipBonus
            << " bonus strength." << endl;

        general->gainExperience(duration * 5);
    }

    bool victory = ourStrength > enemyStrength;

    infantry->battle(enemyStrength / 4);
    cavalry->battle(enemyStrength / 4);
    archers->battle(enemyStrength / 4);
    siegeUnits->battle(enemyStrength / 4);

    totalSoldiers = infantry->getCount() + cavalry->getCount() +
        archers->getCount() + siegeUnits->getCount();

    if (victory)
    {
        setMorale(morale + 20);
        cout << "VICTORY! Army morale significantly increased." << endl;
    }
    else 
    {
        setMorale(morale - 30);
        cout << "DEFEAT! Army morale significantly decreased." << endl;
    }

    if (totalSoldiers < 100 || morale < 20)
    {
        cout << "Army too depleted or demoralized to continue fighting." << endl;
        setAtWar(false);
    }

    cout << "==== BATTLE ENDS ====" << endl;
    cout << "Remaining soldiers: " << totalSoldiers << endl;
    cout << "Current morale: " << morale << endl;
}

int Army::calculateArmyStrength() const
{
    int infantryPower = infantry->calculatePower();
    int cavalryPower = cavalry->calculatePower();
    int archerPower = archers->calculatePower();
    int siegePower = siegeUnits->calculatePower();

    int totalPower = infantryPower + cavalryPower + archerPower + siegePower;

    double moraleMultiplier = morale / 100.0;
    totalPower = static_cast<int>(totalPower * moraleMultiplier);

    return totalPower;
}

void Army::handleDesertion()
{
    double desertionRate = (100 - morale) / 200.0; 
    if (daysWithoutPay > 10)
    {
        desertionRate += 0.1; 
    }

    int infantryDeserters = static_cast<int>(infantry->getCount() * desertionRate);
    int cavalryDeserters = static_cast<int>(cavalry->getCount() * desertionRate);
    int archerDeserters = static_cast<int>(archers->getCount() * desertionRate);
    int siegeDeserters = static_cast<int>(siegeUnits->getCount() * desertionRate);

    infantry->setCount(infantry->getCount() - infantryDeserters);
    cavalry->setCount(cavalry->getCount() - cavalryDeserters);
    archers->setCount(archers->getCount() - archerDeserters);
    siegeUnits->setCount(siegeUnits->getCount() - siegeDeserters);

    int totalDeserters = infantryDeserters + cavalryDeserters + archerDeserters + siegeDeserters;
    totalSoldiers -= totalDeserters;

    cout << "DESERTION CRISIS! " << totalDeserters << " soldiers have deserted the army!" << endl;
    cout << "Infantry deserters: " << infantryDeserters << endl;
    cout << "Cavalry deserters: " << cavalryDeserters << endl;
    cout << "Archer deserters: " << archerDeserters << endl;
    cout << "Siege unit deserters: " << siegeDeserters << endl;

    setMorale(morale - 10);
}
#include<iomanip>
void Army::simulateDailyUpdate()
{
    
    double dailyCost = calculateMaintenance();
    militaryBudget -= dailyCost;

    bool canPayArmy = (militaryBudget >= 0);
    payArmy(canPayArmy);

    if (!canPayArmy)
    {
        daysWithoutPay++;
        morale -= 2 * daysWithoutPay; 
    }
    else
    {
        daysWithoutPay = 0;
    }

    if (supplies > 0)
    {
        feedArmy();
        supplies--; 
    }
    else
    {
        morale -= 5;
    }
    if (morale > 40 && !atWar)
    {
        morale++;
    }

    if (morale > 100) 
    {
        morale = 100;
    }

    totalSoldiers = infantry->getCount() + cavalry->getCount() +
        archers->getCount() + siegeUnits->getCount();

    if (morale < 20) 
    
    {
        handleDesertion();
    }

    if (atWar)
    {
        morale -= 1;

        supplies--;
    }

    if (general != nullptr) 
    {
        if (atWar) 
        {
            general->gainExperience(1);
        }

        if (general->getLoyalty() < 30 && morale < 30)
        {
            if (general->attemptCoup()) 
            {
                if (politicsSystem != nullptr) 
                {
               
                }
            }
        }
    }

    // Check max army size and adjust recruitment rate
    int maxSize = calculateMaxArmySize();
    if (totalSoldiers >= maxSize) 
    {
        recruitmentRate = 0.0; // Stop recruitment
    }
    else 
    {
        // Adjust recruitment rate based on available population and demand
        recruitmentRate = 0.01 * (1.0 - static_cast<double>(totalSoldiers) / maxSize);
    }
    SoldierType type;
    switch (rand() % 4)
    {
    case 0: type = SoldierType::INFANTRY;break;
    case 1:type = SoldierType::CAVALRY;break;
    case 2:type = SoldierType::ARCHER;break;
    case 3:type = SoldierType::SIEGE;break;
    }
    recruitSoldiers(100*(recruitmentRate*10) , type);
}

void Army::displayArmyStatus() const
{
    cout << "===== ARMY STATUS REPORT =====" << endl;
    cout << "Total Soldiers: " << totalSoldiers << endl;
    cout << "Morale: " << morale << "/100" << endl;
    cout << "War Status: " << (atWar ? "AT WAR" : "At Peace") << endl;
    cout << "Days Without Pay: " << daysWithoutPay << endl;
    cout << "Supplies: " << supplies << " days" << endl;
    cout << "Military Budget: " << fixed << setprecision(2) << militaryBudget << endl;
    cout << "Daily Maintenance: " << fixed << setprecision(2) << maintenanceCost << endl;
    cout << "Max Army Size: " << calculateMaxArmySize() << endl;
    cout << endl;

    cout << "UNIT BREAKDOWN:" << endl;
    cout << "-----------------" << endl;
    displayUnitStatus(SoldierType::INFANTRY);
    displayUnitStatus(SoldierType::CAVALRY);
    displayUnitStatus(SoldierType::ARCHER);
    displayUnitStatus(SoldierType::SIEGE);

    if (general != nullptr) 
    {
        cout << endl;
        cout << "GENERAL: " << general->getName() << endl;
        cout << "Leadership: " << general->getLeadership() << endl;
        cout << "Experience: " << general->getExperience() << endl;
        cout << "Loyalty: " << general->getLoyalty() << endl;
        cout << "Status: " << (general->getIsCorrupt() ? "CORRUPT" : "Honorable") << endl;
    }
    else 
    {
        cout << endl;
        cout << "No general appointed." << endl;
    }

 
    cout << endl;
    cout << "FACILITIES:" << endl;
    if (barracks != nullptr) 
    {
        cout << "Barracks: "
            << (barracks->getOperational() ? "Operational" : "Not Operational")
            << " (Capacity: " << barracks->getCapacity() << ")" << endl;
    }
    if (blacksmith != nullptr) {
        cout << "Blacksmith: Operational" << endl;
    }

    cout << "============================" << endl;
}

void Army::displayUnitStatus(SoldierType type) const
{
    const Unit* unit = nullptr;
    string typeName;

    switch (type) 
    {
    case SoldierType::INFANTRY:
        unit = infantry;
        typeName = "INFANTRY";
        break;
    case SoldierType::CAVALRY:
        unit = cavalry;
        typeName = "CAVALRY";
        break;
    case SoldierType::ARCHER:
        unit = archers;
        typeName = "ARCHERS";
        break;
    case SoldierType::SIEGE:
        unit = siegeUnits;
        typeName = "SIEGE";
        break;
    }

    if (unit != nullptr)
    {
        cout << typeName << ": " << unit->getCount() << " soldiers" << endl;
        cout << "  Strength: " << unit->getStrength() << endl;
        cout << "  Morale: " << unit->getMorale() << endl;
        cout << "  Training Level: " << unit->getTrainingLevel() << endl;
        cout << "  Status: " << (unit->getIsPaid() ? "Paid" : "Unpaid") << endl;
        cout << "  Combat Power: " << unit->calculatePower() << endl;
        cout << "-----------------" << endl;
    }
}
void Army::suppressRiots(int i)
{
    populationSystem[i]->setrebelmeter(populationSystem[i]->getrebelmeter() - 50);
    populationSystem[i]->Effectpopulation(100, 1);
    cout << "Rebelion solved" << endl;
}