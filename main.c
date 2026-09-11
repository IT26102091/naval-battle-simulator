#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "fileio.h"

#define MAX_PATH_POINTS 50


int main(void)
{
    Battleship originalBattleship;
    Battleship battleship1;
    Battleship battleship2;

    EscortShip originalEscorts[MAX_ESCORTS];
    EscortShip escorts1[MAX_ESCORTS];
    EscortShip escorts2[MAX_ESCORTS];

    double path[MAX_PATH_POINTS][2];

    double canvasSize;
    double jamMinimumAngle;

    unsigned int seed;

    int numberOfEscorts;
    int numberOfPoints;
    int jamIteration;

    int i;
    int j;

    int sim1Sunk = 0;
    int sim2Sunk = 0;

    int sim1Sinker = -1;
    int sim2Sinker = -1;

    int sim1Destroyed = 0;
    int sim2Destroyed = 0;

    int sim1Steps = 0;
    int sim2Steps = 0;

    printf(" ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("          PART 1-B\n");
    

    do {
        printf("\nEnter battlefield size D: ");
        scanf("%lf", &canvasSize);
    }
    while (canvasSize <= 0);


    do {
        printf(
            "Enter number of escort ships (1-%d): ",
            MAX_ESCORTS
        );

        scanf("%d", &numberOfEscorts);
    }
    while (
        numberOfEscorts < 1 ||
        numberOfEscorts > MAX_ESCORTS
    );


    
    do {
        printf(
            "Enter number of path points k (2-%d): ",
            MAX_PATH_POINTS
        );

        scanf("%d", &numberOfPoints);
    }
    while (
        numberOfPoints < 2 ||
        numberOfPoints > MAX_PATH_POINTS
    );


    do {
        printf(
            "Enter gun jam iteration t (1-%d): ",
            numberOfPoints - 1
        );

        scanf("%d", &jamIteration);
    }
    while (
        jamIteration < 1 ||
        jamIteration >= numberOfPoints
    );


    do {
        printf(
            "Enter minimum angle after gun jam (0-30): "
        );

        scanf("%lf", &jamMinimumAngle);
    }
    while (
        jamMinimumAngle <= 0.0 ||
        jamMinimumAngle >= 30.0
    );


    printf("Enter random seed: ");
    scanf("%u", &seed);

    srand(seed);


   
    setupBattleship(
        &originalBattleship,
        canvasSize
    );


    generateEscortShips(
        originalEscorts,
        numberOfEscorts,
        canvasSize,
        originalBattleship.maxVelocity
    );


    generateBattlePath(
        path,
        numberOfPoints,
        canvasSize
    );


    ensureOutputDirectory();


    saveInitialConditions(
        originalBattleship,
        originalEscorts,
        numberOfEscorts,
        canvasSize,
        seed
    );


    displayBattlefield(
        originalBattleship,
        originalEscorts,
        numberOfEscorts
    );


    displayBattlePath(
        path,
        numberOfPoints
    );


    
    battleship1 = originalBattleship;
    battleship2 = originalBattleship;


    for (i = 0; i < numberOfEscorts; i++) {
        escorts1[i] = originalEscorts[i];
        escorts2[i] = originalEscorts[i];
    }


        printf(" PART 1-B - SIMULATION 1\n");
   
    for (i = 0; i < numberOfPoints; i++)
    {

        int destroyedThisStep = 0;

        sim1Steps = i + 1;

        battleship1.x = path[i][0];
        battleship1.y = path[i][1];


        printf(
            "\nIteration %d - Position (%.2f, %.2f)\n",
            i + 1,
            battleship1.x,
            battleship1.y
        );


        
        for (j = 0; j < numberOfEscorts; j++) {

            if (!escorts1[j].alive) {
                continue;
            }

            if (
                escortCanHitBattleship(
                    escorts1[j],
                    battleship1
                )
            ) {
                sim1Sunk = 1;
                sim1Sinker = escorts1[j].id;
                battleship1.alive = 0;

                printf(
                    "Battleship destroyed by Escort %d\n",
                    sim1Sinker
                );

                break;
            }
        }


        
        if (!sim1Sunk) {

            for (j = 0; j < numberOfEscorts; j++) {

                if (!escorts1[j].alive) {
                    continue;
                }

                if (
                    battleshipCanHit(
                        battleship1,
                        escorts1[j]
                    )
                ) {
                    escorts1[j].alive = 0;

                    destroyedThisStep++;
                    sim1Destroyed++;

                    printf(
                        "Destroyed Escort %d (%s)\n",
                        escorts1[j].id,
                        escorts1[j].notation
                    );
                }
            }
        }


        savePart1BStep(
            i + 1,
            battleship1,
            escorts1,
            numberOfEscorts,
            sim1Sunk,
            sim1Sinker,
            destroyedThisStep
        );


        if (sim1Sunk) {
            break;
        }
    }


   
    
    printf(" PART 1-B - SIMULATION 2\n");
    
    printf(
        "Gun will jam after iteration %d.\n",
        jamIteration
    );

    printf(
        "Minimum angle after jam: %.2f degrees\n",
        jamMinimumAngle
    );


    for (i = 0; i < numberOfPoints; i++) {

        int destroyedThisStep = 0;
        int gunJammed;

        sim2Steps = i + 1;

        battleship2.x = path[i][0];
        battleship2.y = path[i][1];


        
        gunJammed =
            ((i + 1) > jamIteration);


        printf(
            "\nIteration %d - Position (%.2f, %.2f)\n",
            i + 1,
            battleship2.x,
            battleship2.y
        );


        if (gunJammed) {
            printf(
                "Gun status: JAMMED (%.2f - 90 degrees)\n",
                jamMinimumAngle
            );
        }
        else {
            printf("Gun status: NORMAL\n");
        }


        
        for (j = 0; j < numberOfEscorts; j++) {

            if (!escorts2[j].alive) {
                continue;
            }

            if (
                escortCanHitBattleship(
                    escorts2[j],
                    battleship2
                )
            ) {
                sim2Sunk = 1;
                sim2Sinker = escorts2[j].id;
                battleship2.alive = 0;

                printf(
                    "Battleship destroyed by Escort %d\n",
                    sim2Sinker
                );

                break;
            }
        }


        
        if (!sim2Sunk) {

            for (j = 0; j < numberOfEscorts; j++) {

                int canHit;

                if (!escorts2[j].alive) {
                    continue;
                }


                if (gunJammed) {

                    canHit =
                        battleshipCanHitJammed(
                            battleship2,
                            escorts2[j],
                            jamMinimumAngle
                        );
                }

                else {

                    canHit =
                        battleshipCanHit(
                            battleship2,
                            escorts2[j]
                        );
                }


                if (canHit) {

                    escorts2[j].alive = 0;

                    destroyedThisStep++;
                    sim2Destroyed++;

                    printf(
                        "Destroyed Escort %d (%s)\n",
                        escorts2[j].id,
                        escorts2[j].notation
                    );
                }
            }
        }


        savePart1BSimulation2Step(
            i + 1,
            battleship2,
            escorts2,
            numberOfEscorts,
            sim2Sunk,
            sim2Sinker,
            destroyedThisStep,
            gunJammed,
            jamMinimumAngle
        );


        if (sim2Sunk) {
            break;
        }
    }


   
    printf(" PART 1-B COMPARISON\n");
   

    printf("\nSimulation 1\n");

    printf(
        "Battleship: %s\n",
        sim1Sunk ? "DESTROYED" : "ALIVE"
    );

    printf(
        "Escorts destroyed: %d\n",
        sim1Destroyed
    );


    printf("\nSimulation 2\n");

    printf(
        "Battleship: %s\n",
        sim2Sunk ? "DESTROYED" : "ALIVE"
    );

    printf(
        "Escorts destroyed: %d\n",
        sim2Destroyed
    );


    savePart1BComparison(
        sim1Sunk,
        sim1Destroyed,
        sim1Steps,
        sim2Sunk,
        sim2Destroyed,
        sim2Steps,
        jamIteration,
        jamMinimumAngle
    );


    printf(
        "\nComparison saved to "
        "output/part1b_comparison.txt\n"
    );


    return 0;
}
