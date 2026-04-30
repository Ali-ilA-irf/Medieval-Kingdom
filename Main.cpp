#include "GameEngine.h"
#include "Economy.h"
#include <iostream>
#include <string>
#include <limits>
#include <ctime>
#include <cstdlib>

using namespace std;

void displayMainMenu();
void displayGameMenu();
void displayHelpMenu();
void clearScreen();
void pauseScreen();

int main() 
{
    srand(static_cast<unsigned>(time(nullptr)));

    clearScreen();
    cout << "=========================================" << endl;
    cout << "       KINGDOM MANAGEMENT SIMULATOR      " << endl;
    cout << "=========================================" << endl;
    cout << "Welcome to the Kingdom Management Simulator!" << endl;
    cout << "Rule your kingdom wisely to achieve prosperity." << endl;
    cout << endl;
    pauseScreen();

    bool exitGame = false;
    GameEngine* game = nullptr;

    while (!exitGame)
    {
        clearScreen();
        displayMainMenu();

        int choice;
        cout << "Enter your choice: ";
        while (true)
        {
            cin >> choice;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid Input" << endl;
            }
            else if (choice < 1 || choice>4)
            {
                cout << "Invalid Input" << endl;
            }
            else
                break;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) 
        {
        case 1:
        {
            clearScreen();
            cout << "Starting a new game..." << endl;

            if (game) 
            {
                delete game;
            }

            game = new GameEngine();
            game->initializeGame();

            cout << "Game created successfully!" << endl;
            pauseScreen();

            bool inGame = true;
            while (inGame && game) 
            {
                clearScreen();
                displayGameMenu();

                int gameChoice;
                cout << "Enter your choice: ";
                while (true)
                {
                    cin >> gameChoice;
                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid Input" << endl;
                    }
                    else if (gameChoice < 1 || gameChoice>7)
                    {
                        cout << "Invalid Input" << endl;
                    }
                    else
                        break;
                }

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (gameChoice)
                {
                case 1:
                    clearScreen();
                    cout << "Advancing one day..." << endl;
                    game->processDay();
                    pauseScreen();
                    break;

                case 2:
                    clearScreen();
                    cout << "Advancing one week..." << endl;
                    for (int i = 0; i < 7 && game; i++) 
                    {
                        game->processDay();

                        if (!game) break;
                    }
                    cout << "Week completed." << endl;
                    pauseScreen();
                    break;

                case 3:
                    clearScreen();
                    if (game) 
                    {
                        game->displayGameStatus();
                    }
                    pauseScreen();
                    break;

                case 4:
                {
                    clearScreen();
                    string filename;
                    cout << "Enter save filename: ";
                    getline(cin, filename);

                    if (filename.empty()) 
                    {
                        filename = "autosave.dat";
                    }

                    if (game) 
                    {
                        game->saveGameState(filename);
                        game->logScore(filename, game->economySystem->getTreasury());

                    }
                    pauseScreen();
                }
                break;

                case 5:
                {
                    clearScreen();
                    string filename;
                    cout << "Enter load filename: ";
                    getline(cin, filename);

                    if (filename.empty())
                    {
                        filename = "autosave.dat";
                    }

                    if (game)
                    {
                        game->loadGameState(filename);
                    }
                    pauseScreen();
                }
                break;

                case 6:
                    clearScreen();
                    displayHelpMenu();
                    pauseScreen();
                    break;

                case 7:
                    inGame = false;
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
                    pauseScreen();
                    break;
                }
            }
            break;
        }

        case 2: {
            clearScreen();
            string filename;
            cout << "Enter the save file to load: ";
            getline(cin, filename);

            if (filename.empty())
            {
                filename = "autosave.dat";
            }

            if (game) 
            {
                delete game;
            }

            game = new GameEngine();
            game->loadGameState(filename);
            pauseScreen();
            break;
        }

        case 3:
            clearScreen();
            displayHelpMenu();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            
            cout << "Thank you for playing Kingdom Management Simulator!" << endl;
            cout << "Exiting game..." << endl;

            if (game) 
            {
                delete game;
                game = nullptr;
            }

            exitGame = true;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            pauseScreen();
            break;
        }
    }

    return 0;
}

void displayMainMenu() {
    cout << "=========================================" << endl;
    cout << "       KINGDOM MANAGEMENT SIMULATOR      " << endl;
    cout << "=========================================" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Help" << endl;
    cout << "4. Exit" << endl;
    cout << "=========================================" << endl;
}

void displayGameMenu() {
    cout << "=========================================" << endl;
    cout << "             GAME MENU                   " << endl;
    cout << "=========================================" << endl;
    cout << "1. Advance One Day" << endl;
    cout << "2. Advance One Week" << endl;
    cout << "3. Display Kingdom Status" << endl;
    cout << "4. Save Game" << endl;
    cout << "5. Load Game" << endl;
    cout << "6. Help" << endl;
    cout << "7. Exit to Main Menu" << endl;
    cout << "=========================================" << endl;
}

void displayHelpMenu() {
    cout << "=========================================" << endl;
    cout << "               HELP                      " << endl;
    cout << "=========================================" << endl;
    cout << "Kingdom Management Simulator is a strategy game" << endl;
    cout << "where you manage various aspects of a medieval kingdom." << endl;
    cout << endl;
    cout << "Key Systems:" << endl;
    cout << "- Population: Manage different social classes" << endl;
    cout << "- Resources: Gather and distribute resources" << endl;
    cout << "- Economy: Handle taxes, treasury, and trade" << endl;
    cout << "- Military: Train and command your army" << endl;
    cout << "- Politics: Deal with leadership and stability" << endl;
    cout << endl;
    cout << "Basic Commands:" << endl;
    cout << "- Use the Game Menu to navigate options" << endl;
    cout << "- Advance time to see your kingdom evolve" << endl;
    cout << "- Save regularly to prevent loss of progress" << endl;
    cout << endl;
    cout << "Victory Conditions:" << endl;
    cout << "- Achieve economic prosperity (100,000+ treasury)" << endl;
    cout << "- Build a large population (50,000+ citizens)" << endl;
    cout << "- Additional conditions may be discovered" << endl;
    cout << "=========================================" << endl;
}

void clearScreen() 
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() 
{
    cout << endl << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}