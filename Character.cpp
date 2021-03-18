#include "Character.h"

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
	this->name = _name;
	this->type = _type;
	this->attack = _attack;
	this->defense = _defense;
	this->remainingHealth = _remainingHealth;
	this->nMaxRounds = _nMaxRounds;
	this->healthHistory =new int[_nMaxRounds+1];
	healthHistory[0] = _remainingHealth;
	this->nRoundsSinceSpecial = 0;
}

Character::Character(const Character& character) {
	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nMaxRounds =	character.nMaxRounds;
	this->healthHistory =new int[character.nMaxRounds+1];

	for (int i = 0; i < this->nMaxRounds+1; i++)
		this->healthHistory[i] = character.healthHistory[i];

	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;

}

Character& Character::operator=(const Character& character) {
	if (this == &character){
		return *this;
	}

	if(this->healthHistory != NULL){
		delete[] this->healthHistory;
	}

	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nMaxRounds =	character.nMaxRounds;
	this->healthHistory =new int[character.nMaxRounds+1];

	for (int i = 0; i < this->nMaxRounds+1; i++)
		this->healthHistory[i] = character.healthHistory[i];

	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;

	return *this;
}

bool Character::operator<(const Character& other) {

	int x = other.name.compare(this->name);

	if(x > 0){
		return true;
	}else{
		return false;
	}
		
}

Character::~Character() {
	delete[] this->healthHistory;
}