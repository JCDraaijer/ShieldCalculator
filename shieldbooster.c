#include "shieldbooster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ShieldBooster *shieldBoosterFromString(char *value) {
    char *idStr = malloc(sizeof(char) * 64);
    char *effectStr = malloc(sizeof(char) * 64);
    char *experimentalStr = malloc(sizeof(char) * 64);
    char *shieldStrengthStr = malloc(sizeof(char) * 64);
    char *explosionStr = malloc(sizeof(char) * 64);
    char *kineticStr = malloc(sizeof(char) * 64);
    char *thermalStr = malloc(sizeof(char) * 64);

    sscanf(value, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^',']\n", idStr, effectStr, experimentalStr,
           shieldStrengthStr, explosionStr, kineticStr,
           thermalStr);

    int id = (int) strtol(idStr, NULL, 10);
    double shieldStrength = strtod(shieldStrengthStr, NULL);
    double explosion = strtod(explosionStr, NULL);
    double kinetic = strtod(kineticStr, NULL);
    double thermal = strtod(thermalStr, NULL);

    free(idStr);
    free(shieldStrengthStr);
    free(explosionStr);
    free(kineticStr);
    free(thermalStr);

    struct ShieldBooster *booster = malloc(sizeof(struct ShieldBooster));
    booster->id = id;
    booster->effect = effectStr;
    booster->experimental = experimentalStr;
    booster->shieldStrenghtBonus = shieldStrength;
    booster->explosionResistanceBonus = explosion;
    booster->kineticResistanceBonus = kinetic;
    booster->thermalResistanceBonus = thermal;
    return booster;
}

struct List *generateLoadOutList(struct List *availableList, int max, int added, int typesAdded) {
    struct List *loadOutList = newList(1);
    int toAdd = max - added;
    if (toAdd == 0) {
        if (max > 0) {
            struct List *someNewList = newList(max);
            add(loadOutList, someNewList);
        }
        return loadOutList;
    }
    for (int bstr = typesAdded; bstr < availableList->length; bstr++) {
        struct ShieldBooster *booster = get(availableList, bstr);

        for (int currentAdd = toAdd; currentAdd >= 1; currentAdd--) {
            struct List *otherList = generateLoadOutList(availableList, max, added + currentAdd, bstr);

            for (int i = 0; i < otherList->length; i++) {
                struct List *loadOut = get(otherList, i);

                if (!contains(loadOut, booster)) {
                    for (int x = 0; x < currentAdd; x++) {
                        add(loadOut, booster);
                    }
                    add(loadOutList, loadOut);
                }
            }
        }
    }
    return loadOutList;
}

struct List *generateLoadOuts(struct List *shieldBoosterList, int max) {
    struct List *list = newList(START_SIZE);
    for (int i = 0; i <= max; i++) {
        struct List *smallerList = generateLoadOutList(shieldBoosterList, i, 0, 0);
        for (int x = 0; x < smallerList->length; x++) {
            struct List *actualList = get(smallerList, x);
            add(list, actualList);
        }
        freeList(smallerList);
    }
    return list;
}

void printBoosterList(struct List *boosterList) {
    for (int i = 0; i < boosterList->length; i++) {
        struct ShieldBooster *booster = get(boosterList, i);
        printf("[%s] - [%s]\n", booster->effect, booster->experimental);
    }
}

void printBooster(struct ShieldBooster *booster) {
    printf("Booster id #%i\n", booster->id);
    printf("Effect: %s\n", booster->effect);
    printf("Experimental: %s\n", booster->experimental);
    printf("Shield strenght bonus: %lf\n", booster->shieldStrenghtBonus);
    printf("Explosion resistance bonus: %lf\n", booster->explosionResistanceBonus);
    printf("Kinetic resistance bonus: %lf\n", booster->kineticResistanceBonus);
    printf("Thermal resistance bonus: %lf\n", booster->thermalResistanceBonus);
}

void freeBooster(struct ShieldBooster *booster) {
    free(booster->experimental);
    free(booster->effect);
    free(booster);
}

