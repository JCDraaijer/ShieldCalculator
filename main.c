#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "shieldbooster.h"
#include "shieldgenerator.h"
#include "test.h"

const struct TestConfig config = {33, 33, 33, 33, 0.5};

int main(int argc, char **argv) {

    FILE *fdBooster;
    fdBooster = fopen(argv[1], "r");

    if (!fdBooster) {
        return -1;
    }

    struct List *specialList = newList(START_SIZE);

    struct List *boosterList = newList(START_SIZE);
    char *line = malloc(sizeof(char) * 1024);
    size_t size = 1024;
    while (getline(&line, &size, fdBooster) != -1) {
        struct ShieldBooster *booster = shieldBoosterFromString(line);
        add(boosterList, booster);
    }

    printf("Found %d booster variations.\n", boosterList->length);

    struct List *loadOutList = generateLoadOuts(boosterList, 2);
    printf("Total possible booster loadouts: %d\n", loadOutList->length);

    struct List *generatorList = newList(START_SIZE);

    FILE *fdGenerator;
    fdGenerator = fopen(argv[2], "r");
    if (!fdGenerator) {
        return -1;
    }

    while (getline(&line, &size, fdGenerator) != -1) {
        struct ShieldGenerator *generator = generatorFromString(line);
        add(generatorList, generator);
    }
    free(line);

    printf("Found %d generator variants.\n", generatorList->length);

    struct TestResult best = {NULL, 0.0};
    for (int i = 0; i < generatorList->length; i++) {
        struct TestResult *possibleBest = findBestResult(get(generatorList, i), loadOutList, config);
        if (possibleBest->survivalTime > best.survivalTime){
            best = *possibleBest;
        }
    }

    printTestResult(&best);

    freeList(boosterList);
    freeList(generatorList);

    for (int i = 0; i < loadOutList->length; i++) {
        freeList(get(loadOutList, i));
    }
    freeList(loadOutList);
    return 0;
}