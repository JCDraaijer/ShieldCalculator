#include "test.h"
#include <stdlib.h>
#include <stdio.h>

struct LoadOutStats *calculateLoadOutStats(struct ShieldGenerator *generator, struct List *boosters) {
    double exp = 1;
    double kin = 1;
    double therm = 1;
    double hpb = 0;

    for (int i = 0; i < boosters->length; i++) {
        struct ShieldBooster *booster = get(boosters, i);
        exp *= (1 - booster->explosionResistanceBonus);
        kin *= (1 - booster->kineticResistanceBonus);
        therm *= (1 - booster->thermalResistanceBonus);
        hpb += booster->shieldStrenghtBonus;
    }

    if (exp < 0.7) {
        exp = 0.7 - (0.7 * exp) / 2;
    }

    if (kin < 0.7) {
        kin = 0.7 - (0.7 * exp) / 2;
    }

    if (therm < 0.7) {
        therm = 0.7 - (0.7 * exp) / 2;
    }

    double expRes = 1 - ((1 - generator->explosionResistance) * exp);
    double kinRes = 1 - ((1 - generator->kineticResistance) * kin);
    double thermRes = 1 - ((1 - generator->thermalResistance) * therm);

    double hp = (1 + hpb) * generator->shieldStrength;

    struct LoadOutStats *stats = malloc(sizeof(struct LoadOutStats));

    stats->generator = generator;
    stats->boosters = boosters;
    stats->hitPoints = hp;
    stats->regenRate = generator->regenRate;
    stats->explosiveResistance = expRes;
    stats->kineticResistance = kinRes;
    stats->thermalResistance = thermRes;

    return stats;
}

struct TestResult *
calculateTestResult(struct ShieldGenerator *generator, struct List *loadOut, struct TestConfig config) {
    struct TestResult *result = malloc(sizeof(struct TestResult));
    struct LoadOutStats *stats = calculateLoadOutStats(generator, loadOut);
    double actualDPS = config.damageEffectiveness *
                       (config.explosiveDPS * (1 - stats->explosiveResistance) +
                        config.kineticDPS * (1 - stats->kineticResistance) +
                        config.thermalDPS * (1 - stats->thermalResistance) +
                        config.absoluteDPS) -
                       stats->regenRate * (1 - config.damageEffectiveness);
    double survivalTime = stats->hitPoints / actualDPS;
    result->loadOutStats = stats;
    result->survivalTime = survivalTime;

    return result;
}

struct TestResult *findBestResultGen(struct List *generators, struct List *loadOut, struct TestConfig config) {
    struct TestResult *best = malloc(sizeof(struct TestResult));
    best->survivalTime = 0;
    for (int i = 0; i < generators->length; i++) {
        struct TestResult *result = calculateTestResult(get(generators, i), loadOut, config);
        printTestResult(result);
        if (best->survivalTime < result->survivalTime) {
            freeResult(best);
            best = result;
        } else {
            freeResult(result);
        }
    }

    return best;
}

struct TestResult *
findBestResultBooster(struct ShieldGenerator *generator, struct List *loadOutList, struct TestConfig config) {
    struct TestResult *best = malloc(sizeof(struct TestResult));
    best->survivalTime = 0;
    best->loadOutStats = NULL;
    for (int i = 0; i < loadOutList->length; i++) {
        struct TestResult *result = calculateTestResult(generator, get(loadOutList, i), config);
        if (best->survivalTime < result->survivalTime) {
            freeResult(best);
            best = result;
        } else {
            freeResult(result);
        }
    }

    return best;
}

struct TestResult *findBestResult(struct List *generators, struct List *boosterLists, struct TestConfig config) {
    struct TestResult *best = malloc(sizeof(struct TestResult));
    best->survivalTime = 0;
    best->loadOutStats = NULL;
    for (int g = 0; g < generators->length; g++) {
        struct ShieldGenerator *generator = get(generators, g);
        for (int b = 0; b < boosterLists->length; b++) {
            struct List *loadOut = get(boosterLists, b);
            struct TestResult *result = calculateTestResult(generator, loadOut, config);
            if (best->survivalTime < result->survivalTime) {
                freeResult(best);
                best = result;
            } else {
                freeResult(result);
            }
        }
    }
    return best;
}

void printTestResult(struct TestResult *result) {
    printf("Survival time: [%.02lf s]\n", result->survivalTime);
    printf("Shield generator: [%s] [%s] [%s]\n", result->loadOutStats->generator->type,
           result->loadOutStats->generator->engineering, result->loadOutStats->generator->experimental);
    printBoosterList(result->loadOutStats->boosters);
    printf("Shield hitpoints: [%.02lf]\n", result->loadOutStats->hitPoints);
    printf("Shield regen: [%.02lf hp/s]\n", result->loadOutStats->regenRate);
    printf("Explosion resistance: [%.02lf%%]\n", result->loadOutStats->explosiveResistance * 100);
    printf("  Kinetic Resistance: [%0.02lf%%]\n", result->loadOutStats->kineticResistance * 100);
    printf("  Thermal Resistance: [%0.02lf%%]\n", result->loadOutStats->thermalResistance * 100);
}

void printTestConfig(struct TestConfig *config) {
    printf("Test case:\n");
    printf("Explosive DPS: [%.02lf]\n", config->explosiveDPS);
    printf("Kinetic DPS: [%.02lf]\n", config->kineticDPS);
    printf("Thermal DPS: [%.02lf]\n", config->thermalDPS);
    printf("Absolute DPS: [%.02lf]\n", config->absoluteDPS);
    printf("Damage effectiveness: [%.01lf%%]\n", config->damageEffectiveness * 100);
}

void freeResult(struct TestResult *result) {
    free(result->loadOutStats);
    free(result);
}
