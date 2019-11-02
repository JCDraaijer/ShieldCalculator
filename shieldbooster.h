#ifndef SHIELDCALCULATOR_SHIELDBOOSTER_H
#define SHIELDCALCULATOR_SHIELDBOOSTER_H

#include "list.h"

struct ShieldBooster {
    int id;
    char *effect;
    char *experimental;
    double shieldStrenghtBonus;
    double explosionResistanceBonus;
    double kineticResistanceBonus;
    double thermalResistanceBonus;
};

struct ShieldBooster *shieldBoosterFromString(char *);

struct List *generateLoadOuts(struct List *shieldBoosterList, int max);

void printBoosterList(struct List *boosterList);

void printBooster(struct ShieldBooster *);

int boosterEquals(struct ShieldBooster, struct ShieldBooster);

void freeBooster(struct ShieldBooster *);


#endif //SHIELDCALCULATOR_SHIELDBOOSTER_H
