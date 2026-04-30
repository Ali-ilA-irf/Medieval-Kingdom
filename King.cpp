#include "King.h"
#include "Population.h"
#include "Economy.h"
#include "Army.h"
#include "Resource.h"
#include <iostream>
using namespace std;
#include <cstdlib>  
#include <ctime>    

Leader::Leader(string n, int c, int i, int e)
    : name(n), charisma(c), intelligence(i), experience(e)
{
}
   

Leader::~Leader() 
{
    
}


King::King(string n, int c, int i, int e)
    : Leader(n, c, i, e), reign(0), popularity(50.0), isAlive(true), decisionStyle(rand() % 3)
{
    
}

double King::getPopularity() 
{
    return popularity;
}

void King::makeDecision() 
{
    
    cout << "King " << name << " is making a decision..." << endl;

    switch (decisionStyle) 
    {
    case 0: 
        cout << "King " << name << " focuses on military matters." << endl;
        break;
    case 1: 
        cout << "King " << name << " focuses on economic matters." << endl;
        break;
    case 2: 
        cout << "King " << name << " focuses on diplomatic matters." << endl;
        break;
    }

    experience += 1;
}

int King::calculateInfluence() const 
{
   

    double influence = (charisma * 0.3) + (intelligence * 0.3) +
        (experience * 0.2) + (popularity * 0.2);

    if (influence > 100) influence = 100;

    return influence;
}

void King::displayStatus() const 
{
    cout << "===== KING STATUS =====" << endl;
    cout << "Name: " << name << endl;
    cout << "Years of Reign: " << reign << endl;
    cout << "Popularity: " << popularity << "%" << endl;
    cout << "Charisma: " << charisma << endl;
    cout << "Intelligence: " << intelligence << endl;
    cout << "Experience: " << experience << endl;

    cout << "Leadership Style: ";
    switch (decisionStyle)
    {
    case 0: cout << "Military" << endl; break;
    case 1: cout << "Economic" << endl; break;
    case 2: cout << "Diplomatic" << endl; break;
    }

    cout << "Status: " << (isAlive ? "Alive" : "Deceased") << endl;
    cout << "======================" << endl;
}

void King::setTaxPolicy(double rate) 
{
   
    if (rate > 20.0) 
    {
        popularity -= (rate - 20.0) * 0.5;
    }
    else 
    {
       
        popularity += (20.0 - rate) * 0.2;
    }

    if (popularity < 0) popularity = 0;
    if (popularity > 100) popularity = 100;

    cout << "King " << name << " has set the tax rate to " << rate << "%" << endl;
    cout << "The king's popularity is now " << popularity << "%" << endl;
}

bool King::surviveAssassinationAttempt(double probability) 
{
    
    int survivedRoll = rand() % 100;

    int survivalBonus = (intelligence / 2) + (charisma / 4);

    bool survived = (survivedRoll + survivalBonus > probability);

    if (!survived) 
    {
        isAlive = false;
        cout << "King " << name << " has been assassinated!" << endl;
    }
    else 
    {
        experience += 5;
        cout << "King " << name << " survived an assassination attempt!" << endl;
    }

    return survived;
}

void King::adjustPopularity(int amount) 
{
    popularity += amount;

    if (popularity < 0) popularity = 0;
    if (popularity > 100) popularity = 100;

    cout << "King " << name << "'s popularity ";
    if (amount > 0) {
        cout << "increased by " << amount << "%" << endl;
    }
    else {
        cout << "decreased by " << -amount << "%" << endl;
    }
    cout << "Current popularity: " << popularity << "%" << endl;
}

// -----------------------------------------------------//
Politics::Politics() 
{
    currentKing = new King("Default King", 5, 5, 5);
    stabilityIndex = 50;
    electionCycle = 0;

    populationSystem = new population * [4];
    for (int i = 0; i < 4; i++) 
    {
        populationSystem[i] = nullptr;
    }

    economySystem = nullptr;
    armySystem = nullptr;
    resourceSystem = nullptr;
}


Politics::~Politics() 
{
    delete currentKing;

    for (int i = 0; i < candidates.size(); i++) 
    {
        delete candidates[i];
    }
    candidates.clear();

    delete[] populationSystem;
}

void Politics::linkPopulation(population* pop, int index) 
{
    if (index >= 0 && index < 4) 
    {
        populationSystem[index] = pop;
    }
}

void Politics::linkEconomy(Economy* eco) 
{
    economySystem = eco;
}

void Politics::linkArmy(Army* army) 
{
    armySystem = army;
}

void Politics::linkResources(Resources* res) 
{
    resourceSystem = res;
}

void Politics::conductElection() 
{
    cout << "Conducting royal election..." << endl;

    if (candidates.empty()) 
    {
        cout << "No candidates available for election!" << endl;
        return;
    }

    vector<int> votes(candidates.size(), 0);

    for (size_t i = 0; i < candidates.size(); i++) 
    {
        votes[i] = (candidates[i]->calculateInfluence() * 10);
        cout << "Candidate " << i + 1 << " received " << votes[i] << " votes." << endl;
    }

    int maxVotes = -1;
    int winnerIndex = -1;

    for (size_t i = 0; i < votes.size(); i++) 
    {
        if (votes[i] > maxVotes) 
        {
            maxVotes = votes[i];
            winnerIndex = i;
        }
    }

    if (winnerIndex >= 0) 
    {
        Leader* winner = candidates[winnerIndex];

        if (currentKing != nullptr)
        {
            delete currentKing;
        }

        
        currentKing = new King(winner->getName(), winner->getCharisma(),
            winner->getIntelligence(), winner->getExperience());

        cout << "New king elected: " << currentKing->getName() << endl;

        stabilityIndex = 60;  
    }
    else 
    {
        cout << "Election failed! No winner determined." << endl;
        stabilityIndex -= 10; 
    }
}

void Politics::addCandidate(Leader* candidate) 
{
    candidates.push_back(candidate);
    cout << "Added candidate: " << candidate->getName() << endl;
}

void Politics::removeCandidate(int index) 
{
    if (index >= 0 && index < static_cast<int>(candidates.size())) 
    {
        string name = candidates[index]->getName();

        delete candidates[index];

        candidates.erase(candidates.begin() + index);

        cout << "Removed candidate: " << name << endl;
    }
    else
    {
        cout << "Invalid candidate index!" << endl;
    }
}

void Politics::handleCoup(double probability) 
{
    if (currentKing == nullptr) 
    {
        cout << "Cannot attempt coup: No king currently ruling!" << endl;
        return;
    }

    double coupRoll = (rand() % 100) / 100.0;  

    double adjustedProbability = probability - (currentKing->calculateInfluence() / 200.0);

    if (coupRoll < adjustedProbability) 
    {
        cout << "A coup has succeeded! King " << currentKing->getName() << " has been overthrown!" << endl;

        delete currentKing;
        currentKing = nullptr;

        stabilityIndex -= 30;

        conductElection();
    }
    else 
    {
        cout << "A coup has been attempted but failed! King " << currentKing->getName() << " remains in power." << endl;

        stabilityIndex -= 10;
    }
}

void Politics::handleAssassination(double probability) 
{
    if (currentKing == nullptr) 
    {
        cout << "Cannot attempt assassination: No king currently ruling!" << endl;
        return;
    }

    cout << "Assassination attempt on King " << currentKing->getName() << "!" << endl;

    if (!currentKing->surviveAssassinationAttempt(probability * 100) )
     {

        delete currentKing;
        currentKing = nullptr;

        stabilityIndex -= 20;

        conductElection();
    }
    else 
    {
        stabilityIndex -= 5;
    }
}

void Politics::calculateStability() 
{
   
    int newStability = stabilityIndex;

    if (currentKing != nullptr) 
    {
        newStability += ((currentKing->calculateInfluence() - 50) / 5);
    }
    else 
    {
        newStability -= 10;
    }

    if (economySystem != nullptr) 
    {
        if (economySystem->getTreasury() > 1000) 
        {
            newStability += 5;
        }
        else if (economySystem->getTreasury() < 100) 
        {
            newStability -= 5;
        }

        if (economySystem->getCorruption() > 50) 
        {
            newStability -= 10;
        }
    }

    if (armySystem != nullptr) 
    {
        if (armySystem->getTotalSoldiers() > 500) 
        {
            newStability += 5;
        }
        else if (armySystem->getTotalSoldiers() < 100) 
        {
            newStability -= 5;
        }

        if (armySystem->getMorale() < 30) 
        {
            newStability -= 10;
        }
    }

    if (newStability < 0) newStability = 0;
    if (newStability > 100) newStability = 100;

    stabilityIndex = newStability;
}

void Politics::simulateDailyUpdate() 
{

    electionCycle++;

    if (electionCycle >= 365) 
    {
        cout << "Election year has arrived!" << endl;
        conductElection();
        electionCycle = 0;  
    }

    calculateStability();

   
    if (stabilityIndex < 30 && currentKing != nullptr) 
    {
        int eventRoll = rand() % 100;

        if (eventRoll < 5) 
        {  
            if (eventRoll < 2) 
            {
                handleCoup(0.3);  
            }
            else 
            {
                handleAssassination(0.3); 
            }
        }
    }

    
    if (currentKing == nullptr && !candidates.empty()) 
    {
        cout << "Kingdom has no ruler! Emergency election called." << endl;
        conductElection();
    }

    if (currentKing != nullptr) 
    {
      
    }
}

void Politics::displayPoliticalStatus() const 
{
    cout << "\n===== POLITICAL STATUS =====" << endl;
    cout << "Stability: " << stabilityIndex << "%" << endl;
    cout << "Days until next election: " << (365 - electionCycle) << endl;

    if (currentKing != nullptr) 
    {
        cout << "\nCurrent Ruler:" << endl;
        currentKing->displayStatus();
    }
    else 
    {
        cout << "\nWARNING: Kingdom has no ruler!" << endl;
    }

    cout << "\nPotential Candidates: " << candidates.size() << endl;
    for (size_t i = 0; i < candidates.size(); i++) 
    {
        cout << (i + 1) << ". " << candidates[i]->getName()
            << " (Influence: " << candidates[i]->calculateInfluence() << ")" << endl;
    }

    cout << "============================" << endl;
}

string Leader::getName()  
{
    return name;
}

int Leader::getCharisma()  
{
    return charisma;
}

int Leader::getIntelligence()  
{
    return intelligence;
}

int Leader::getExperience()  
{
    return experience;
}

void Leader::setName(string n) 
{
    name = n;
}

void Leader::setCharisma(int c)
{
    charisma = c;
}

void Leader::setIntelligence(int i) 
{
    intelligence = i;
}

void Leader::setExperience(int e)
{
    experience = e;
}