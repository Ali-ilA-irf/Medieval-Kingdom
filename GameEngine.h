#pragma once
#include <string>
#include <vector>
using namespace std;
//
// Forward declarations
class population;
class Resources;
class Army;
class Economy;
class Politics;

// Player class for multiplayer
class Player 
{
private:
    string name;
    string kingdom;
    bool isActive;
    vector<string> messages;
  /*  vector<string> allies;
    vector<string> enemies;*/

public:
    // Constructor
    Player(string n, string k);
    void setActive(bool active);
    // Methods
    vector<string> getMessages() const;

    /*void sendMessage(string recipient, string message);
    void addAlly(string ally);
    void removeAlly(string ally);
    bool isAllied(string kingdom) const;*/
    // Additional methods...
};

// Game engine class
class GameEngine 
{
private:
    // Game systems
    population** populationSystem;
    Resources* resourceSystem;
    Army* armySystem;
    Politics* politicsSystem;

    // Game state
    int currentDay;
    bool gameRunning;
    vector<Player*> players;

public:
    Economy* economySystem;

    // Constructor & Destructor
    GameEngine();
    ~GameEngine();

    // Game initialization
    void initializeGame();
    void linkSystems();

    // Game loop methods
    void runGameLoop();
    void processDay();
    void processRandomEvents();
    void processPlayerActions();

    // File handling
    void saveGameState(string filename);
    void loadGameState(string filename);
    void logScore(string playerName, int score);

    //// Multiplayer methods
    //void handlePlayerCommunication();
    //void processAlliances();
    void checkVictoryConditions();

    // Display methods
    void displayGameStatus() const;
    void addPlayer(string name, string kingdom);
};