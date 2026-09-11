#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "fileio.h"


int main(void)
{
    Battleship battleship;

    EscortShip escorts[MAX_ESCORTS];


    int numberOfEscorts;
    int i;

    double canvasSize;

    unsigned int seed;


    int battleshipSunk = 0;
    int sinkerID = -1;


    int hitIndices[MAX_ESCORTS];
    double hitTimes[MAX_ESCORTS];

    int hitCount = 0;

    double battleTime = 0.0;


   
    printf(" ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("          PART 1-A\n");
   


    do {

        printf("\nEnter battlefield size D: ");
        scanf("%lf", &canvasSize);

    } while (canvasSize <= 0);


    do {

        printf("Enter number of escort ships (1-%d): ",
               MAX_ESCORTS);

        scanf("%d", &numberOfEscorts);

    } while (numberOfEscorts < 1 ||
             numberOfEscorts > MAX_ESCORTS);


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


    displayBattlefield(
        battleship,
        escorts,
        numberOfEscorts
    );


    ensureOutputDirectory();


    saveInitialConditions(
        battleship,
        escorts,
        numberOfEscorts,
        canvasSize,
        seed
    );



    for (i = 0; i < numberOfEscorts; i++) {

        if (escortCanHitBattleship(
                escorts[i],
                battleship)) {

            battleshipSunk = 1;
            sinkerID = escorts[i].id;

            battleship.alive = 0;

            break;
        }
    }


    if (battleshipSunk) 
    {

        printf("BATTLE RESULT\n");
    
        printf("Battleship was destroyed!\n");

        printf("Escort ship %d destroyed the battleship.\n",
               sinkerID);


        saveFinalConditions(
            battleship,
            escorts,
            numberOfEscorts,
            1,
            sinkerID,
            0,
            0
        );
    }

    else {

        for (i = 0; i < numberOfEscorts; i++) 
	{

            if (battleshipCanHit(
                    battleship,
                    escorts[i])) {

                double time;


                time =
                    calculateBattleshipHitTime(
                        battleship,
                        escorts[i]
                    );


                hitIndices[hitCount] = i;

                hitTimes[hitCount] = time;


                if (time > battleTime)
	       	{

                    battleTime = time;
                }


                escorts[i].alive = 0;

                hitCount++;
            }
        }


        printf("BATTLE RESULT\n");

        printf("Battleship survived.\n");

        printf("Escort ships destroyed: %d\n",
               hitCount);

        printf("Battle duration: %.2f seconds\n",
               battleTime);


        saveHitDetails(
            escorts,
            hitIndices,
            hitTimes,
            hitCount
        );


        saveFinalConditions(
            battleship,
            escorts,
            numberOfEscorts,
            0,
            -1,
            hitCount,
            battleTime
        );
    }


    printf("\nSimulation files saved in output/ directory.\n");


    return 0;
}
