#include "Population.h"
#include "Economy.h"
#include"Resource.h"
#include"King.h"
Disastertype stringtotypedisaster(const string a)
{
   
    if (a == "Famine")return Famine;
    else if (a == "Disease")return Disease;
    else if (a=="Flood") return Flood;
    else if(a== "Earthquake")return Earthquake;
}


string getdisaster(Disastertype a)
{
    switch (a)
    {
    case Famine:return"Famine";
    case Disease:return"Disease";
    case Flood: return"Flood";
    case Earthquake:return"Earthquake";
    }
}

population::population(int t, int a, int m, int w, int c, int e, int h, int s)
    : totalpopulation(t), adults(a), men(m), women(w), children(c), elders(e),
    happymeter(h), rebelmeter(s), economysystem(nullptr), resourecessystem(nullptr) 
{}

population::~population() {}

int population::getadults() const 
{
    return adults;
}

int population::getwomen() const 
{
    return women;
}

int population::getchildren() const 
{
    return children;
}

int population::getman() const 
{
    return men;
}

int population::getelders() const 
{
    return elders;
}

int population::gethappymeter() const 
{
    return happymeter;
}

int population::gettotalpopulaiton() const 
{
    return totalpopulation;
}

int population::getrebelmeter() const 
{
    return rebelmeter;
}

void population::setadults(int a) 
{
    adults = a;
}

void population::setwomen(int a) 
{
    women = a;
}

void population::setchildren(int a) 
{
    children = a;
}

void population::setman(int a) 
{
    men = a;
}

void population::setelders(int a) 
{

    elders = a;
}

void population::setrebelmeter(int a) 
{
    rebelmeter = a;
}

void population::sethappymeter(int a) 
{
    happymeter = a;
}

void population::settotalpopulaiton(int a) 
{
    totalpopulation = a;
}

void population::seteconomy(Economy* a)
{
    economysystem = a;
}

void population::setresources(Resources* a)
{
    resourecessystem = a;
}
//--------------------------------------------------------------------------//
Peasants::Peasants(double f, double ta, int t, int a, int m, int w, int c, int e, int h, int s):
population(t,a,m,w,c,e,h,s), farmingefficiency(f) , taxBurden(ta){}

double Peasants::getFarmingEfficiency()const
{
    return farmingefficiency;
}
double Peasants::getTaxBurden() const
{
    return taxBurden;
}
void Peasants::setFarmingEfficiency(double efficiency)
{
    farmingefficiency = efficiency;
}
void Peasants::setTaxBurden(double burden)
{
    taxBurden = burden;
}
void Peasants::displayPopulation()
{
    cout << "----Population of Farmers----" << endl;
    cout << "Man: " << men << "\n Women: " << women << "\n Children: " << children << "\n Elders: " << elders << endl;
    cout << "HappyMeter: " << happymeter << "\n RebelMeter: " << rebelmeter << endl;
}
void Peasants::increasepopulation(int amount)
{   
    men += (amount * 0.5);
    int m= (amount * 0.5);   
    women += (amount * 0.25);
    int w = (amount * 0.25);
    children += (amount * 0.10);
    elders += (amount * 0.10);
    m += w;
    adults += m;
}
void Peasants::decreasepopulation(int amount)
{
    men -= (amount * 0.5);
    int m = (amount * 0.5);
    women -= (amount * 0.25);
    int w = (amount * 0.25);
    children -= (amount * 0.10);
    elders -= (amount * 0.10);
    m += w;
    adults -= m;
}
void Peasants::Effectpopulation(int a,int b)
{
    if (b == 0)
    {
        increasepopulation(a);
        totalpopulation+= a;
    }
    else
    {
        decreasepopulation(a);
        totalpopulation -= a;
    }
}
void Peasants::resourcesintake()
{
    resourecessystem->consume(ResourceType::FOOD, totalpopulation/2);
    updatehealth();
    if (gethappymeter() % 99 == 0)
    {
        rebelmeter = 0;
        rebel = false;
    }
    cout << "resource Intake done Farmers" << endl;
}
bool Peasants::checkrebelion()
{
    if(happymeter<rebelmeter)
    if (rebelmeter %99==0)
    {
        
        rebel = true;
    }
    return rebel;
}
void Peasants::updatehealth()
{
    sethappymeter(gethappymeter() + 20);
    cout << "Health Update succesfully" << endl;
}
void Peasants::simulatetimepassage() 
{
    if (resourecessystem->canSustainPopulation()) 
    {
        sethappymeter(gethappymeter() + 2); 
    }
    else 
    {
        sethappymeter(gethappymeter() - 5); 
        setrebelmeter(getrebelmeter() + 3); 
    }
    if (taxBurden > 0.3) 
    {
        sethappymeter(gethappymeter() - 3);
        setrebelmeter(getrebelmeter() + 2);
    }

    int foodProduced = producefood();
    resourecessystem->produce(FOOD, foodProduced);

    if (gethappymeter() > 50 && resourecessystem->getResource(FOOD)->getQuantity() > 100)
    {
        increasepopulation(5); 
        totalpopulation += 5; 
    }

    
    resourcesintake(); 

    if (checkrebelion())
    {
        rebel = true;
        setrebelmeter(getrebelmeter() + 10); 
    }

    if (gethappymeter() >= 100) sethappymeter(99);
    if (gethappymeter() < 0) sethappymeter(0);
    if (getrebelmeter() >= 100) setrebelmeter(99);
    if (getrebelmeter() < 0) setrebelmeter(0);
}
int Peasants::producefood()
{
    int foodproduced = totalpopulation * farmingefficiency;
    resourecessystem->produce(ResourceType::FOOD,foodproduced);
    return 1;
}
void Peasants::respondtotax()
{
    economysystem->displayTaxStatus(1);
    if (economysystem->taxSystem->getpeasanttax() > 100)
    {
        rebelmeter += 50;
        if (checkrebelion())
        {
            cout << "Farmers are rebeling" << endl;
        }
        else
        {
            cout << "Chill mahol. Aao Huka piye" << endl;
        }
   }
}
void Peasants::taxgeneration()
{
    if (!economysystem || !economysystem->taxSystem) return;

    double taxRate = economysystem->taxSystem->getpeasanttax();  
    double taxCollected = adults * taxRate * (1.0 - taxBurden);  

    economysystem->collectTaxes( taxCollected, 0);

    if (taxBurden > 0.5) 
    {
        economysystem->setinfaltion(economysystem->getinflation() + 5);
        happymeter -= 10;
        rebelmeter += 10;
    }
    else 
    {
        happymeter += 5;
    }

    if (happymeter >= 100) happymeter = 99;
    if (happymeter < 0) happymeter = 0;

    if (rebelmeter >=100) rebelmeter = 99;
    if (rebelmeter < 0) rebelmeter = 0;


    cout << "Peasants paid taxes: " << taxCollected << ". Happy: " << happymeter << ", Rebel: " << rebelmeter << endl;
}

void Peasants::handleDisaster(const string a)
{
    int liveslost = 0;
    switch (stringtotypedisaster(a))
    {
    case Famine: liveslost = totalpopulation * 0.10;break;//Famine
    case Disease:liveslost = totalpopulation * 0.5;break;// Disease
    case Flood:liveslost = totalpopulation * 0.15;break;// Flood
    case Earthquake:liveslost = totalpopulation * 0.20;break;// EarthQuake
    }
    Effectpopulation(liveslost, 1);
    cout << "Peasants population decreased by " << a << " Lives lost are" << liveslost << endl;
}
//--------------------------------------------------//



Merchants::Merchants(int t,int a, int m, int w, int c, int e,int h,int s)
    : population(t, a, m, w, c, e, h, s),
    tradingefficinecy(0.0), sales(0.0), taxburden(0.0), marketinfluence(0.0) 
{}

Merchants::~Merchants() {}

void Merchants::setsales(double a)
{
    sales = a;
}
double Merchants::getsales()
{
    return sales;
}
double Merchants::getTradingEfficiency() const 
{
    return tradingefficinecy;
}

void Merchants::setTradingEfficiency(double efficiency) 
{
    tradingefficinecy = efficiency;
}

double Merchants::gettaxburden() const 
{
    return taxburden;
}

void Merchants::settaxburden(double w)
{
    taxburden = w;
}

double Merchants::getMarketInfluence() const
{
    return marketinfluence;
}

void Merchants::setMarketInfluence(double influence) 
{
    marketinfluence = influence;
}
void Merchants::displayPopulation()
{
    cout << "----Population of Merchants----" << endl;
    cout << "Man: " << men << "\n Women: " << women << "\n Children: " << children << "\n Elders: " << elders << endl;
    cout << "HappyMeter: " << happymeter << "\n RebelMeter: " << rebelmeter << endl;
}
void Merchants::increasepopulation(int amount)
{
    men += (amount * 0.7);
    int m = (amount * 0.7);
    women += (amount * 0.10);
    int w = (amount * 0.10);
    children += (amount * 0.02);
    elders += (amount * 0.20);
    m += w;
    adults += m;
}
void Merchants::decreasepopulation(int amount)
{
    men -= (amount * 0.7);
    int m = (amount * 0.7);
    women -= (amount * 0.10);
    int w = (amount * 0.10);
    children -= (amount * 0.02);
    elders -= (amount * 0.20);
    m += w;
    adults -= m;
}
void Merchants::Effectpopulation(int a, int b)
{
    if (b == 0)
    {
        increasepopulation(a);
        totalpopulation += a;
    }
    else
    {
        decreasepopulation(a);
        totalpopulation -= a;
    }
}
void Merchants::resourcesintake()
{
    resourecessystem->consume(ResourceType::FOOD, totalpopulation/2 );
    updatehealth();
    if (gethappymeter() % 99 == 0)
    {
        rebelmeter = 0;
        rebel = false;
    }
    cout << "resource Intake done Merchants" << endl;
}
void Merchants::taxgeneration()
{
    if (!economysystem || !economysystem->taxSystem) return;

    double taxRate = economysystem->taxSystem->getmerchanttax(); 
    double taxCollected = adults * taxRate * (1.0 - taxburden);  

    economysystem->collectTaxes( taxCollected, 1);

    if (taxburden > 0.5)\
    {
        happymeter -= 10;
        rebelmeter += 10;
    }
    else 
    {
        happymeter += 5;
    }

    if (happymeter > 100) happymeter = 100;
    if (happymeter < 0) happymeter = 0;

    if (rebelmeter > 100) rebelmeter = 100;
    if (rebelmeter < 0) rebelmeter = 0;


    cout << "Merchants paid taxes: " << taxCollected << ". Happy: " << happymeter << ", Rebel: " << rebelmeter << endl;
}
bool Merchants::checkrebelion()
{
        if (rebelmeter % 99 == 0)
        {

            rebel = true;
        }
    
    return rebel;
}
void Merchants::updatehealth()
{
    sethappymeter(gethappymeter() + 20);
    cout << "Health Update succesfully" << endl;
}
void Merchants::handleDisaster(const string a)
{
    int liveslost = 0;
    switch (stringtotypedisaster(a))
    {
    case Famine: liveslost = totalpopulation * 0.10;break;//Famine
    case Disease:liveslost = totalpopulation * 0.5;break;// Disease
    case Flood:liveslost = totalpopulation * 0.15;break;// Flood
    case Earthquake:liveslost = totalpopulation * 0.20;break;// EarthQuake
    }
    Effectpopulation(liveslost, 1);
    cout << "Merchants population decreased by " << a << " Lives lost are" << liveslost << endl;
}
void Merchants::simulatetimepassage() 
{
    if (economysystem->getinflation() < 0.1)
    {
        sethappymeter(gethappymeter() + 3);
    }
    else 
    { 
        sethappymeter(gethappymeter() - 4);
        setrebelmeter(getrebelmeter() + 2);
    }
    if (taxburden > 0.25) 
    {
        sethappymeter(gethappymeter() - 2);
        setrebelmeter(getrebelmeter() + 1);
    }

    if (resourecessystem->getResource(GOLD)->getQuantity() > 50) 
    {
        setsales(sales + 10.0 * tradingefficinecy); 
        economysystem->setTreasury(economysystem->getTreasury() + sales * 0.2);
    }

    if (resourecessystem->tradeRouteCount > 0)
    {
        for (int i = 0; i < resourecessystem->tradeRoutes[i]->isSecure();i++)
        {
            generateTrade(*resourecessystem->tradeRoutes[i]);
            break;
        }

    }

    if (gethappymeter() > 60 && sales > 100.0) 
    {
        increasepopulation(25);
        totalpopulation += 25;
    }
    resourcesintake();

    if (checkrebelion()) 
    {
        rebel = true;
        setrebelmeter(getrebelmeter() + 8);
    }

    if (gethappymeter() >= 100) sethappymeter(99);
    if (gethappymeter() < 0) sethappymeter(0);
    if (getrebelmeter() >= 100) setrebelmeter(99);
    if (getrebelmeter() < 0) setrebelmeter(0);
}
void Merchants::inflationeffects()
{
    cout << "Tax Burden Increasing due to Inflation on Merchants" << endl;
    settaxburden(gettaxburden() + 0.2);
    if (taxburden > 0.5) 
    {
        happymeter -= 10;
        rebelmeter += 10;
    }
    if (checkrebelion())
    {
        cout << "Merchants have gone for riots. gal wad gayi ye" << endl;
    }
}
void Merchants:: generateTrade(TradeRoute t)
{
    if (!t.isSecure())
    {
        cout << "not Secure trade root" << endl;
        return;
    }
    cout << "Merchants did a trade<<endl" << endl;
    cout << "Monthly Sales increased" << endl;
    setsales(getsales() + totalpopulation * 0.10);
}
void Merchants::negotiateTaxes(double kingdom)
{
    char choice;
    int nego;
    cout << "negotiation Tax Rate is:" << endl;
    cin >> nego;
    cout << "Do you Want to accept it?\n If yes press Y\n If no press N" << endl;
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        happymeter += 10;
        if (happymeter >= 100) happymeter = 99;
        rebelmeter -= 10;
        if (rebelmeter < 0) rebelmeter = 0;
        economysystem->taxSystem->setmerchanttax(economysystem->taxSystem->getmerchanttax() - 10);
    }
    else
    {
        happymeter -= 10;
        if (happymeter < 0) happymeter = 0;
        rebelmeter += 10;
        if (rebelmeter >=100) rebelmeter = 99;
        if (checkrebelion())
        {
            cout << "Merchants have gone for riots. gal wad gayi ye" << endl;
        }
    }    
}



//-------------------------------------------//
Nobility::Nobility(int p , int l , int g,int t, int a, int m, int w, int c, int e, int h, int s)
    : population(t, a, m, w, c, e, h, s),
    politicalInfluence(p),landOwnership(l), guards(g)
{
}
Nobility::~Nobility(){}
double Nobility::getPoliticalInfluence() const 
{
    return politicalInfluence;
}

double Nobility::getLandOwnership() const 
{
    return landOwnership;
}

int Nobility::getGuards() const 
{
    return guards;
}
void Nobility::setPoliticalInfluence(double influence) 
{
    politicalInfluence = influence;
}
void Nobility::setLandOwnership(double land) 
{
    landOwnership = land;
}
void Nobility::setGuards(int g) 
{
    guards = g;
}
void Nobility::displayPopulation()
{
    cout << "----Population of Nobility----" << endl;
    cout << "Man: " << men << "\n Women: " << women << "\n Children: " << children << "\n Elders: " << elders << endl;
    cout << "HappyMeter: " << happymeter << "\n RebelMeter: " << rebelmeter << endl;
}
void Nobility::increasepopulation(int amount)
{
    men += (amount * 0.5);
    int m = (amount * 0.5);
    women += (amount * 0.4);
    int w = (amount * 0.4);
    children += (amount * 0.05);
    elders += (amount * 0.05);
    m += w;
    adults += m;
}
void Nobility::decreasepopulation(int amount)
{
    men -= (amount * 0.5);
    int m = (amount * 0.5);
    women -= (amount * 0.4);
    int w = (amount * 0.4);
    children -= (amount * 0.05);
    elders -= (amount * 0.05);
    m += w;
    adults -= m;
}
void Nobility::Effectpopulation(int a, int b)
{
    if (b == 0)
    {
        increasepopulation(a);
        totalpopulation += a;
    }
    else
    {
        decreasepopulation(a);
        totalpopulation -= a;
    }
}
void Nobility::resourcesintake()
{
    resourecessystem->consume(ResourceType::FOOD, totalpopulation/2 );
    updatehealth();
    if (gethappymeter() % 99 == 0)
    {
        rebelmeter = 0;
        rebel = false;
    }
    cout << "resource Intake done Merchants" << endl;
}
void Nobility::taxgeneration()
{
    cout << "Nobels not gonna pay tax. Uchee loggg" << endl;
    collectTaxesFromLand();
}
bool Nobility::checkrebelion()
{
    
        if (rebelmeter % 99 == 0)
        {

            rebel = true;
        }
    
    return rebel;
}
void Nobility::updatehealth()
{
    sethappymeter(gethappymeter() + 20);
    cout << "Health Update succesfully" << endl;
}
void Nobility::handleDisaster(const string a)
{
    int liveslost = 0;
    switch (stringtotypedisaster(a))
    {
    case Famine: liveslost = totalpopulation * 0.10;break;//Famine
    case Disease:liveslost = totalpopulation * 0.5;break;// Disease
    case Flood:liveslost = totalpopulation * 0.15;break;// Flood
    case Earthquake:liveslost = totalpopulation * 0.20;break;// EarthQuake
    }
    Effectpopulation(liveslost, 1);
    cout << "Nobility population decreased by " << a << " Lives lost are" << liveslost << endl;
}
void Nobility::simulatetimepassage() 
{
   
    if (politicalInfluence > 50 && landOwnership > 100) 
    {
        sethappymeter(gethappymeter() + 4); 
    }
    else 
    {
        sethappymeter(gethappymeter() - 3); 
        setrebelmeter(getrebelmeter() + 2);
    }

    collectTaxesFromLand(); 
    if (politicalSystem && politicalSystem->currentKing && politicalSystem->currentKing->getPopularity() < 30.0) 
    {
        if (attemptCoup(politicalSystem->currentKing->getPopularity())) 
        {
            setrebelmeter(getrebelmeter() + 15); 
        }
    }

    if (resourecessystem->getResource(GOLD)->getQuantity() > 50) 
    {
        setGuards(getGuards() + 1); 
        resourecessystem->consume(GOLD, 10); 
    }

   
    if (gethappymeter() > 70 && landOwnership > 200) 
    {
        increasepopulation(1);
        totalpopulation += 1; 
    }

    resourcesintake();

    if (checkrebelion()) 
    {
        rebel = true;
        setrebelmeter(getrebelmeter() + 10);
    }

    if (gethappymeter() >= 100) sethappymeter(99);
    if (gethappymeter() < 0) sethappymeter(0);
    if (getrebelmeter() >= 100) setrebelmeter(99);
    if (getrebelmeter() < 0) setrebelmeter(0);
}
int Nobility::influenceKingElection(string candiateName, double supportlevel ,int current_votes)
{
    if (politicalInfluence > 50)
    {
        cout << "Bribery on the way for " << candiateName;
        current_votes += supportlevel;
    }
    return current_votes;
}
void Nobility::collectTaxesFromLand()
{
    int taxcollected = landOwnership * economysystem->taxSystem->getnobilityTax();
    int a = rand() % 3;
    switch (a)
    {
    case 0:a = (taxcollected * 0);taxcollected -= a;break;
    case 1:a = (taxcollected * 0.5);taxcollected -= a;break;
    case 2:a = (taxcollected * 0.8);taxcollected -= a;break;
    }
    economysystem->collectTaxes(taxcollected, 2);
}
bool Nobility:: attemptCoup(double kingUnpopularity)
{
    if (politicalInfluence == 100)
    {
        politicalSystem->currentKing->adjustPopularity(20);
        politicalSystem->stabilityIndex - 20;
        return true;
    }
    return false;
}






//---------------------------------//
Military::Military(double com, double Mor, double tra, int casual, int  desert,int t, int a, int m, int w, int c, int e,int h, int s)
    : population(t, a, m, w, c, e, h, s),  
    combatEfficiency(com), morale(Mor), training(tra), casualties(casual), deserters(desert)
{}
Military::~Military() {}
double Military::getCombatEfficiency() const 
{
    return combatEfficiency;
}

double Military::getMorale() const 
{
    return morale;
}

double Military::getTraining() const 
{
    return training;
}

int Military::getCasualties() const 
{
    return casualties;
}

int Military::getDeserters() const 
{
    return deserters;
}
void Military::setCombatEfficiency(double efficiency) 
{
    combatEfficiency = efficiency;
}

void Military::setMorale(double m) 
{
    morale = m;
}

void Military::setTraining(double t) 
{
    training = t;
}

void Military::setCasualties(int c) 
{
    casualties = c;
}

void Military::setDeserters(int d) 
{
    deserters = d;
}
void Military::displayPopulation()
{
    cout << "----Population of Military----" << endl;
    cout << "Man: " << men << "\n Women: " << women << "\n Children: " << children << "\n Elders: " << elders << endl;
    cout << "HappyMeter: " << happymeter << "\n RebelMeter: " << rebelmeter << endl;
}
void Military::increasepopulation(int amount)
{
    men += (amount * 0.7);
    int m = (amount * 0.7);
    women += (amount * 0.2);
    int w = (amount * 0.2);
    children += (amount * 0.05);
    elders += (amount * 0.05);
    m += w;
    adults += m;
}
void Military::decreasepopulation(int amount)
{
    men -= (amount * 0.7);
    int m = (amount * 0.7);
    women -= (amount * 0.2);
    int w = (amount * 0.2);
    children -= (amount * 0.05);
    elders -= (amount * 0.05);
    m += w;
    adults -= m;
}
void Military::Effectpopulation(int a, int b)
{
    if (b == 0)
    {
        increasepopulation(a);
        totalpopulation += a;
    }
    else
    {
        decreasepopulation(a);
        totalpopulation -= a;
    }
}
void Military::resourcesintake()
{
    resourecessystem->consume(ResourceType::FOOD, totalpopulation/2);
    updatehealth();
    if (gethappymeter() % 99 == 0)
    {
        rebelmeter = 0;
        rebel = false;
    }
    cout << "resource Intake done Military" << endl;
}
void Military::taxgeneration()
{
    cout << "Army not gonna pay tax. Uchee loggg" << endl;
}
bool Military::checkrebelion()
{
    
        if (rebelmeter % 99 == 0)
        {

            rebel = true;
        }
    
    return rebel;
}
void Military::updatehealth()
{
    sethappymeter(gethappymeter() + 20);
    cout << "Health Update succesfully" << endl;
}
void Military::handleDisaster(const string a)
{
    int liveslost = 0;
    switch (stringtotypedisaster(a))
    {
    case Famine: liveslost = totalpopulation * 0.10;break;//Famine
    case Disease:liveslost = totalpopulation * 0.5;break;// Disease
    case Flood:liveslost = totalpopulation * 0.15;break;// Flood
    case Earthquake:liveslost = totalpopulation * 0.20;break;// EarthQuake
    }
    Effectpopulation(liveslost, 1);
    cout << "Military population decreased by " << a << " Lives lost are" << liveslost << endl;
}
void Military::simulatetimepassage() 
{
    
    if (resourecessystem->canSustainArmy() && morale > 50.0) 
    {
        sethappymeter(gethappymeter() + 3); 
    }
    else 
    {
        sethappymeter(gethappymeter() - 5);
        setrebelmeter(getrebelmeter() + 3);
    }

    
    setTraining(getTraining() + 0.1); 
    if (resourecessystem->getResource(WEAPONS)->getQuantity() > 50) 
    {
        setCombatEfficiency(getCombatEfficiency() + 0.05); 
    }
    if (gethappymeter() < 30) 
    {
        setMorale(getMorale() - 5.0); 
    }

    if (getMorale() < 20.0) 
    {
        setDeserters(getDeserters() + 5);
        decreasepopulation(5);
        totalpopulation -= 5;
    }
     
    resourecessystem->consume(WEAPONS, gettotalpopulaiton() / 10); 

    resourcesintake(); 

    if (checkrebelion()) 
    {
        rebel = true;
        setrebelmeter(getrebelmeter() + 12);
    }

    if (gethappymeter() >= 100) sethappymeter(99);
    if (gethappymeter() < 0) sethappymeter(0);
    if (getrebelmeter() >= 100) setrebelmeter(99);
    if (getrebelmeter() < 0) setrebelmeter(0);
    if (getTraining() > 100.0) setTraining(100.0);
    if (getMorale() > 100.0) setMorale(100.0);
    if (getMorale() < 0.0) setMorale(0.0);
}
