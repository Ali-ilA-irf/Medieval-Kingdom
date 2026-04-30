#include "GameEngine.h"
#include "Population.h"
#include "Resource.h"
#include "Army.h"
#include "Economy.h"
#include "King.h"
#include <iostream>
#include <cstdlib>

// ==================== Resource Class Implementation ====================

Resource::Resource(ResourceType t, int q, int maxS, int prodR, int consR)
    : type(t), quantity(q), maxStorage(maxS), productionRate(prodR), consumptionRate(consR)
{
}

ResourceType Resource::getType() const
{
    return type;
}

int Resource::getQuantity() const
{
    return quantity;
}

int Resource::getMaxStorage() const
{
    return maxStorage;
}

int Resource::getProductionRate() const
{
    return productionRate;
}

int Resource::getConsumptionRate() const
{
    return consumptionRate;
}

// Setter
void Resource::setQuantity(int q)
{
    if (q < 0)
        quantity = 0;
    else if (q > maxStorage)
        quantity = maxStorage;
    else
        quantity = q;
}

string Resource::getTypeName() const
{
    switch (type) 
    {
    case ResourceType::FOOD:  return "Food";
    case ResourceType::GOLD:  return "Gold";
    case ResourceType::WOOD:  return "Wood";
    case ResourceType::STONE: return "Stone";
    case ResourceType::IRON:  return "Iron";
    case ResourceType::WEAPONS: return "Weapons";
    default:                  return "Unknown";
    }
}

void Resource::add(int amount)
{
    int oldQuantity = quantity;
    quantity += amount*3;
    if (quantity > maxStorage) quantity = maxStorage;
    cout << "Quantity of " << getTypeName() << " increased from " << oldQuantity
        << " to " << quantity << " (+" << (quantity - oldQuantity) << ")" << endl;
}

void Resource::remove(int amount)
{
    int oldQuantity = quantity;
    quantity -= amount;
    if (quantity < 0) quantity = 0;
    cout << "Quantity of " << getTypeName() << " decreased from " << oldQuantity
        << " to " << quantity << " (-" << (oldQuantity - quantity) << ")" << endl;
}

void Resource::updateProduction(int laborers)
{
    double oldRate = productionRate;
    if (laborers >= 1000)
        productionRate += 0.5;
    else if (laborers >= 500)
        productionRate += 0.2;
    else
        productionRate += 0.1;

    cout << getTypeName() << " production rate updated from " << oldRate
        << " to " << productionRate << endl;
}

bool Resource::isLowProdcution() const
{
    if (productionRate < 0.5) 
    {
        return true;
    }
    return false;
}

bool Resource::isLowconsumption() const
{
    if (consumptionRate < 0.5) 
    {
        return true;
    }
    return false;
}

// ==================== Facility Class Implementation ====================

Facility::Facility(int w, int p)
    : workers(w), productivityRate(p), operational(true)
{
}

int Facility::getWorkers() const
{
    return workers;
}

void Facility::setWorkers(int w)
{
    if (w < 0)
        workers = 0;
    else
        workers = w;

    cout << "Workers adjusted to " << workers << endl;
}

int Facility::getProductivity() const
{
    return productivityRate;
}

bool Facility::isOperational() const
{
    return operational;
}

void Facility::upgrade(int investment)
{
    int oldProductivity = productivityRate;
    productivityRate += (investment / 100);
    cout << "Facility upgraded: Productivity increased from " << oldProductivity
        << " to " << productivityRate << endl;
}

// ==================== Farm Class Implementation ====================

Farm::Farm(int workers, int productivity, bool irrigation, double impact)
    : Facility(workers, productivity), hasIrrigation(irrigation), weatherImpact(impact)
{
}

double Farm::produce(int amount)
{
    if (!operational) 
    {
        cout << "Farm is not operational, no production" << endl;
        return 0;
    }

    int baseProdRate = productivityRate * 100;

    bool irrigationWorking = rand() % 2 == 0 ? false : true;
    int effectiveRate = baseProdRate;

    if (hasIrrigation && irrigationWorking) 
    {
        effectiveRate += 50;
        cout << "Irrigation system is working well, productivity increased" << endl;
    }

    double effectiveProductivity = effectiveRate / 100.0;
    double output = workers * effectiveProductivity * weatherImpact;

    cout << "Farm produced " << output << " food units (Workers: " << workers
        << ", Productivity: " << effectiveProductivity
        << ", Weather impact: " << weatherImpact << ")" << endl;

    return output;
}

void Farm::setWeatherImpact(double impact)
{
    double oldImpact = weatherImpact;
    weatherImpact = impact;

    if (weatherImpact > oldImpact) 
    {
        cout << "Weather conditions improved for farming" << endl;
    }
    else if (weatherImpact < oldImpact) 
    {
        cout << "Weather conditions worsened for farming" << endl;
    }
}

// ==================== Mine Class Implementation ====================

Mine::Mine(ResourceType type, int workers, int productivity)
    : Facility(workers, productivity), resourceType(type), depletion(0.1)
{
}

ResourceType Mine::getResourceType() const
{
    return resourceType;
}

double Mine::produce(int amount)
{
    if (!operational) 
    {
        cout << "Mine is not operational, no production" << endl;
        return 0;
    }

    double depletionFactor = 1.0 - depletion;
    double output = workers * productivityRate * depletionFactor;

    checkDepletion();

    string resourceName;
    switch (resourceType) 
    {
    case STONE: resourceName = "stone"; break;
    case IRON:  resourceName = "iron"; break;
    case GOLD:  resourceName = "gold"; break;
    default:    resourceName = "unknown"; break;
    }

    cout << "Mine produced " << output << " units of " << resourceName
        << " (Workers: " << workers
        << ", Productivity: " << productivityRate
        << ", Depletion factor: " << depletionFactor << ")" << endl;

    return output;
}

void Mine::checkDepletion()
{
    depletion += 0.01;

    if (depletion > 0.9)
    {
        depletion = 0.9;
        cout << "Warning: Mine severely depleted, consider finding new sources" << endl;
    }
    else if (depletion > 0.7)
    {
        cout << "Warning: Mine depletion is significant" << endl;
    }
    else if (depletion > 0.5)
    {
        cout << "Notice: Mine resources are showing signs of depletion" << endl;
    }
}

// ==================== Forest Class Implementation ====================

Forest::Forest(int g, int workers, int productivity)
    : Facility(workers, productivity), regrowthRate(g)
{
}

void Forest::plant(int area)
{
    double oldRate = regrowthRate;

    if (area > 1000) 
    {
        regrowthRate += 2;
        cout << "Large area planted, significant increase in forest regrowth rate" << endl;
    }
    else if (area > 500) 
    {
        regrowthRate += 1;
        cout << "Medium area planted, moderate increase in forest regrowth rate" << endl;
    }
    else
    {
        regrowthRate += 0.5;
        cout << "Small area planted, slight increase in forest regrowth rate" << endl;
    }

    cout << "Forest regrowth rate increased from " << oldRate << " to " << regrowthRate << endl;
}

double Forest::produce(int amount)
{
    if (!operational) 
    {
        cout << "Forest operation is not functional, no production" << endl;
        return 0;
    }

    double output = workers * productivityRate * regrowthRate;

    cout << "Forest produced " << output << " wood units (Workers: " << workers
        << ", Productivity: " << productivityRate
        << ", Regrowth rate: " << regrowthRate << ")" << endl;

    return output;
}

// ==================== TradeRoute Class Implementation ====================

TradeRoute::TradeRoute(string dest, int cap, double tax)
    : destination(dest), capacity(cap), taxRate(tax), secure(true)
{
}

bool TradeRoute::isSecure() const
{
    return secure;
}

void TradeRoute::setSecure(bool s)
{
    secure = s;
    if (secure) {
        cout << "Trade route to " << destination << " is now secure" << endl;
    }
    else {
        cout << "Warning: Trade route to " << destination << " is no longer secure" << endl;
    }
}

int TradeRoute::getCapacity() const
{
    return capacity;
}

double TradeRoute::getTaxRate() const
{
    return taxRate;
}

bool TradeRoute::attemptTrade(ResourceType type, int amount, Resource* r)
{
    int index = type;
    bool isAllowed = (r[index].getQuantity() >= amount && !r[index].isLowProdcution());

    if (isAllowed) 
    {
        cout << "Trade of " << amount << " " << r[index].getTypeName()
            << " to " << destination << " is allowed" << endl;
    }
    else 
    {
        if (r[index].getQuantity() < amount)
        {
            cout << "Trade failed: Not enough " << r[index].getTypeName()
                << " available (" << r[index].getQuantity() << "/" << amount << ")" << endl;
        }
        if (r[index].isLowProdcution())
        {
            cout << "Trade failed: Production rate of " << r[index].getTypeName()
                << " is too low for export" << endl;
        }
    }

    return isAllowed;
}



// ==================== Resources Class Implementation ====================

Resources::Resources()
{
    resources[FOOD] = new Resource(FOOD, 5000, 5000, 100, 50);
    resources[WOOD] = new Resource(WOOD, 800, 3000, 80, 30);
    resources[STONE] = new Resource(STONE, 500, 2000, 50, 20);
    resources[IRON] = new Resource(IRON, 300, 1500, 30, 15);
    resources[GOLD] = new Resource(GOLD, 200, 1000, 20, 10);
    resources[WEAPONS] = new Resource(WEAPONS, 1000, 500, 10, 5);
    tradeRouteCount = 0;
    tradeRoutes = new TradeRoute*[5];

    farm = new Farm(100, 1, false, 0.8);
    stoneMine = new Mine(STONE, 50, 1);
    ironMine = new Mine(IRON, 30, 1);
    goldMine = new Mine(GOLD, 20, 1);
    forest = new Forest(5, 60, 1);

 

    populationSystem = nullptr;
    armySystem = nullptr;
    economySystem = nullptr;

    cout << "Resource system initialized" << endl;
}

Resources::~Resources()
{
    for (int i = 0; i < 6; i++) 
    {
        delete resources[i];
    }

    delete farm;
    delete stoneMine;
    delete ironMine;
    delete goldMine;
    delete forest;

    for (int i = 0; i < tradeRouteCount; i++)
    {
        delete tradeRoutes[i];
    }
    delete[] tradeRoutes;

    delete weather;

    cout << "Resource system destroyed" << endl;
}

Resource* Resources::getResource(ResourceType type) const
{
    return resources[type];
}

Farm* Resources::getFarm() const
{
    return farm;
}

Mine* Resources::getStoneMine() const
{
    return stoneMine;
}

Mine* Resources::getIronMine() const
{
    return ironMine;
}

Mine* Resources::getGoldMine() const
{
    return goldMine;
}

Forest* Resources::getForest() const
{
    return forest;
}

void Resources::linkPopulation(population* pop)
{
    populationSystem = &pop;
    cout << "Population system linked to resources" << endl;
}

void Resources::linkArmy(Army* army)
{
    armySystem = army;
    cout << "Army system linked to resources" << endl;
}

void Resources::linkEconomy(Economy* eco)
{
    economySystem = eco;
    cout << "Economy system linked to resources" << endl;
}

bool Resources::consume(ResourceType type, int amount)
{


    if (resources[type]->getQuantity() >= amount)
    {
        resources[type]->remove(amount);
        return true;
    }
    else 
    {
        cout << "Not enough " << resources[type]->getTypeName() << " available for consumption" << endl;
        return false;
    }
}

void Resources::produce(ResourceType type, int amount)
{
    Resource* resource = resources[type];
    double produced = 0;

    switch (type) 
    {
    case FOOD:
        produced = farm->produce(amount);
        break;
    case WOOD:
        produced = forest->produce(amount);
        break;
    case STONE:
        produced = stoneMine->produce(amount);
        break;
    case IRON:
        produced = ironMine->produce(amount);
        break;
    case GOLD:
        produced = goldMine->produce(amount);
        break;
    case WEAPONS:
        if (consume(IRON, amount / 2)) 
        {
            produced = amount;
            cout << "Weapons forge produced " << produced << " weapon units" << endl;
        }
        else 
        {
            cout << "Not enough iron to produce weapons" << endl;
        }
        break;
    }

    if (produced > 0) 
    {
        resource->add(static_cast<int>(produced));
    }
}

void Resources::addTradeRoute(string destination, int capacity, double tax)
{
    if (tradeRouteCount >= 5) 
    {
        cout << "Cannot add more trade routes, maximum reached" << endl;
        return;
    }

    tradeRoutes[tradeRouteCount] = new TradeRoute(destination, capacity, tax);
    cout << "New trade route established to " << destination << " (Capacity: "
        << capacity << ", Tax rate: " << tax << ")" << endl;
    tradeRouteCount++;
}

bool Resources::trade(ResourceType type, int amount, double price)
{
    if (tradeRouteCount == 0) 
    {
        cout << "No trade routes available for trading" << endl;
        return false;
    }

    for (int i = 0; i < tradeRouteCount; i++) 
    {
        if (!tradeRoutes[i]->isSecure())
        {
            continue; 
        }

        if (tradeRoutes[i]->getCapacity() < amount) 
        {
            cout << "Trade route capacity too small for requested amount" << endl;
            continue;
        }

        if (tradeRoutes[i]->attemptTrade(type, amount, resources[type])) 
        {
            double taxAmount = price * tradeRoutes[i]->getTaxRate();
            double finalPrice = price + taxAmount;

            resources[type]->remove(amount);
            resources[GOLD]->add(static_cast<int>(finalPrice));

            cout << "Trade completed: " << amount << " " << resources[type]->getTypeName()
                << " sold for " << finalPrice << " gold (Tax: " << taxAmount << ")" << endl;

            return true;
        }
    }

    cout << "No suitable trade route found for this trade" << endl;
    return false;
}



bool Resources::canSustainPopulation() const
{
    int foodProduction = resources[FOOD]->getProductionRate();
    int foodConsumption = resources[FOOD]->getConsumptionRate();

    bool sustainable = foodProduction >= foodConsumption;

    if (!sustainable) 
    {
        cout << "WARNING: Food production (" << foodProduction
            << ") is insufficient for consumption (" << foodConsumption << ")" << endl;
    }

    return sustainable;
}

bool Resources::canSustainArmy() const
{
    int foodProduction = resources[FOOD]->getProductionRate();
    int goldProduction = resources[GOLD]->getProductionRate();

    bool foodSustainable = resources[FOOD]->getConsumptionRate() <= (foodProduction * 0.8);
    bool goldSustainable = goldProduction * 0.3 > 0;
    bool weaponsSustainable = resources[WEAPONS]->getQuantity() > 10;

    bool sustainable = foodSustainable && goldSustainable && weaponsSustainable;

    if (!sustainable) 
    {
        cout << "WARNING: Resources insufficient to sustain current army size" << endl;
        if (!foodSustainable)
        {
            cout << "- Food production too low" << endl;
        }
        if (!goldSustainable) 
        {
            cout << "- Gold production too low" << endl;
        }
        if (!weaponsSustainable) 
        {
            cout << "- Weapons stockpile too low" << endl;
        }
    }

    return sustainable;
}

void Resources::displayStatus() const
{
    cout << "\n====== RESOURCE STATUS ======" << endl;

    for (int i = 0; i < 6; i++) 
    {
        cout << resources[i]->getTypeName() << ": " << resources[i]->getQuantity()
            << "/" << resources[i]->getMaxStorage()
            << " (Production: " << resources[i]->getProductionRate()
            << ", Consumption: " << resources[i]->getConsumptionRate() << ")" << endl;
    }

    cout << "\n----- Facilities -----" << endl;
    cout << "Farm: " << farm->getWorkers() << " workers, Productivity: "
        << farm->getProductivity() << endl;
    cout << "Forest: " << forest->getWorkers() << " workers, Productivity: "
        << forest->getProductivity() << endl;
    cout << "Stone Mine: " << stoneMine->getWorkers() << " workers, Productivity: "
        << stoneMine->getProductivity() << endl;
    cout << "Iron Mine: " << ironMine->getWorkers() << " workers, Productivity: "
        << ironMine->getProductivity() << endl;
    cout << "Gold Mine: " << goldMine->getWorkers() << " workers, Productivity: "
        << goldMine->getProductivity() << endl;

    cout << "\n----- Trade Routes -----" << endl;
    for (int i = 0; i < tradeRouteCount; i++) 
    {
        cout << "Route " << (i + 1) << ": Capacity: " << tradeRoutes[i]->getCapacity()
            << ", Tax: " << tradeRoutes[i]->getTaxRate()
            << ", Security: " << (tradeRoutes[i]->isSecure() ? "Secure" : "Insecure") << endl;
    }
    if (tradeRouteCount == 0) 
    {
        cout << "No trade routes established" << endl;
    }

    cout << "\n============================" << endl;
}

void Resources::dailyUpdate()
{
    cout << "\n====== DAILY RESOURCE UPDATE ======" << endl;

   
    produce(FOOD, farm->getWorkers());
    produce(WOOD, forest->getWorkers());
    produce(STONE, stoneMine->getWorkers());
    produce(IRON, ironMine->getWorkers());
    produce(GOLD, goldMine->getWorkers());
    produce(WEAPONS, ironMine->getWorkers());

  
    consume(FOOD, resources[FOOD]->getConsumptionRate());
    consume(WOOD, resources[WOOD]->getConsumptionRate());
    consume(GOLD, resources[GOLD]->getConsumptionRate());

    canSustainPopulation();
    canSustainArmy();

    cout << "====== END OF DAILY UPDATE ======\n" << endl;
}