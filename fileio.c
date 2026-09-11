#include <stdio.h>
#include <sys/stat.h>

#include "fileio.h"


void ensureOutputDirectory(void)
{
    mkdir("output", 0777);
}


void saveInitialConditions(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize,
    unsigned int seed
)
{
    FILE *file;
    int i;


    file = fopen(
        "output/initial_conditions.txt",
        "w"
    );


    if (file == NULL) {

        printf("Error creating initial conditions file.\n");
        return;
    }


    fprintf(file,
            "NAVAL BATTLE SIMULATOR - INITIAL CONDITIONS\n");


    fprintf(file,
            "\nCanvas size: %.2f x %.2f\n",
            canvasSize,
            canvasSize);

    fprintf(file,
            "Random seed: %u\n",
            seed);


    fprintf(file,
            "\nBATTLESHIP\n");

    fprintf(file,
            "Type: %c\n",
            battleship.notation);

    fprintf(file,
            "Name: %s\n",
            battleship.name);

    fprintf(file,
            "Gun: %s\n",
            battleship.gunName);

    fprintf(file,
            "Position: (%.2f, %.2f)\n",
            battleship.x,
            battleship.y);

    fprintf(file,
            "Maximum velocity: %.2f\n",
            battleship.maxVelocity);


    fprintf(file,
            "\nESCORT SHIPS\n");


    for (i = 0; i < numberOfEscorts; i++)
    {

        fprintf(file,
                "\nEscort ID: %d\n",
                escorts[i].id);

        fprintf(file,
                "Type: %s\n",
                escorts[i].notation);

        fprintf(file,
                "Name: %s\n",
                escorts[i].typeName);

        fprintf(file,
                "Gun: %s\n",
                escorts[i].gunName);

        fprintf(file,
                "Position: (%.2f, %.2f)\n",
                escorts[i].x,
                escorts[i].y);

        fprintf(file,
                "Impact Power: %.2f\n",
                escorts[i].impactPower);

        fprintf(file,
                "Angle Range: %.2f - %.2f\n",
                escorts[i].minAngle,
                escorts[i].maxAngle);

        fprintf(file,
                "Velocity Range: %.2f - %.2f\n",
                escorts[i].minVelocity,
                escorts[i].maxVelocity);
    }


    fclose(file);
}


void saveHitDetails(
    EscortShip escorts[],
    int hitIndices[],
    double hitTimes[],
    int hitCount
)
{
    FILE *file;
    int i;


    file = fopen(
        "output/hit_details.txt",
        "w"
    );


    if (file == NULL)
    {

        printf("Error creating hit details file.\n");
        return;
    }


    fprintf(file,
            "ESCORT SHIPS HIT BY BATTLESHIP\n");

   

    if (hitCount == 0) 
    {

        fprintf(file,
                "No escort ships were hit.\n");
    }


    for (i = 0; i < hitCount; i++) {

        int index = hitIndices[i];

        fprintf(file,
                "\nEscort ID: %d\n",
                escorts[index].id);

        fprintf(file,
                "Type: %s\n",
                escorts[index].notation);

        fprintf(file,
                "Time to hit: %.2f seconds\n",
                hitTimes[i]);
    }


    fclose(file);
}


void saveFinalConditions(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int battleshipSunk,
    int sinkerID,
    int hitCount,
    double battleTime
)
{
    FILE *file;
    int i;


    file = fopen(
        "output/final_conditions.txt",
        "w"
    );


    if (file == NULL) {

        printf("Error creating final conditions file.\n");
        return;
    }


    fprintf(file,
            "NAVAL BATTLE SIMULATOR - FINAL CONDITIONS\n");



    if (battleshipSunk) {

        fprintf(file,
                "\nBattleship status: DESTROYED\n");

        fprintf(file,
                "Destroyed by Escort ID: %d\n",
                sinkerID);
    }
    else {

        fprintf(file,
                "\nBattleship status: ALIVE\n");

        fprintf(file,
                "Escort ships destroyed: %d\n",
                hitCount);

        fprintf(file,
                "Battle duration: %.2f seconds\n",
                battleTime);
    }


    fprintf(file,
            "\nFINAL ESCORT STATUS\n");


    for (i = 0; i < numberOfEscorts; i++) {

        fprintf(file,
                "Escort %d (%s): %s\n",
                escorts[i].id,
                escorts[i].notation,
                escorts[i].alive
                    ? "ALIVE"
                    : "DESTROYED");
    }


    fclose(file);
}
