#include <stdio.h>
#include <sys/stat.h>

#include "fileio.h"


void ensureOutputDirectory(void)
{
    mkdir(
        "output",
        0777
    );
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


    file =
        fopen(
            "output/initial_conditions.txt",
            "w"
        );


    if (file == NULL)
    {
        printf(
            "Error creating initial conditions file.\n"
        );

        return;
    }


    fprintf(
        file,
        "NAVAL BATTLE SIMULATOR - INITIAL CONDITIONS\n"
    );

    fprintf(
        file,
        "\nCanvas size: %.2f x %.2f\n",
        canvasSize,
        canvasSize
    );


    fprintf(
        file,
        "Random seed: %u\n",
        seed
    );


    fprintf(
        file,
        "\nBATTLESHIP\n"
    );


    fprintf(
        file,
        "Type: %c\n",
        battleship.notation
    );


    fprintf(
        file,
        "Name: %s\n",
        battleship.name
    );


    fprintf(
        file,
        "Gun: %s\n",
        battleship.gunName
    );


    fprintf(
        file,
        "Initial Position: (%.2f, %.2f)\n",
        battleship.x,
        battleship.y
    );


    fprintf(
        file,
        "Maximum Velocity: %.2f\n",
        battleship.maxVelocity
    );


    fprintf(
        file,
        "\nESCORT SHIPS\n"
    );


    for (
        i = 0;
        i < numberOfEscorts;
        i++
    )
    {
        fprintf(
            file,
            "\nEscort ID: %d\n",
            escorts[i].id
        );


        fprintf(
            file,
            "Type: %s\n",
            escorts[i].notation
        );


        fprintf(
            file,
            "Name: %s\n",
            escorts[i].typeName
        );


        fprintf(
            file,
            "Gun: %s\n",
            escorts[i].gunName
        );


        fprintf(
            file,
            "Position: (%.2f, %.2f)\n",
            escorts[i].x,
            escorts[i].y
        );


        fprintf(
            file,
            "Impact Power: %.2f%%\n",
            escorts[i].impactPower *
            100.0
        );


        fprintf(
            file,
            "Angle Range: %.2f - %.2f\n",
            escorts[i].minAngle,
            escorts[i].maxAngle
        );


        fprintf(
            file,
            "Velocity Range: %.2f - %.2f\n",
            escorts[i].minVelocity,
            escorts[i].maxVelocity
        );
    }


    fclose(file);
}


void savePart1CStep(
    int simulationNumber,
    int step,
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double cumulativeDamage,
    int battleshipSunk,
    int gunJammed,
    double minimumAngle
)
{
    FILE *file;

    char filename[100];

    int i;


    snprintf(
        filename,
        sizeof(filename),
        "output/part1c_sim%d_step_%02d.txt",
        simulationNumber,
        step
    );


    file =
        fopen(
            filename,
            "w"
        );


    if (file == NULL)
    {
        printf(
            "Error creating Part 1-C step file.\n"
        );

        return;
    }


    fprintf(
        file,
        "PART 1-C - SIMULATION %d\n",
        simulationNumber
    );

    
    fprintf(
        file,
        "\nIteration: %d\n",
        step
    );


    fprintf(
        file,
        "Battleship position: (%.2f, %.2f)\n",
        battleship.x,
        battleship.y
    );


    if (simulationNumber == 2)
    {
        if (gunJammed)
        {
            fprintf(
                file,
                "Gun status: JAMMED\n"
            );

            fprintf(
                file,
                "Allowed angle range: %.2f - 90.00 degrees\n",
                minimumAngle
            );
        }

        else
        {
            fprintf(
                file,
                "Gun status: NORMAL\n"
            );

            fprintf(
                file,
                "Allowed angle range: 0.00 - 90.00 degrees\n"
            );
        }
    }


    fprintf(
        file,
        "\nBattleship status: %s\n",
        battleshipSunk ?
            "DESTROYED" :
            "ALIVE"
    );


    fprintf(
        file,
        "Cumulative damage: %.2f%%\n",
        cumulativeDamage *
        100.0
    );


    fprintf(
        file,
        "\nESCORT STATUS\n"
    );


    for (
        i = 0;
        i < numberOfEscorts;
        i++
    )
    {
        fprintf(
            file,
            "Escort %d (%s): %s | Fired: %s\n",
            escorts[i].id,
            escorts[i].notation,

            escorts[i].alive ?
                "ALIVE" :
                "DESTROYED",

            escorts[i].hasFired ?
                "YES" :
                "NO"
        );
    }


    fclose(file);
}


void savePart1CComparison(
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
)
{
    FILE *file;


    file =
        fopen(
            "output/part1c_comparison.txt",
            "w"
        );


    if (file == NULL)
    {
        printf(
            "Error creating Part 1-C comparison file.\n"
        );

        return;
    }


    fprintf(
        file,
        "PART 1-C SIMULATION COMPARISON\n"
    );

    
    fprintf(
        file,
        "\nSIMULATION 1\n"
    );


    fprintf(
        file,
        "Battleship: %s\n",
        sim1Sunk ?
            "DESTROYED" :
            "ALIVE"
    );


    fprintf(
        file,
        "Cumulative damage: %.2f%%\n",
        sim1Damage *
        100.0
    );


    fprintf(
        file,
        "Escorts destroyed: %d\n",
        sim1Destroyed
    );


    fprintf(
        file,
        "Iterations completed: %d\n",
        sim1Steps
    );


    fprintf(
        file,
        "\nSIMULATION 2\n"
    );


    fprintf(
        file,
        "Gun jams after iteration: %d\n",
        jamIteration
    );


    fprintf(
        file,
        "Minimum angle after jam: %.2f degrees\n",
        minimumAngle
    );


    fprintf(
        file,
        "Battleship: %s\n",
        sim2Sunk ?
            "DESTROYED" :
            "ALIVE"
    );


    fprintf(
        file,
        "Cumulative damage: %.2f%%\n",
        sim2Damage *
        100.0
    );


    fprintf(
        file,
        "Escorts destroyed: %d\n",
        sim2Destroyed
    );


    fprintf(
        file,
        "Iterations completed: %d\n",
        sim2Steps
    );


    fclose(file);
}
