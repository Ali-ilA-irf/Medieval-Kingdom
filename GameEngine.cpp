#include "GameEngine.h"
#include "Population.h"
#include "Resource.h"
#include "Army.h"
#include "Economy.h"
#include "King.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

// Player implementation
Player::Player(string n, string k)
    : name(n), kingdom(k), isActive(true)
{
}


vector<string> Player::getMessages() const
{
    return messages;
}



// GameEngine implementation
GameEngine::GameEngine()
    : currentDay(1), gameRunning(false) 
{
    // Initialize pointers to null
    populationSystem = nullptr;
    resourceSystem = nullptr;
    armySystem = nullptr;
    economySystem = nullptr;
    politicsSystem = nullptr;
}

GameEngine::~GameEngine() 
{
    // Clean up population system
    if (populationSystem) 
    {
        // We need to delete all 4 population classes that were created
        for (int i = 0; i < 4; i++)
        {
            if (populationSystem[i])
            {
                delete populationSystem[i];
            }
        }
        delete[] populationSystem;
    }

    // Clean up other systems
    if (resourceSystem) delete resourceSystem;
    if (armySystem) delete armySystem;
    if (economySystem) delete economySystem;
    if (politicsSystem) delete politicsSystem;

    // Clean up players
    for (auto player : players)
    {
        delete player;
    }
    players.clear();
}

void GameEngine::initializeGame() 
{
    cout << "Initializing game systems..." << endl;

    populationSystem = new population * [4];

    populationSystem[0] = new Peasants(0.8, 0.2, 10000, 6000, 3000, 3000, 3000, 1000, 70, 80);

    populationSystem[1] = new Merchants(1000, 800, 400, 400, 100, 100, 80, 70);

 
    populationSystem[2] = new Nobility(80, 50, 200, 500, 300, 150, 150, 100, 100, 90, 95);

    
    populationSystem[3] = new Military(0.85, 0.9, 0.8, 0, 0, 2000, 2000, 1800, 200, 0, 0, 75, 90);

    resourceSystem = new Resources();

    armySystem = new Army(150, 300, 200, 0);

    economySystem = new Economy(10000);

    politicsSystem = new Politics();

    linkSystems();

   

    cout << "Game initialization complete!" << endl;
    gameRunning = true;
}

void GameEngine::linkSystems() 
{
    cout << "Linking game systems..." << endl;

    // Link population to other systems
    for (int i = 0; i < 4; i++) 
    {
        if (populationSystem[i]) 
        {
            populationSystem[i]->seteconomy(economySystem);
            populationSystem[i]->setresources(resourceSystem);
        }
    }

    // Link resources to other systems
    if (resourceSystem) 
    {
        for (int i = 0; i < 4; i++)
        {
            resourceSystem->linkPopulation(populationSystem[i]);
        }
        resourceSystem->linkArmy(armySystem);
        resourceSystem->linkEconomy(economySystem);
    }

    // Link army to other systems
    if (armySystem) 
    {
        for (int i = 0; i < 4; i++) 
        {
            armySystem->linkPopulation(populationSystem[i], i);
        }
        armySystem->linkResources(resourceSystem);
        armySystem->linkEconomy(economySystem);
        armySystem->linkPolitics(politicsSystem);
    }

    // Link economy to other systems
    if (economySystem) 
    {
        for (int i = 0; i < 4; i++) 
        {
            economySystem->linkPopulation(populationSystem[i], i);
        }
        economySystem->linkResources(resourceSystem);
        economySystem->linkArmy(armySystem);
    }

    // Link politics to other systems
    if (politicsSystem) 
    {
        for (int i = 0; i < 4; i++) 
        {
            politicsSystem->linkPopulation(populationSystem[i], i);
        }
        politicsSystem->linkEconomy(economySystem);
        politicsSystem->linkArmy(armySystem);
        politicsSystem->linkResources(resourceSystem);
    }

    cout << "Systems linked successfully!" << endl;
}

void GameEngine::runGameLoop() 
{
    cout << "Starting game loop..." << endl;

    while (gameRunning)
    {
        processDay();
        checkVictoryConditions();

        currentDay++;

      

        if (currentDay > 365) 
        {
            cout << "Year has passed. Saving and exiting game." << endl;
            saveGameState("autosave.dat");
            gameRunning = false;
        }
    }
}

void GameEngine::processDay()
{
    cout << "Day " << currentDay << ":" << endl;


    for (int i = 0; i < 4; i++)
    {
        if (populationSystem[i]) 
        {
            populationSystem[i]->simulatetimepassage();
        }
    }

    if (resourceSystem) 
    {
        resourceSystem->dailyUpdate();
    }

    if (armySystem)
    {
        armySystem->simulateDailyUpdate();
    }

    if (economySystem) 
    {
        economySystem->simulateDailyUpdate();
    }

    if (politicsSystem) 
    {
        politicsSystem->simulateDailyUpdate();
    }

    if (rand() % 100 < 15)
    { // 15% chance per day
        processRandomEvents();
    }

    // Process player actions
    processPlayerActions();

    // Handle multiplayer communication if any
 
    currentDay++;
}

void GameEngine::processRandomEvents()
{
    // Generate a random event
    int eventType = rand() % 5;

    switch (eventType) 
    {
    case 0: 
    {
        // Natural disaster
        string disasterTypes[] = { "Famine", "Disease", "Flood", "Earthquake" };
        string disaster = disasterTypes[rand() % 4];
        cout << "Random event: " << disaster << " has struck the kingdom!" << endl;

        // Apply disaster to each population class
        for (int i = 0; i < 4; i++) 
        {
            if (populationSystem[i]) 
            {
                populationSystem[i]->handleDisaster(disaster);
            }
        }
        break;
    }
    case 1: 
    {
        double marketChange = (rand() % 30 - 15) / 100.0; // -15% to +15%
        cout << "Random event: Market fluctuation of " << (marketChange * 100) << "%" << endl;

        break;
    }
    case 2:
    {
        cout << "Random event: Diplomatic incident with neighboring kingdom" << endl;

        break;
    }
    case 3: 
    {
        cout << "Random event: Border skirmish with raiders" << endl;

        if (armySystem)
        {
            int enemyStrength = 300 + (rand() % 500);
            armySystem->battle(enemyStrength, 1);
        }
        break;
    }
    case 4: 
    {
        int populationChange = (rand() % 300) - 100; // -100 to +200
        cout << "Random event: Population " << (populationChange > 0 ? "immigration" : "emigration")
            << " of " << abs(populationChange) << " people" << endl;

        if (populationSystem[0]) 
        {
            if (populationChange > 0) 
            {
                populationSystem[0]->increasepopulation(populationChange);
            }
            else 
            {
                populationSystem[0]->decreasepopulation(-populationChange);
            }
        }
        break;
    }
    }
}

void GameEngine::processPlayerActions() 
{
    

    bool rebellion = false;
    for (int i = 0; i < 4; i++) 
    {
        if (populationSystem[i] && populationSystem[i]->checkrebelion())
        {
            rebellion = true;
            cout << "Alert: Population rebellion detected!" << endl;

            if (armySystem) 
            {
                armySystem->suppressRiots( i);
            }
            break;
        }
    }

    if (resourceSystem) 
    {
        Resource* food = resourceSystem->getResource(FOOD);
        if (food && food->getQuantity() < 500)
        {
            cout << "Alert: Food supplies critically low!" << endl;

            if (resourceSystem->tradeRouteCount < 3) 
            {
                resourceSystem->addTradeRoute("Emergency Food Supplier", 1000, 0.1);
                resourceSystem->trade(FOOD, 500, 1.5);
            }
        }
    }
}

void GameEngine::saveGameState(string filename) 
{
    cout << "Saving game state to " << filename << "..." << endl;

    ofstream outFile(filename, ios::binary);

    if (!outFile) {
        cout << "Error: Cannot open file for saving." << endl;
        return;
    }

    outFile.write(reinterpret_cast<char*>(&currentDay), sizeof(currentDay));

    int playerCount = players.size();
    outFile.write(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));

    for (const auto& player : players) {
        int nameLength = player->getMessages().size();
        outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    }


    outFile.close();
    cout << "Game saved successfully!" << endl;
}

void GameEngine::loadGameState(string filename) 
{
    cout << "Loading game state from " << filename << "..." << endl;

    ifstream inFile(filename, ios::binary);

    if (!inFile)
    {
        cout << "Error: Cannot open file for loading." << endl;
        return;
    }

    for (auto player : players) 
    {
        delete player;
    }
    players.clear();

    inFile.read(reinterpret_cast<char*>(&currentDay), sizeof(currentDay));

    int playerCount;
    inFile.read(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));

    for (int i = 0; i < playerCount; i++) {
        int messageCount;
        inFile.read(reinterpret_cast<char*>(&messageCount), sizeof(messageCount));

        Player* player = new Player("LoadedPlayer" + to_string(i), "LoadedKingdom");
        players.push_back(player);
    }


    inFile.close();
    cout << "Game loaded successfully!" << endl;
    gameRunning = true;
}

void GameEngine::logScore(string playerName, int score) 
{
    cout << "Logging score for " << playerName << ": " << score << endl;

    ofstream scoreFile("highscores.txt", ios::app);
    if (scoreFile) 
    {
        scoreFile << playerName << "," << score << "," << currentDay << endl;
        scoreFile.close();
    }
}

//void GameEngine::handlePlayerCommunication()
//{
//    // For this example, we'll just check if any messages exist
//    for (auto player : players) {
//        vector<string> messages = player->getMessages();
//        if (!messages.empty()) {
//            cout << "Player " << player->getMessages().size() << " messages pending" << endl;
//        }
//    }
//}
//
//void GameEngine::processAlliances() 
//{
//    for (size_t i = 0; i < players.size(); i++)
//    {
//        for (size_t j = i + 1; j < players.size(); j++) 
//        {
//            if (players[i]->isAllied(players[j]->getMessages()[0]))
//            {
//                // Allied players would get bonuses or shared information
//                cout << "Alliance detected between players " << i << " and " << j << endl;
//            }
//        }
//    }
//}

void GameEngine::checkVictoryConditions()
{
   

    if (economySystem && economySystem->getTreasury() > 100000)
    {
        cout << "Economic victory condition met!" << endl;
        cout << "Player has accumulated vast wealth and won the game!" << endl;

        // Log score
        logScore("Player1", economySystem->getTreasury());

        // End game
        gameRunning = false;
    }

  
    int totalPopulation = 0;
    for (int i = 0; i < 4; i++) 
    {
        if (populationSystem[i])
        {
            totalPopulation += populationSystem[i]->gettotalpopulaiton();
        }
    }

    if (totalPopulation > 50000) {
        cout << "Population victory condition met!" << endl;
        cout << "Kingdom has grown to a prosperous population and won the game!" << endl;

        logScore("Player1", totalPopulation);

        gameRunning = false;
    }
}

void GameEngine::displayGameStatus() const
{
    cout << "========== Kingdom Status: Day " << currentDay << " ==========" << endl;

    cout << "--- Population ---" << endl;
    int totalPopulation = 0;
    for (int i = 0; i < 4; i++)
    {
        if (populationSystem[i])
        {
            populationSystem[i]->displayPopulation();
            totalPopulation += populationSystem[i]->gettotalpopulaiton();
        }
    }
    cout << "Total population: " << totalPopulation << endl;

    // Display resource status
    cout << "--- Resources ---" << endl;
    if (resourceSystem)
    {
        resourceSystem->displayStatus();
    }

    // Display army status
    cout << "--- Military ---" << endl;
    if (armySystem) 
    {
        armySystem->displayArmyStatus();
    }

    // Display economy status
    cout << "--- Economy ---" << endl;
    if (economySystem)
    {
        economySystem->displayEconomyStatus();
    }

    // Display political status
    cout << "--- Politics ---" << endl;
    if (politicsSystem)
    {
        politicsSystem->displayPoliticalStatus();
    }

    cout << "=====================================================" << endl;
}