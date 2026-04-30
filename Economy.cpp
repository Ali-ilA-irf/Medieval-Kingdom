#include "Economy.h"
#include"Population.h"
#include<iostream>
Bank::Bank(double rate, int term, double funds)
    : interestRate(rate), loanTerm(term), availableFunds(funds), loansOutstanding(0.0)
{
}
double Bank::getInterestRate() const 
{
    return interestRate;
}

int Bank::getLoanTerm() const 
{
    return loanTerm;
}

double Bank::getAvailableFunds() const 
{
    return availableFunds;
}

double Bank::getLoansOutstanding() const 
{
    return loansOutstanding;
}
void Bank::setInterestRate(double rate) 
{
    interestRate = rate;
}

void Bank::setLoanTerm(int term) 
{
    loanTerm = term;
}

void Bank::setAvailableFunds(double funds) 
{
    availableFunds = funds;
}

void Bank::setLoansOutstanding(double loans) 
{
    loansOutstanding = loans;
}
void Bank::adjustInterestRate(double economicHealth)
{
    if (loanTerm < 10 && economicHealth>70)
    {
        cout << "InterestRate Adjusted from " << interestRate << " to";
        interestRate -= 0.2;
        cout << interestRate;
        return;
    }
    cout << "Cant adjust the interestRate" << endl;
}
double Bank::calculateInterest(double amount, int term)
{
    return (amount * interestRate * term);
}
double Bank::repayLoan(double amount, double Loanamount)
{
    if (amount < Loanamount)
    {
        loansOutstanding -= amount;
        cout << "Remaining loan is" << loansOutstanding << endl;
        return loansOutstanding;
    }
    else
    {
        loansOutstanding = 0;
        cout << "Loan has been repaid" << endl;
    }
    return loansOutstanding;

}
void Bank::grantLoan(double amount, int term)
{
    if (amount / 1000 == 0) interestRate = 0.05;
    else if (amount / 10000 == 0)interestRate = 0.1;
    else if (amount / 100000 == 0) interestRate = 0.2;
    cout << "Interest Rate is " << interestRate << endl;
    cout << "Do you want to negotiate?\n Y for yes\n N for no" << endl;
    loansOutstanding = amount + calculateInterest(amount, term);
    loanTerm = term;
    cout << "Loan Has been Granted" << endl;
    cout << "Total Amount to be repay is " << loansOutstanding << endl;
}



//---------------------------------------------------------------//
TaxSystem::TaxSystem(double pRate, double mRate, double nRate, double efficiency)
    : peasantTaxRate(pRate), merchantTaxRate(mRate), nobilityTaxRate(nRate), taxEfficiency(efficiency)
{
}
double TaxSystem::getpeasanttax() 
{
    return peasantTaxRate;
}

double TaxSystem::getmerchanttax() 
{
    return merchantTaxRate;
}

double TaxSystem::getnobilityTax() 
{
    return nobilityTaxRate;
}
void TaxSystem::setmerchanttax(double a) 
{
    merchantTaxRate = a;
}
void TaxSystem::setpeasanttax(double a)
{
    peasantTaxRate = a;
}

void TaxSystem::setnobilitytax(double a)
{
    nobilityTaxRate = a;
}
double TaxSystem:: gettaxefficiency()
{
    return taxEfficiency;
}
//--//
double TaxSystem::defaultgetpeasanttax()
{
    return defaultpeasanttax;
}

double TaxSystem::defaultgetmerchanttax()
{
    return defaultMerchantstax;
}

double TaxSystem::defaultgetnobilityTax()
{
    return defaultNobilitytax;
}
void TaxSystem::setdefaultmerchanttax(double a)
{
    defaultMerchantstax = a;
}
void TaxSystem::setdefaultpeasanttax(double a)
{
    defaultpeasanttax = a;
}

void TaxSystem::setdefaultnobilitytax(double a)
{
    defaultNobilitytax = a;
}
void TaxSystem::adjustTaxRates(double pRate, double mRate, double nRate)
{
    peasantTaxRate = pRate;
    merchantTaxRate = mRate;
    nobilityTaxRate = nRate;
}



//-------------------------------------------------//

Economy::Economy(double startingFunds) 
{
    treasury = startingFunds;
    inflation = 0.0;
    economicGrowth = 0.0;
    corruption = 0.0;
    totaltax = 0.0;
    Loanamount = 0.0;
    Isloaned = false;

    populationSystem = new population * [4];
    for (int i = 0; i < 4; i++) {
        populationSystem[i] = nullptr;
    }

    bank = new Bank(0.05, 12, 10000);
    taxSystem = new TaxSystem(0.1, 0.15, 0.2, 0.8);

    resourceSystem = nullptr;
    armySystem = nullptr;
}
Economy::~Economy() 
{
    delete[] populationSystem;

    delete bank;
    delete taxSystem;
}
double Economy::getinflation()
{
    return inflation;
}

void Economy::setinfaltion(double a) {
    inflation = a;
} 
double Economy::getTreasury() const
{ 
    return treasury; 
}
void Economy::setTreasury(double a) 
{ 
    treasury = a; 
}

double Economy::getCorruption() const 
{ 
    return corruption; 
}
void Economy::setCorruption(double c) 
{ 
    corruption = c; 
}

double Economy::getLoanAmount() const 
{ 
    return Loanamount; 
}
bool Economy::getIsLoaned() const 
{ 
    return Isloaned; 
}

void Economy::setLoanAmount(double a) 
{ 
    Loanamount = a; 
}
void Economy::setIsLoaned(bool b) 
{ 
    Isloaned = b; 
}
void Economy::linkPopulation(population* pop, int index) 
{
    if (index >= 0 && index < 4) 
    {
        populationSystem[index] = pop;
    }
}

void Economy::linkResources(Resources* res) 
{
    resourceSystem = res;
}

void Economy::linkArmy(Army* army) 
{
    armySystem = army;
}
void Economy::displayEconomyStatus() const 
{
    cout << "----- Economy Status -----" << endl;
    cout << "Treasury Balance: " << treasury << " gold" << endl;
    cout << "Inflation Rate: " << inflation << "%" << endl;
    cout << "Economic Growth: " << economicGrowth << "%" << endl;
    cout << "Corruption Level: " << corruption << "%" << endl;
    cout << "Loan Taken: " << (Isloaned ? "Yes" : "No") << endl;
    cout << "Outstanding Loan Amount: " << Loanamount << " gold" << endl;
    cout << "--------------------------" << endl;
}
void Economy::displayBankStatus() const
{
    if (!bank) {
        cout << "Bank system is not initialized." << endl;
        return;
    }

    cout << "----- Bank Status -----" << endl;
    cout << "Interest Rate: " << bank->getInterestRate() * 100 << "% per day" << endl;
    cout << "Loan Term (Days): " << bank->getLoanTerm() << endl;
    cout << "Available Funds: " << bank->getAvailableFunds() << " gold" << endl;
    cout << "Loans Outstanding: " << bank->getLoansOutstanding() << " gold" << endl;
    cout << "------------------------" << endl;
}
void Economy::displayTaxStatus(int a) const {
    if (!taxSystem) 
    {
        cout << "Tax system not initialized." << endl;
        return;
    }

    cout << "----- Tax System Status -----" << endl;

       if(a==1) cout << "Peasant Tax Rate: " << taxSystem->getpeasanttax() << " per unit\n";
        if(a==2)cout << "Merchant Tax Rate: " << taxSystem->getmerchanttax() << " per unit\n";
        if(a==3)cout << "Nobility Tax Rate: " << taxSystem->getnobilityTax() << " per unit\n";

    cout << "Tax Collection Efficiency: " << taxSystem->gettaxefficiency() * 100 << "%\n";
    cout << "------------------------------" << endl;
}
void Economy::collectTaxes(int tax, int choice)
{
    switch (choice)
    {
    case 1:
    {
        if (tax < taxSystem->defaultgetpeasanttax())
        {
            economicGrowth -= 10;
            corruption += 10;
        }
    }
    case 2:
    {
        if (tax < taxSystem->defaultgetmerchanttax())
        {
            economicGrowth -= 10;
            corruption += 10;
        }
    }
    case 3:
    {
        if (tax < taxSystem->defaultgetnobilityTax())
        {
            economicGrowth -= 10;
            corruption += 10;
        }
    }
    }
    treasury += tax;
}
void Economy::spendFunds(double amount)
{
    int purpose;
    cout << "What do you want to build?\n 1-Hospital\n 2-School\n 3-People relief" << endl;
    cin >> purpose;
    while (purpose < 1 || purpose>3)
    {
        cin >> purpose;
    }
    switch (purpose)
    {
    case 1:
    {
        if (treasury > 5000) treasury -= 5000;
        else { cout << "Not enough money" << endl;return; }    
        cout << "Build a hospital" << endl;
        for (int i = 0; i < 3; i++)
        {
            populationSystem[i]->sethappymeter(populationSystem[i]->gethappymeter() + 20);
        }

    }
    case 2:
    {
        if (treasury > 2000) treasury -= 2000;
        else { cout << "Not enough money" << endl;return; }
        cout << "Build a School" << endl;
        for (int i = 0; i < 3; i++)
        {
            populationSystem[i]->sethappymeter(populationSystem[i]->gethappymeter() + 10);
        }
    }
    case 3:
    {
        if (treasury > 6000) treasury -= 6000;
        else { cout << "Not enough money" << endl;return; }
        cout << "Food Releif" << endl;
        resourceSystem->produce(ResourceType::FOOD, 6000);
    }
    }
}
void Economy::takeLoan(double amount)
{
    int days;
    cout << "For how many days?" << endl;
    cin >> days;
    while (days < 0)
    {
        cin >> days;
    }
    bank->grantLoan(amount, days);
    Loanamount = bank->getLoansOutstanding();
    Isloaned = true;
    treasury += amount;
}
void Economy::repayLoan()
{
    cout << "Loan on you is " << Loanamount << endl;
    cout << "How much do you want to repay?" << endl;
    int amount;
    cin >> amount;
    while (amount < 0)
    {
        cin >> amount;
    }
    Loanamount =bank->repayLoan(amount,Loanamount);
}
void Economy::handleCorruption()
{
    if (corruption > 60)
    {
        cout << "Kingdom has gone corrupted" << endl;return;
    }
    cout << "Sab set hai" << endl;
}
void Economy::simulateDailyUpdate() {
    cout << "----- Simulating Daily Economy Update -----" << endl;

    double totalTaxCollected = 0.0;
    for (int i = 0; i < 4; ++i) 
    {
        if (populationSystem[i])
        {
            populationSystem[i]->taxgeneration();
            double taxRate = 0.0;
            int choice = 0;
            if (i == 0) 
            { // Peasants
                taxRate = taxSystem->getpeasanttax();
                choice = 1;
            }
            else if (i == 1) 
            { // Merchants
                taxRate = taxSystem->getmerchanttax();
                choice = 2;
            }
            else if (i == 2) 
            { // Nobility
                taxRate = taxSystem->getnobilityTax();
                choice = 3;
            }
            else 
            { // Military
                continue; 
            }
            double tax = populationSystem[i]->getadults() * taxRate * taxSystem->gettaxefficiency();
            collectTaxes(static_cast<int>(tax), choice);
            totalTaxCollected += tax;
        }
    }
    cout << "Total taxes collected: " << totalTaxCollected << " gold" << endl;


    double happinessAvg = 0.0;
    int popCount = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (populationSystem[i])
        {
            happinessAvg += populationSystem[i]->gethappymeter();
            ++popCount;
        }
    }
    happinessAvg = popCount > 0 ? happinessAvg / popCount : 50.0;

    double resourceFactor = 0.0;
    if (resourceSystem) 
    {
        resourceFactor = resourceSystem->canSustainPopulation() ? -0.01 : 0.02;
    }

    inflation += (treasury > 10000 ? 0.03 : -0.01) + (corruption > 50 ? 0.02 : 0.0) +
        (happinessAvg > 70 ? -0.01 : 0.01) + resourceFactor;
    inflation = max(0.0, min(100.0, inflation));
    cout << "Inflation updated to: " << inflation << "%" << endl;

    economicGrowth += (totalTaxCollected > 1000 ? 0.4 : -0.2) - (inflation > 20 ? 0.3 : 0.0) +
        (happinessAvg > 70 ? 0.2 : 0.0) + (resourceSystem && resourceSystem->canSustainPopulation() ? 0.2 : -0.2);
    economicGrowth = max(-100.0, min(100.0, economicGrowth));
    cout << "Economic growth updated to: " << economicGrowth << "%" << endl;


    if (bank) 
    {
        bank->adjustInterestRate(economicGrowth);
    }

    if (rand() % 100 < 10) 
    {
        if (rand() % 2 == 0) 
        { // Market boom
            treasury += 1000;
            economicGrowth += 2.0;
            cout << "Market boom! Treasury increased by 1000 gold." << endl;
        }
        else 
        { // Market crash
            treasury -= 500;
            economicGrowth -= 2.0;
            cout << "Market crash! Treasury decreased by 500 gold." << endl;
        }
        treasury = max(0.0, treasury);
    }

    if (treasury <= 0) 
    {
        cout << "Treasury depleted! Economic crisis imminent." << endl;
        corruption += 10;
        for (int i = 0; i < 4; ++i)
        {
            if (populationSystem[i]) 
            {
                populationSystem[i]->setrebelmeter(populationSystem[i]->getrebelmeter() + 5);
            }
        }
        treasury = 0;
    }

    displayEconomyStatus();
    cout << "----- Daily Economy Update Complete -----" << endl;
}