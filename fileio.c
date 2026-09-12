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
            "output/part2_initial_conditions.txt",
            "w"
        );


    if (
        file == NULL
    )
    {
        printf( "Error creating initial conditions file.\n" );

        return;
    }


    fprintf(
        file,
        "PART 2 - INITIAL CONDITIONS\n"
    );

    

    fprintf(
        file,
        "\nCanvas Size: %.2f\n",
        canvasSize
    );


    fprintf(
        file,
        "Random Seed: %u\n",
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
        "Position: (%.2f, %.2f)\n",
        battleship.x,
        battleship.y
    );


    fprintf(
        file,
        "Max Velocity: %.2f\n",
        battleship.maxVelocity
    );


    fprintf(
        file,
        "Reload Time: %.2f seconds\n",
        battleship.reloadTime
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
            "\nEscort %d (%s)\n",
            escorts[i].id,
            escorts[i].notation
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
            escorts[i].impactPower * 100.0
        );


        fprintf(
            file,
            "Reload Time: %.2f seconds\n",
            escorts[i].reloadTime
        );
    }


    fclose(file);
}


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
)
{
    FILE *file;

    char filename[100];

    int i;


    snprintf(
        filename,
        sizeof(filename),
        "output/part2_sim%d_step_%02d.txt",
        simulationNumber,
        step
    );


    file =
        fopen(
            filename,
            "w"
        );


    if (
        file == NULL
    )
    {
        printf(
            "Error creating Part 2 output file.\n"
        );

        return;
    }


    fprintf(
        file,
        "PART 2 - SIMULATION %d\n",
        simulationNumber
    );


    
    fprintf(
        file,
        "\nIteration: %d\n",
        step
    );


    fprintf(
        file,
        "Simulation Time: %.2f seconds\n",
        currentTime
    );


    fprintf(
        file,
        "Battleship Position: (%.2f, %.2f)\n",
        battleship.x,
        battleship.y
    );


    if (
        simulationNumber == 2
    )
    {
        fprintf(
            file,
            "Gun Status: %s\n",
            gunJammed ?
                "JAMMED" :
                "NORMAL"
        );


        if (
            gunJammed
        )
        {
            fprintf(
                file,
                "Allowed Angle: %.2f - 90.00 degrees\n",
                minimumAngle
            );
        }
    }


    fprintf(
        file,
        "Battleship Status: %s\n",
        battleshipSunk ?
            "DESTROYED" :
            "ALIVE"
    );


    fprintf(
        file,
        "Cumulative Damage: %.2f%%\n",
        cumulativeDamage * 100.0
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
            "Escort %d (%s): %s | "
            "Next Fire: %.2f sec\n",

            escorts[i].id,
            escorts[i].notation,

            escorts[i].alive ?
                "ALIVE" :
                "DESTROYED",

            escorts[i].nextFireTime
        );
    }


    fclose(file);
}


void saveAttackOrder(
    int simulationNumber,
    int attackOrder[],
    double attackTimes[],
    int attackCount
)
{
    FILE *file;

    char filename[100];

    int i;


    snprintf(
        filename,
        sizeof(filename),
        "output/part2_sim%d_attack_order.txt",
        simulationNumber
    );


    file =
        fopen(
            filename,
            "w"
        );


    if (
        file == NULL
    )
    {
        printf(
            "Error creating attack order file.\n"
        );

        return;
    }


    fprintf(
        file,
        "BATTLESHIP ATTACK ORDER\n"
    );

    


    if (
        attackCount == 0
    )
    {
        fprintf(
            file,
            "Battleship did not destroy any escorts.\n"
        );
    }


    for (
        i = 0;
        i < attackCount;
        i++
    )
    {
        fprintf(
            file,
            "%d. Escort %d at %.2f seconds\n",
            i + 1,
            attackOrder[i],
            attackTimes[i]
        );
    }


    fclose(file);
}


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
)
{
    FILE *file;


    file =
        fopen(
            "output/part2_comparison.txt",
            "w"
        );


    if (
        file == NULL
    )
    {
        printf(
            "Error creating Part 2 comparison file.\n"
        );

        return;
    }


    fprintf(
        file,
        "PART 2 SIMULATION COMPARISON\n"
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
        "Damage: %.2f%%\n",
        sim1Damage * 100.0
    );


    fprintf(
        file,
        "Escorts Destroyed: %d\n",
        sim1Destroyed
    );


    fprintf(
        file,
        "Iterations: %d\n",
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
        "Jammed minimum angle: %.2f degrees\n",
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
        "Damage: %.2f%%\n",
        sim2Damage * 100.0
    );


    fprintf(
        file,
        "Escorts Destroyed: %d\n",
        sim2Destroyed
    );


    fprintf(
        file,
        "Iterations: %d\n",
        sim2Steps
    );


    fclose(file);
}
