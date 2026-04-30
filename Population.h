#pragma once
using namespace std;
#include "GameEngine.h"
#include "Population.h"
#include "Resource.h"
#include "Army.h"
#include "Economy.h"
#include "King.h"
#include <iostream>
#include"Resource.h"
class Economy;
class Resources;
class Politics;

enum Disastertype{Famine , Disease , Flood , Earthquake};
string typetostringdisaster(Disastertype a);
Disastertype stringtotypedisaster(const string a);
class population
{
protected:
	int totalpopulation;
	int adults;
	int men;
	int women;
	int children;
	int elders;
	int happymeter;
	int rebelmeter;
	bool rebel=false;
	Politics* politicalSystem;
	Economy* economysystem;
	Resources* resourecessystem;

public:
	//constructor
	population(int t,int a, int m, int w, int c, int e ,int h , int s);
	//virutal destructor
	virtual ~population();
	//Getters
	int getadults()const;
	int getwomen()const;
	int getchildren()const;
	int getman()const;
	int getelders()const;
	int gethappymeter()const;
	int gettotalpopulaiton()const;
	int getrebelmeter()const;

	//Setters	
	void setadults(int a);
	void setwomen(int a);
	void setchildren(int a);
	void setman(int a);
	void setelders(int a);
	void setrebelmeter(int a);
	void sethappymeter(int a);
	void settotalpopulaiton(int a);
	//linking economy and resources
	void seteconomy(Economy* a);
	void setresources(Resources* a);

	//methods
	virtual void displayPopulation()=0;
	virtual void Effectpopulation(int a, int b=0) = 0; 
	virtual void increasepopulation(int amount)=0;
	virtual void decreasepopulation(int amount) = 0;
	virtual void taxgeneration() = 0;
	virtual void resourcesintake() = 0;
	virtual bool checkrebelion() = 0;
	virtual void updatehealth() = 0;
	virtual void handleDisaster(const string a) = 0;
	virtual void simulatetimepassage() = 0;
};
class Peasants : public population
{
	double farmingefficiency;
	double taxBurden;

public:
	Peasants(double f , double ta,int t, int a, int m, int w, int c, int e, int h, int s);

	void displayPopulation()  override;
	void Effectpopulation(int a,int b=0) override;
	void increasepopulation(int amount) override;
	void decreasepopulation(int amount) override;
	void taxgeneration() override;
	void resourcesintake() override;
	bool checkrebelion() override;
	void updatehealth() override;
	void handleDisaster(const string a) override;
	void simulatetimepassage() override;

	// class specific 

	// Getters/Setters for class-specific attributes
	double getFarmingEfficiency() const;
	void setFarmingEfficiency(double efficiency);
	double getTaxBurden() const;
	void setTaxBurden(double burden);
	//--------------------//
	int producefood();
	void respondtotax();

};

class Merchants : public population
{
	double tradingefficinecy;
	double sales;
	double taxburden;
	double marketinfluence;
public:
	Merchants(int t , int a, int m, int w, int c, int e,int h,int s);
	~Merchants()override;
	double getsales();
	void setsales(double a);
	double getTradingEfficiency() const;
	void setTradingEfficiency(double efficiency);
	double gettaxburden() const;
	void settaxburden(double w);
	double getMarketInfluence() const;
	void setMarketInfluence(double influence);
	//---------------------------------------//
	void displayPopulation()  override;
	void Effectpopulation(int a , int b=0) override;
	void increasepopulation(int amount) override;
	void decreasepopulation(int amount) override;
	void taxgeneration() override;
	void resourcesintake() override;
	bool checkrebelion() override;
	void updatehealth() override;
	void handleDisaster(const string a) override;
	void simulatetimepassage() override;
	//class specific------------------------//
	void inflationeffects();
	void effectonsales();
	void generateTrade(TradeRoute t);
	void negotiateTaxes(double kingdomWealth);
};
class Nobility : public population
{
private:
	double wealth;
	double politicalInfluence;
	double landOwnership;
	int guards; // Personal guards of nobility

public:
	Nobility(int p , int l , int g,int t, int a, int m, int w, int c, int e, int h, int s);
	~Nobility()override;
	// Getters/Setters for class-specific attributes
	double getPoliticalInfluence() const;
	void setPoliticalInfluence(double influence);
	double getLandOwnership() const;
	void setLandOwnership(double land);
	int getGuards() const;
	void setGuards(int g);

	void displayPopulation()  override;
	void Effectpopulation(int a , int b=0) override;
	void increasepopulation(int amount) override;
	void decreasepopulation(int amount) override;
	void taxgeneration() override;
	void resourcesintake() override;
	bool checkrebelion() override;
	void updatehealth() override;
	void handleDisaster(const string a) override;
	void simulatetimepassage() override;

	int influenceKingElection(string candidateName, double supportLevel,int current_votes);
	void collectTaxesFromLand();
	bool attemptCoup(double kingUnpopularity) ;
};

class Military : public population
{
	double combatEfficiency;
	double morale;
	double training;
	int casualties;
	int deserters;
public:
	Military(double com , double Mor , double tra,int casual, int  desert ,int t, int a, int m, int w, int c, int e, int h, int s);
	~Military();
	// Getters/Setters for class-specific attributes
	double getCombatEfficiency() const;
	void setCombatEfficiency(double efficiency);
	double getMorale() const;
	void setMorale(double m);
	double getTraining() const;
	void setTraining(double t);
	int getCasualties() const;
	void setCasualties(int c);
	int getDeserters() const;
	void setDeserters(int d);

	//-----------//
	void displayPopulation()  override;
	void Effectpopulation(int a ,int b=0) override;
	void increasepopulation(int amount) override;
	void decreasepopulation(int amount) override;
	void taxgeneration() override;
	void resourcesintake() override;
	bool checkrebelion() override;
	void updatehealth() override;
	void handleDisaster(const string  a) override;
	void simulatetimepassage() override;
};
