#include "Character.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//finds the next alive character instead of a dead defender or attacker 
Character* findTheNextAliveCharacter(Character* character, Character** community){
	string name[5];

	for (int i = 0; i < 5; ++i)
		name[i] = community[i]->name;

	sort(begin(name),end(name));

	int myCharacter = 0;
	for (int i = 0; i < 5; ++i)
	{
		if((character->name).compare(name[i])==0)
			myCharacter = i;
	}

	//searches for a alphabetically next character
	for (int j = 1; myCharacter + j < 5; j++){
		for (int i = 0; i < 5; i++){
			if(community[i]->name.compare(name[myCharacter+j])==0 && community[i]->isAlive)
				return community[i];
		}
	}

	//searches for a alphabatically previous character
	for (int j = myCharacter-1; j >0; j--){
		for (int i = 0; i < 5; i++){
			if (community[i]->name.compare(name[j])==0 && community[i]->isAlive)
				return community[i];
		}
	}
}

//executes the rounds and changes the health of characters if necessary. If defined executes the specials of characters. Returns true if the hobbit is dead and war is over otherwise false
bool makeWar(int& _attacker, int& _defender, string special, Character** attackersCommunity,Character** defendersCommunity,int &attackersLastDead, int &defendersLastDead){
	Character* attacker = attackersCommunity[_attacker];
	Character* defender = defendersCommunity[_defender];
	if(!(attacker->isAlive)){
		attacker = findTheNextAliveCharacter(attacker,attackersCommunity);
	}
	if(!(defender->isAlive)){
		defender = findTheNextAliveCharacter(defender,defendersCommunity);
	}
	int damage = defender->defense-attacker->attack;
		
	if (damage < 0){
		defender->remainingHealth = defender->remainingHealth + damage;
	}else{
		damage = 0;
	}

	if(special.compare("SPECIAL") == 0){
		if(attacker->type.compare("Wizards")== 0){
			if(attackersLastDead != -1 && (attacker->nRoundsSinceSpecial)>50){
				attackersCommunity[attackersLastDead]->isAlive = true;
				attackersCommunity[attackersLastDead]->remainingHealth = attackersCommunity[attackersLastDead]->healthHistory[0];
				attacker->nRoundsSinceSpecial = 0;
				defender->nRoundsSinceSpecial = 0;
			}
		}else if(attacker->type.compare("Dwarfs") == 0){
			if (attacker->nRoundsSinceSpecial>20){
				defender->remainingHealth = defender->remainingHealth + damage;
				attacker->nRoundsSinceSpecial = 0;
			}
		}else if(attacker->type.compare("Elves") == 0){
			if(attacker->nRoundsSinceSpecial>10){
				for (int i = 0; i < 5; i++){
					if(attackersCommunity[i]->type.compare("Hobbit")==0){
						attackersCommunity[i]->remainingHealth =attackersCommunity[i]->remainingHealth +(attacker->remainingHealth/2);
						attacker->remainingHealth -= attacker->remainingHealth/2;
						attacker->nRoundsSinceSpecial = 0;
					}
				}
			}
		}
	}

	if(defender->remainingHealth<=0){
		defender->remainingHealth = 0;
		defender->isAlive = false;
		defendersLastDead = _defender;
		if(defender->type.compare("Hobbit")==0)
			return true;
	}
	return false;
}

int main(int argc, char* argv[]) {
	string inputFile = "input.txt";//argv[1];
	string outputfile = "output.txt";//argv[2];

	//input file
	ifstream infile;
	infile.open(inputFile);

	//output file
	ofstream outfile;
	outfile.open(outputfile);

	//maximum rounds of war
	int nMaxRounds;
	infile >> nMaxRounds;
		
	//two dimensional string array to store war data
	string war[3][nMaxRounds];

	//two pointer pointers for storing characters
	Character** communityOne = new Character*[5];
	Character** communityTwo = new Character*[5];
		
	//take the data of first community
	for (int i = 0; i < 5; i++)
	{
		string name,type;
		int attackPoint,defensePoint,initialHealth;
		infile >> name >> type >> attackPoint >> defensePoint >> initialHealth;
		communityOne[i] = new Character(name,type,attackPoint,defensePoint,initialHealth,nMaxRounds);
	}
		
	//take the data of second community 
	for (int i = 0; i < 5; i++)
	{
		string name,type;
		int attackPoint,defensePoint,initialHealth;
		infile >> name >> type >> attackPoint >> defensePoint >> initialHealth;
		communityTwo[i] = new Character(name,type,attackPoint,defensePoint,initialHealth,nMaxRounds);
	}

	//take the war information
	for (int i = 0; i < nMaxRounds; i++)
	{
		infile>>war[0][i]>>war[1][i]>>war[2][i];
	}
		
	string result = "Draw";
	int nOfRounds = nMaxRounds;
	int casualtiesCommOne = 0;
	int casualtiesCommTwo = 0;
	int lastDeadOfCommunityOne = -1;
	int lastDeadOfCommunityTwo = -1;
	for (int i = 0; i < nMaxRounds; i++)
	{
		casualtiesCommOne = 0;
		casualtiesCommTwo = 0;
		for (int j = 0; j < 5; j++)
		{
			if(!communityOne[j]->isAlive)
				casualtiesCommOne++;
			if(!communityTwo[j]->isAlive)
				casualtiesCommTwo++;
		}
		if(casualtiesCommOne>=4){
			result = "Community-2";
			nOfRounds = i;
			break;
		}
		if(casualtiesCommTwo>=4){
			result = "Community-1";
			nOfRounds = i;
			break;
		}
		//call war function
		bool isOver = false;
		bool turn = i%2;

		int one = 0;
		int two = 0;

		for (int j = 0; j < 5; j++)
		{
			if(turn){
				if(communityTwo[j]->name.compare(war[0][i])==0)
					two = j;
				if(communityOne[j]->name.compare(war[1][i])==0)
					one = j;
			}else{
				if(communityOne[j]->name.compare(war[0][i])==0)
					one = j;
				if(communityTwo[j]->name.compare(war[1][i])==0)
					two = j;
			}
		}

		isOver = (turn) ? makeWar(two,one,war[2][i],communityTwo,communityOne,lastDeadOfCommunityTwo, lastDeadOfCommunityOne)
							:makeWar(one,two,war[2][i],communityOne,communityTwo,lastDeadOfCommunityOne, lastDeadOfCommunityTwo);

		if(isOver){
			result = (turn) ? "Community-2" : "Community-1";
			nOfRounds = i+1;
			break;
		}

		for (int j = 0; j < 5; j++){
			communityOne[j]->nRoundsSinceSpecial++;
			communityOne[j]->healthHistory[i+1]=communityOne[j]->remainingHealth;
			communityTwo[j]->nRoundsSinceSpecial++;
			communityTwo[j]->healthHistory[i+1]=communityTwo[j]->remainingHealth;
		}
	}

	outfile << result << endl << nOfRounds << endl << casualtiesCommTwo+casualtiesCommOne << endl;

	for (int i = 0; i < 5; i++)
	{
		outfile << communityOne[i]->name << " ";
		for (int j = 0; j < nOfRounds+1; j++)
			outfile  <<communityOne[i]->healthHistory[j]<< " ";
		outfile<<endl;
	}
	for (int i = 0; i < 5; i++)
	{
		outfile << communityTwo[i]->name << " ";
		for (int j = 0; j < nOfRounds+1; j++)
			outfile << communityTwo[i]->healthHistory[j] << " ";
		outfile<<endl;
	}

	//closing and deleting everything
	for (int i = 0; i < 5; i++)
	{
		delete communityOne[i];
		delete communityTwo[i];
	}
		
	delete[] communityOne;
	delete[] communityTwo;

	infile.close();
	outfile.close();

	   return 0;
}