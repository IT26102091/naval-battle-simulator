#ifndef FILEIO_H
#define FILEIO_H

#include "simulator.h"


void ensureOutputDirectory(void);


void saveInitialConditions(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize,
    unsigned int seed
);


void savePart2Step(
    int simulationNumber,
    int step,
    double currentTime,
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double cumulativeDamage,
    int battleshipSunk,
    int gunJammed,
    double minimumAngle
);


void saveAttackOrder(
    int simulationNumber,
    int attackOrder[],
    double attackTimes[],
    int attackCount
);


void savePart2Comparison(
    int sim1Sunk,
    double sim1Damage,
    int sim1Destroyed,
    int sim1Steps,

    int sim2Sunk,
    double sim2Damage,
    int sim2Destroyed,
    int sim2Steps,

    int jamIteration,
    double minimumAngle
);

#endif
