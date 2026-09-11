#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "fileio.h"

#define MAX_PATH_POINTS 50


int main(void)
{
    Battleship battleship;

    EscortShip escorts[MAX_ESCORTS];

    double path[MAX_PATH_POINTS][2];

    int numberOfEscorts;
    int numberOfPoints;

    int i;
    int j;

    double canvasSize;

    unsigned int seed;

    int battleshipSunk = 0;
    int sinkerID = -1;

    int totalDestroyed = 0;

    printf(" ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("      PART 1-B SIMULATION 1\n");


    do {

        printf("\nEnter battlefield size D: ");
        scanf("%lf", &canvasSize);

    } while (canvasSize <= 0);


    do {

        printf(
            "Enter number of escort ships (1-%d): ",
            MAX_ESCORTS
        );

        scanf("%d", &numberOfEscorts);

    } while (
        numberOfEscorts < 1 ||
        numberOfEscorts > MAX_ESCORTS
    );


    do {

        printf(
            "Enter number of path points k (1-%d): ",
            MAX_PATH_POINTS
        );

        scanf("%d", &numberOfPoints);

    } while (
        numberOfPoints < 1 ||
        numberOfPoints > MAX_PATH_POINTS
    );


    printf("Enter random seed: ");
    scanf("%u", &seed);

    srand(seed);



    setupBattleship(
        &battleship,
        canvasSize
    );


    generateEscortShips(
        escorts,
        numberOfEscorts,
        canvasSize,
        battleship.maxVelocity
    );


        generateBattlePath(
        path,
        numberOfPoints,
        canvasSize
    );


   
    displayBattlefield(
        battleship,
        escorts,
        numberOfEscorts
    );


    displayBattlePath(
        path,
        numberOfPoints
    );


   
    ensureOutputDirectory();


    saveInitialConditions(
        battleship,
        escorts,
        numberOfEscorts,
        canvasSize,
        seed
    );


   
    printf(" STARTING PART 1-B SIMULATION 1\n");
    

    
    for (i = 0; i < numberOfPoints; i++)
    {

        int destroyedThisStep = 0;


        
        battleship.x = path[i][0];
        battleship.y = path[i][1];


        printf("\n------------------------------\n");

        printf(
            "Iteration %d\n",
            i + 1
        );

        printf(
            "Battleship moved to (%.2f, %.2f)\n",
            battleship.x,
            battleship.y
        );


            for (j = 0; j < numberOfEscorts; j++) 
		{

            if (!escorts[j].alive) {

                continue;
            }


            if (
                escortCanHitBattleship(
                    escorts[j],
                    battleship
                )
            ) {

                battleshipSunk = 1;

                sinkerID =
                    escorts[j].id;

                battleship.alive = 0;


                printf(
                    "Battleship destroyed by Escort %d!\n",
                    sinkerID
                );

                break;
            }
        }


       
            if (!battleshipSunk) {

            for (
                j = 0;
                j < numberOfEscorts;
                j++
            ) {

               
                if (!escorts[j].alive) {

                    continue;
                }


                if (
                    battleshipCanHit(
                        battleship,
                        escorts[j]
                    )
                ) {

                    escorts[j].alive = 0;

                    destroyedThisStep++;

                    totalDestroyed++;


                    printf(
                        "Battleship destroyed Escort %d (%s)\n",
                        escorts[j].id,
                        escorts[j].notation
                    );
                }
            }


            printf(
                "Destroyed in this iteration: %d\n",
                destroyedThisStep
            );
        }


       
        savePart1BStep(
            i + 1,
            battleship,
            escorts,
            numberOfEscorts,
            battleshipSunk,
            sinkerID,
            destroyedThisStep
        );


    

        if (battleshipSunk) {

            printf(
                "\nSimulation stopped at iteration %d.\n",
                i + 1
            );

            break;
        }
    }


   
    printf(" SIMULATION 1 FINAL RESULT\n");
   


    if (battleshipSunk) {

        printf("Battleship status: DESTROYED\n");

        printf(
            "Destroyed by Escort ID: %d\n",
            sinkerID
        );
    }

    else {

        printf("Battleship status: ALIVE\n");

        printf(
            "All %d path points completed.\n",
            numberOfPoints
        );
    }


    printf(
        "Total escort ships destroyed: %d\n",
        totalDestroyed
    );


    printf(
        "\nSimulation step files saved in output/ directory.\n"
    );


    return 0;
}
