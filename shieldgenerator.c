#include <stdlib.h>
#include <stdio.h>
#include "shieldgenerator.h"
#include "list.h"

struct ShieldGenerator *generatorFromString(char *value) {
    char *idStr = malloc(sizeof(char) * 64);
    char *typeStr = malloc(sizeof(char) * 64);
    char *engStr = malloc(sizeof(char) * 64);
    char *expStr = malloc(sizeof(char) * 64);
    char *shieldStr = malloc(sizeof(char) * 64);
    char *regenRateStr = malloc(sizeof(char) * 64);
    char *explosionStr = malloc(sizeof(char) * 64);
    char *kineticStr = malloc(sizeof(char) * 64);
    char *thermStr = malloc(sizeof(char) * 64);

    sscanf(value, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^',']\n", idStr, typeStr, engStr,
           expStr, shieldStr, regenRateStr,
           explosionStr, kineticStr, thermStr);

    int id = (int) strtol(idStr, NULL, 10);
    double shield = strtod(shieldStr, NULL);
    double regen = strtod(regenRateStr, NULL);
    double explosion = strtod(explosionStr, NULL);
    double kinetic = strtod(kineticStr, NULL);
    double thermal = strtod(thermStr, NULL);

    free(idStr);
    free(shieldStr);
    free(regenRateStr);
    free(explosionStr);
    free(kineticStr);
    free(thermStr);

    struct ShieldGenerator *generator = malloc(sizeof(struct ShieldGenerator));
    generator->id = id;
    generator->type = typeStr;
    generator->engineering = engStr;
    generator->experimental = expStr;
    generator->shieldStrength = shield;
    generator->regenRate = regen;
    generator->explosionResistance = explosion;
    generator->kineticResistance = kinetic;
    generator->thermalResistance = thermal;
    return generator;
}


void printGenerator(struct ShieldGenerator *generator) {
    printf("Generator id #%d\n", generator->id);
    printf("Type: %s\n", generator->type);
    printf("Engineering: %s\n", generator->engineering);
    printf("Experimental: %s\n", generator->experimental);
    printf("Shield Strength: %lf\n", generator->shieldStrength);
    printf("Regen rate: %lf\n", generator->regenRate);
    printf("Explosion resistance: %lf\n", generator->explosionResistance);
    printf("Kinetic resistance: %lf\n", generator->kineticResistance);
    printf("Thermal resistance: %lf\n", generator->thermalResistance);
}

struct List *readGeneratorList(char *file) {
    FILE *fdGenerator;
    fdGenerator = fopen(file, "r");
    if (!fdGenerator) {
        return NULL;
    }

    struct List *generatorList = newList(START_SIZE);
    char *line = malloc(sizeof(char) * 1024);
    size_t size = 1024;
    while (getline(&line, &size, fdGenerator) != -1) {
        struct ShieldGenerator *generator = generatorFromString(line);
        add(generatorList, generator);
    }
    free(line);
    return generatorList;
}

void freeGenerator(struct ShieldGenerator *generator) {
    free(generator->experimental);
    free(generator->engineering);
    free(generator->type);
    free(generator);
}

