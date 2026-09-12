#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "fileio.h"

#define MAX_PATH_POINTS 50


static void runAdvancedSimulation(
    int simulationNumber,

    Battleship *battleship,

    EscortShip escorts[],
    int numberOfEscorts,

    double path[][2],
    int numberOfPoints,

    int jamEnabled,
    int jamIteration,
    double jamMinimumAngle,

    double *finalDamage,
    int *battleshipSunk,
    int *destroyedCount,
    int *stepsCompleted,

    int attackOrder[],
    double attackTimes[],
    int *attackCount
)
{
    int i;
    int j;

    double damage = 0.0;


    *battleshipSunk = 0;

    *destroyedCount = 0;

    *stepsCompleted = 0;

    *attackCount = 0;


    for (
        i = 0;
        i < numberOfPoints;
        i++
    )
    {
        int gunJammed = 0;

        int targetIndex;

        double currentTime =
            (double)i;


        *stepsCompleted =
            i + 1;


        battleship->x =
            path[i][0];


        battleship->y =
            path[i][1];


        if (
            jamEnabled &&
            (i + 1) > jamIteration
        )
        {
            gunJammed = 1;
        }


        


        printf( "Simulation %d - Iteration %d\n",  simulationNumber, i + 1  );


        printf( "Time: %.2f seconds\n", currentTime );


        printf(
            "Position: (%.2f, %.2f)\n",
            battleship->x,
            battleship->y
        );


        if (
            gunJammed
        )
        {
            printf(
                "Gun: JAMMED (%.2f - 90 degrees)\n",
                jamMinimumAngle
            );
        }

        else
        {
            printf(
                "Gun: NORMAL\n"
            );
        }


        
        if (
            currentTime >=
            battleship->nextFireTime
        )
        {
            targetIndex =
                selectBestTarget(
                    *battleship,
                    escorts,
                    numberOfEscorts,
                    gunJammed,
                    jamMinimumAngle
                );


            if (
                targetIndex != -1
            )
            {
                printf(
                    "B attacks Escort %d (%s) "
                    "[Impact threat %.2f%%]\n",

                    escorts[targetIndex].id,
                    escorts[targetIndex].notation,

                    escorts[targetIndex].impactPower *
                    100.0
                );


                escorts[targetIndex].alive = 0;


                attackOrder[
                    *attackCount
                ] =
                    escorts[targetIndex].id;


                attackTimes[
                    *attackCount
                ] =
                    currentTime;


                (*attackCount)++;


                (*destroyedCount)++;


                battleship->nextFireTime =
                    currentTime +
                    battleship->reloadTime;
            }
        }


        
        for (
            j = 0;
            j < numberOfEscorts;
            j++
        )
        {
            if (
                !escorts[j].alive
            )
            {
                continue;
            }


            if (
                currentTime <
                escorts[j].nextFireTime
            )
            {
                continue;
            }


            if (
                escortCanHitBattleship(
                    escorts[j],
                    *battleship
                )
            )
            {
                damage +=
                    escorts[j].impactPower;


                printf(
                    "Escort %d (%s) fires. "
                    "Damage = %.2f%%\n",

                    escorts[j].id,
                    escorts[j].notation,

                    damage * 100.0
                );


                escorts[j].nextFireTime =
                    currentTime +
                    escorts[j].reloadTime;


                if (
                    damage >= 1.0
                )
                {
                    *battleshipSunk = 1;

                    battleship->alive = 0;


                    printf(
                        "Battleship DESTROYED "
                        "by cumulative damage!\n"
                    );


                    break;
                }
            }
        }


        savePart2Step(
            simulationNumber,
            i + 1,
            currentTime,
            *battleship,
            escorts,
            numberOfEscorts,
            damage,
            *battleshipSunk,
            gunJammed,
            jamMinimumAngle
        );


        if (
            *battleshipSunk
        )
        {
            break;
        }
    }


    *finalDamage =
        damage;


    saveAttackOrder(
        simulationNumber,
        attackOrder,
        attackTimes,
        *attackCount
    );
}


int main(void)
{
    Battleship originalBattleship;
    Battleship battleship1;
    Battleship battleship2;
  
    EscortShip escorts[MAX_ESCORTS];
    EscortShip escorts1[MAX_ESCORTS];
    EscortShip escorts2[MAX_ESCORTS];

    double path[MAX_PATH_POINTS][2];


    double canvasSize;

    double jamMinimumAngle;


    double sim1Damage;
    double sim2Damage;


    unsigned int seed;


    int numberOfEscorts;
    int numberOfPoints;

    int jamIteration;


    int sim1Sunk;
    int sim2Sunk;

    int sim1Destroyed;
    int sim2Destroyed;

    int sim1Steps;
    int sim2Steps;


    int sim1AttackOrder[MAX_ESCORTS];
    int sim2AttackOrder[MAX_ESCORTS];

    double sim1AttackTimes[MAX_ESCORTS];
    double sim2AttackTimes[MAX_ESCORTS];

    int sim1AttackCount;
    int sim2AttackCount;


    int i;


    
    printf( " ADVANCED NAVAL BATTLE SIMULATOR\n" );

    printf("        PART 2-A + PART 2-B\n" );

    

    do
    {
        printf(
            "\nEnter battlefield size D: "
        );

        scanf(
            "%lf",
            &canvasSize
        );

    }
    while (
        canvasSize <= 0
    );


    do
    {
        printf(
            "Enter number of escorts (1-%d): ",
            MAX_ESCORTS
        );

        scanf(
            "%d",
            &numberOfEscorts
        );

    }
    while (
        numberOfEscorts < 1 ||
        numberOfEscorts > MAX_ESCORTS
    );


    do
    {
        printf(
            "Enter number of path points k (2-%d): ",
            MAX_PATH_POINTS
        );

        scanf(
            "%d",
            &numberOfPoints
        );

    }
    while (
        numberOfPoints < 2 ||
        numberOfPoints > MAX_PATH_POINTS
    );


    do
    {
        printf(
            "Enter gun jam iteration t (1-%d): ",
            numberOfPoints - 1
        );

        scanf(
            "%d",
            &jamIteration
        );

    }
    while (
        jamIteration < 1 ||
        jamIteration >= numberOfPoints
    );


    do
    {
        printf(
            "Enter minimum jam angle (0-30): "
        );

        scanf(
            "%lf",
            &jamMinimumAngle
        );

    }
    while (
        jamMinimumAngle <= 0.0 ||
        jamMinimumAngle >= 30.0
    );


    printf(
        "Enter random seed: "
    );

    scanf(
        "%u",
        &seed
    );


    srand(seed);


    setupBattleship(
    &originalBattleship,
    canvasSize
);

    generateEscortShips(
    escorts,
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
    escorts,
    numberOfEscorts,
    canvasSize,
    seed

);

    displayBattlefield(
        originalBattleship,
        escorts,
        numberOfEscorts
    );


    displayBattlePath(
        path,
        numberOfPoints
    );


    
    battleship1 =
        originalBattleship;


    battleship2 =
        originalBattleship;


    for (
        i = 0;
        i < numberOfEscorts;
        i++
    )
    {
        escorts1[i] =
            escorts[i];


        escorts2[i] =
            escorts[i];
    }


    
    
    printf( "        PART 2 SIMULATION 1\n" );

    

    runAdvancedSimulation(
        1,

        &battleship1,

        escorts1,
        numberOfEscorts,

        path,
        numberOfPoints,

        0,
        jamIteration,
        jamMinimumAngle,

        &sim1Damage,
        &sim1Sunk,
        &sim1Destroyed,
        &sim1Steps,

        sim1AttackOrder,
        sim1AttackTimes,
        &sim1AttackCount
    );


        
    printf( "        PART 2 SIMULATION 2\n");

    

    runAdvancedSimulation(
        2,

        &battleship2,

        escorts2,
        numberOfEscorts,

        path,
        numberOfPoints,

        1,
        jamIteration,
        jamMinimumAngle,

        &sim2Damage,
        &sim2Sunk,
        &sim2Destroyed,
        &sim2Steps,

        sim2AttackOrder,
        sim2AttackTimes,
        &sim2AttackCount
    );


    
    printf("            RESULTS\n");

    
    printf("\nSimulation 1\n" );


    printf( "Battleship: %s\n", sim1Sunk ? "DESTROYED" : "ALIVE" );


    printf(
        "Damage: %.2f%%\n",
        sim1Damage * 100.0
    );


    printf(
        "Escorts destroyed: %d\n",
        sim1Destroyed
    );


    printf(
        "\nSimulation 2\n"
    );


    printf(
        "Battleship: %s\n",
        sim2Sunk ?
            "DESTROYED" :
            "ALIVE"
    );


    printf(
        "Damage: %.2f%%\n",
        sim2Damage * 100.0
    );


    printf(
        "Escorts destroyed: %d\n",
        sim2Destroyed
    );


    savePart2Comparison(
        sim1Sunk,
        sim1Damage,
        sim1Destroyed,
        sim1Steps,

        sim2Sunk,
        sim2Damage,
        sim2Destroyed,
        sim2Steps,

        jamIteration,
        jamMinimumAngle
    );


    printf(
        "\nPart 2 results saved in output/.\n"
    );


    return 0;
}
