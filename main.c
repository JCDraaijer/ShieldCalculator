#include <stdio.h>
#include "list.h"
#include <time.h>
#include <getopt.h>
#include <stdlib.h>
#include "shieldbooster.h"
#include "shieldgenerator.h"
#include "test.h"

struct TestConfig config = {33, 33, 33, 33, 0.5};

int boosterCount = 2;

int main(int argc, char **argv) {
    int option;
    char *generatorFile = NULL;
    char *boosterFile = NULL;
    while ((option = getopt(argc, argv, "c:e:k:t:a:d:b:g:")) != -1) {
        switch (option) {
            case 'c':
                boosterCount = (int) strtol(optarg, NULL, 10);
                break;
            case 'e':
                config.explosiveDPS = strtod(optarg, NULL);
                break;
            case 'k':
                config.kineticDPS = strtod(optarg, NULL);
                break;
            case 't':
                config.thermalDPS = strtod(optarg, NULL);
                break;
            case 'a':
                config.absoluteDPS = strtod(optarg, NULL);
                break;
            case 'd':
                config.damageEffectiveness = strtod(optarg, NULL);
                break;
            case 'g':
                generatorFile = optarg;
                break;
            case 'b':
                boosterFile = optarg;
                break;
            default:
                break;
        }
    }

    printf("Using booster file %s\n", boosterFile);
    struct List *boosterList = readBoosterList(boosterFile);
    if (!boosterList) {
        printf("Could not find booster file!\n");
        return -1;
    }
    printf("Found %d booster variations.\n", boosterList->length);

    printf("Using generator file %s\n", generatorFile);
    struct List *generatorList = readGeneratorList(generatorFile);
    if (!generatorList) {
        printf("Could not find generator file!\n");
        return -1;
    }
    printf("Found %d generator variants.\n", generatorList->length);

    printTestConfig(&config);

    printf("Max boosters: %d\n", boosterCount);
    struct List *loadOutList = generateLoadOuts(boosterList, boosterCount);
    printf("Total possible loadouts: %d\n", loadOutList->length * generatorList->length);

    clock_t begin = clock();

    struct TestResult *best = findBestResult(generatorList, loadOutList, config);

    double timeSpent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    printTestResult(best);
    printf("%.0lf microseconds\n", timeSpent * 1000000);

    freeResult(best);

    for (int i = 0; i < loadOutList->length; i++) {
        freeList(get(loadOutList, i));
    }
    freeList(loadOutList);

    for (int i = 0; i < boosterList->length; i++) {
        freeBooster(get(boosterList, i));
    }
    freeList(boosterList);

    for (int i = 0; i < generatorList->length; i++) {
        freeGenerator(get(generatorList, i));
    }
    freeList(generatorList);
    return 0;
}
