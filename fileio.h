#ifndef FILEIO_H
#define FILEIO_H

#include "simulator.h"


void ensureOutputDirectory(void);


void saveInitialConditions
(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize,
    unsigned int seed
);


void saveHitDetails
(
    EscortShip escorts[],
    int hitIndices[],
    double hitTimes[],
    int hitCount
);


void saveFinalConditions
(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int battleshipSunk,
    int sinkerID,
    int hitCount,
    double battleTime
);
void savePart1BStep(
    int step,
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int battleshipSunk,
    int sinkerID,
    int destroyedThisStep
);
void savePart1BSimulation2Step(
    int step,
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int battleshipSunk,
    int sinkerID,
    int destroyedThisStep,
    int gunJammed,
    double minimumAngle
);

void savePart1BComparison(
    int sim1Sunk,
    int sim1Destroyed,
    int sim1Steps,
    int sim2Sunk,
    int sim2Destroyed,
    int sim2Steps,
    int jamIteration,
    double minimumAngle
);



#endif
