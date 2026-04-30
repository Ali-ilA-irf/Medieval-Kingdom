#pragma once
#include "GameEngine.h"
#include "Population.h"
#include "Resource.h"
#include "Army.h"
#include "Economy.h"
#include "King.h"
#include <string>
using namespace std;

class population;
class Resources;
class Army;
enum ResourceType;
class Bank 
{
private:
    double interestRate;
    int loanTerm;
    double availableFunds;
    double loansOutstanding;
    
public:
    
    Bank(double rate, int term, double funds);
    void setInterestRate(double rate);
    void setLoanTerm(int term);
    void setAvailableFunds(double funds);
    void setLoansOutstanding(double loans);
    double getInterestRate() const;
    int getLoanTerm() const;
    double getAvailableFunds() const;
    double getLoansOutstanding() const;

    // Methods
    void grantLoan(double amount, int term);
    double calculateInterest(double amount, int term);
    double repayLoan(double amount,double Loanamount);
    void adjustInterestRate(double economicHealth);
    // Additional methods...
};

// Taxation class
class TaxSystem
{
private:
    double peasantTaxRate;
    double merchantTaxRate;
    double nobilityTaxRate;
    double taxEfficiency;

    double defaultpeasanttax;
    double defaultNobilitytax;
    double defaultMerchantstax;


    // Additional properties...

public:
    // Constructor
    TaxSystem(double pRate, double mRate, double nRate, double efficiency);
    double getpeasanttax();
    double getmerchanttax();
    double getnobilityTax();
    double gettaxefficiency();

    double defaultgetpeasanttax();
    double defaultgetmerchanttax();
    double defaultgetnobilityTax();

    void settaxefficiency();
    void setpeasanttax(double a);
    void setmerchanttax(double a);
    void setnobilitytax(double a);

    void setdefaultpeasanttax(double a);
    void setdefaultmerchanttax(double a);
    void setdefaultnobilitytax(double a);


    void adjustTaxRates(double pRate, double mRate, double nRate);
};

class Economy 
{
private:
  
    double treasury;
    double inflation;
    double economicGrowth;
    double corruption;
    double totaltax;
    double Loanamount;
    bool Isloaned;

    population** populationSystem;
    Resources* resourceSystem;
    Army* armySystem;

public:
    Bank* bank;
      TaxSystem* taxSystem;
    Economy(double startingFunds);
    ~Economy();

    double getinflation();
    void setinfaltion(double a);
    double getTreasury() const;
    void setTreasury(double a);

    double getCorruption() const;
    void setCorruption(double c);

    double getLoanAmount() const;
    bool getIsLoaned() const;

    void setLoanAmount(double a);
    void setIsLoaned(bool b);
   
    void linkPopulation(population* pop, int index);
    void linkResources(Resources* res);
    void linkArmy(Army* army);

    void collectTaxes(int tax , int choice);
    void spendFunds(double amount);
    void takeLoan(double amount);
    void repayLoan();
    void handleCorruption();
    void calculateInflation();

    // Time-based methods
    void simulateDailyUpdate();

    // Display methods
    void displayEconomyStatus() const;
    void displayBankStatus() const;
    void displayTaxStatus(int a) const;
};