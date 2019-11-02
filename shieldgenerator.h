#ifndef SHIELDCALCULATOR_SHIELDGENERATOR_H
#define SHIELDCALCULATOR_SHIELDGENERATOR_H

#include <string.h>

struct ShieldGenerator {
    int id;
    char *type;
    char *engineering;
    char *experimental;
    double shieldStrength;
    double regenRate;
    double explosionResistance;
    double kineticResistance;
    double thermalResistance;
};

struct ShieldGenerator *generatorFromString(char *);

void printGenerator(struct ShieldGenerator *);

void freeGenerator(struct ShieldGenerator *);

struct List *readGeneratorList(char *);

#endif //SHIELDCALCULATOR_SHIELDGENERATOR_H
