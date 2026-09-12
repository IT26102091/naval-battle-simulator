#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "simulator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


static double randomDouble(
    double min,
    double max
)
{
    return min +
           ((double)rand() / RAND_MAX) *
           (max - min);
}


static double degreesToRadians(
    double angle
)
{
    return angle * M_PI / 180.0;
}


static void setBattleshipType(
    Battleship *battleship
)
{
    switch (battleship->notation)
    {
        case 'U':

            strcpy(
                battleship->name,
                "USS Iowa (BB-61)"
            );

            strcpy(
                battleship->gunName,
                "50-caliber Mark 7 gun"
            );

            break;


        case 'M':

            strcpy(
                battleship->name,
                "MS King George V"
            );

            strcpy(
                battleship->gunName,
                "(356 mm) Mark VII gun"
            );

            break;


        case 'R':

            strcpy(
                battleship->name,
                "Richelieu"
            );

            strcpy(
                battleship->gunName,
                "(15 inch) Mle 1935 gun"
            );

            break;


        case 'S':

            strcpy(
                battleship->name,
                "Sovetsky Soyuz-class"
            );

            strcpy(
                battleship->gunName,
                "(16 inch) B-37 gun"
            );

            break;
    }
}


void setupBattleship(
    Battleship *battleship,
    double canvasSize
)
{
    char type;


    do
    {
        printf("\nBattleship Types\n");
        printf("U - USS Iowa\n");
        printf("M - MS King George V\n");
        printf("R - Richelieu\n");
        printf("S - Sovetsky Soyuz-class\n");

        printf("Choose battleship type: ");

        scanf(" %c", &type);

        type = toupper(type);

    }
    while (
        type != 'U' &&
        type != 'M' &&
        type != 'R' &&
        type != 'S'
    );


    battleship->notation = type;

    setBattleshipType(
        battleship
    );


    do
    {
        printf(
            "Enter battleship X position (0 - %.2f): ",
            canvasSize
        );

        scanf(
            "%lf",
            &battleship->x
        );

    }
    while (
        battleship->x < 0 ||
        battleship->x > canvasSize
    );


    do
    {
        printf(
            "Enter battleship Y position (0 - %.2f): ",
            canvasSize
        );

        scanf(
            "%lf",
            &battleship->y
        );

    }
    while (
        battleship->y < 0 ||
        battleship->y > canvasSize
    );


    do
    {
        printf(
            "Enter battleship maximum shell velocity: "
        );

        scanf(
            "%lf",
            &battleship->maxVelocity
        );

    }
    while (
        battleship->maxVelocity <= 0
    );


    
    do
    {
        printf(
            "Enter battleship reload interval (seconds): "
        );

        scanf(
            "%lf",
            &battleship->reloadTime
        );

    }
    while (
        battleship->reloadTime <= 0
    );


    battleship->nextFireTime = 0.0;
    battleship->alive = 1;
}
static void setEscortType(
    EscortShip *escort,
    int type,
    double battleshipMaxVelocity
)
{
    double angleRange;

    switch (type)
    {
        case 0:
            strcpy(escort->notation, "EA");
            strcpy(escort->typeName, "1936A-class Destroyer");
            strcpy(escort->gunName, "SK C/34 naval gun");

            escort->impactPower = 0.08;
            angleRange = 20.0;

            escort->maxVelocity =
                1.2 * battleshipMaxVelocity;

            escort->reloadTime = 2.0;
            break;

        case 1:
            strcpy(escort->notation, "EB");
            strcpy(escort->typeName, "Gabbiano-class Corvette");
            strcpy(escort->gunName, "L/47 dual-purpose gun");

            escort->impactPower = 0.06;
            angleRange = 30.0;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            escort->reloadTime = 3.0;
            break;

        case 2:
            strcpy(escort->notation, "EC");
            strcpy(escort->typeName, "Matsu-class Destroyer");
            strcpy(escort->gunName, "Type 89 dual-purpose gun");

            escort->impactPower = 0.07;
            angleRange = 25.0;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            escort->reloadTime = 4.0;
            break;

        case 3:
            strcpy(escort->notation, "ED");
            strcpy(escort->typeName, "F-class Escort Ship");
            strcpy(escort->gunName, "SK C/32 naval gun");

            escort->impactPower = 0.05;
            angleRange = 50.0;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            escort->reloadTime = 5.0;
            break;

        default:
            strcpy(escort->notation, "EE");
            strcpy(escort->typeName, "Japanese Kaibokan");
            strcpy(escort->gunName, "4.7 inch naval gun");

            escort->impactPower = 0.04;
            angleRange = 70.0;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            escort->reloadTime = 6.0;
            break;
    }

    escort->minAngle =
        randomDouble(5.0, 85.0 - angleRange);

    escort->maxAngle =
        escort->minAngle + angleRange;

    escort->minVelocity =
        randomDouble(
            0.30 * escort->maxVelocity,
            0.60 * escort->maxVelocity
        );
}

void generateEscortShips(
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize,
    double battleshipMaxVelocity
)
{
    int i;
    int type;

    for (i = 0; i < numberOfEscorts; i++)
    {
        escorts[i].id = i + 1;

        type = rand() % 5;

        setEscortType(
            &escorts[i],
            type,
            battleshipMaxVelocity
        );

        escorts[i].x =
            randomDouble(0.0, canvasSize);

        escorts[i].y =
            randomDouble(0.0, canvasSize);

        escorts[i].alive = 1;

        escorts[i].nextFireTime = 0.0;
    }
}


double calculateDistance(
    double x1,
    double y1,
    double x2,
    double y2
)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    return sqrt(
        dx * dx +
        dy * dy
    );
}


static double maximumAngleFactor(
    double minAngle,
    double maxAngle
)
{
    double first;
    double second;


    first =
        sin(
            2.0 *
            degreesToRadians(
                minAngle
            )
        );


    second =
        sin(
            2.0 *
            degreesToRadians(
                maxAngle
            )
        );


    if (
        minAngle <= 45.0 &&
        maxAngle >= 45.0
    )
    {
        return 1.0;
    }


    return
        first > second ?
        first : second;
}


static double minimumAngleFactor(
    double minAngle,
    double maxAngle
)
{
    double first;
    double second;


    first =
        sin(
            2.0 *
            degreesToRadians(
                minAngle
            )
        );


    second =
        sin(
            2.0 *
            degreesToRadians(
                maxAngle
            )
        );


    return
        first < second ?
        first : second;
}


int battleshipCanHit(
    Battleship battleship,
    EscortShip escort
)
{
    double distance;

    double maximumRange;


    distance =
        calculateDistance(
            battleship.x,
            battleship.y,
            escort.x,
            escort.y
        );


    maximumRange =
        (
            battleship.maxVelocity *
            battleship.maxVelocity
        ) /
        GRAVITY;


    return
        distance <= maximumRange;
}


int escortCanHitBattleship(
    EscortShip escort,
    Battleship battleship
)
{
    double distance;

    double minFactor;
    double maxFactor;

    double minimumRange;
    double maximumRange;


    distance =
        calculateDistance(
            escort.x,
            escort.y,
            battleship.x,
            battleship.y
        );


    minFactor =
        minimumAngleFactor(
            escort.minAngle,
            escort.maxAngle
        );


    maxFactor =
        maximumAngleFactor(
            escort.minAngle,
            escort.maxAngle
        );


    minimumRange =
        (
            escort.minVelocity *
            escort.minVelocity *
            minFactor
        ) /
        GRAVITY;


    maximumRange =
        (
            escort.maxVelocity *
            escort.maxVelocity *
            maxFactor
        ) /
        GRAVITY;


    return
        distance >= minimumRange &&
        distance <= maximumRange;
}


double calculateBattleshipFiringAngle(
    Battleship battleship,
    EscortShip escort
)
{
    double distance;

    double value;


    distance =
        calculateDistance(
            battleship.x,
            battleship.y,
            escort.x,
            escort.y
        );


    value =
        (
            distance *
            GRAVITY
        ) /
        (
            battleship.maxVelocity *
            battleship.maxVelocity
        );


    if (
        value < 0.0 ||
        value > 1.0
    )
    {
        return -1.0;
    }


    return
        0.5 *
        asin(value) *
        180.0 /
        M_PI;
}


int battleshipCanHitJammed(
    Battleship battleship,
    EscortShip escort,
    double minimumAngle
)

{
    double firingAngle;


    firingAngle =
        calculateBattleshipFiringAngle(
            battleship,
            escort
        );


    if (
        firingAngle < 0.0
    )
    {
        return 0;
    }


    return
        firingAngle >= minimumAngle;
}
int selectBestTarget(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int gunJammed,
    double minimumAngle
)
{
    int i;
    int target = -1;
    double highestImpact = -1.0;

    for (i = 0; i < numberOfEscorts; i++)
    {
        if (!escorts[i].alive)
        {
            continue;
        }

        if (gunJammed)
        {
            if (!battleshipCanHitJammed(
                    battleship,
                    escorts[i],
                    minimumAngle))
            {
                continue;
            }
        }
        else
        {
            if (!battleshipCanHit(
                    battleship,
                    escorts[i]))
            {
                continue;
            }
        }

        if (escorts[i].impactPower > highestImpact)
        {
            highestImpact = escorts[i].impactPower;
            target = i;
        }
    }

    return target;
}


void generateBattlePath(
    double path[][2],
    int numberOfPoints,
    double canvasSize
)
{
    int i;


    for (
        i = 0;
        i < numberOfPoints;
        i++
    )
    {
        path[i][0] =
            randomDouble(
                0,
                canvasSize
            );


        path[i][1] =
            randomDouble(
                0,
                canvasSize
            );
    }
}


void displayBattlePath(
    double path[][2],
    int numberOfPoints
)
{
    int i;

    printf("       BATTLESHIP PATH\n");

    for (i = 0; i < numberOfPoints; i++)
    {
        printf(
            "Point %d: (%.2f, %.2f)\n",
            i + 1,
            path[i][0],
            path[i][1]
        );
    }
}
void displayBattlefield(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts
)
{
    int i;


    

    printf( "       BATTLEFIELD DATA\n" );

    
    printf( "\nBattleship: %s\n", battleship.name );


    printf( "Position: (%.2f, %.2f)\n", battleship.x, battleship.y );


    printf( "Max Velocity: %.2f\n", battleship.maxVelocity );


    printf( "Reload Time: %.2f seconds\n", battleship.reloadTime);


    printf( "\nEscort Ships\n" );


    for (
        i = 0;
        i < numberOfEscorts;
        i++
    )
    {
        printf( "\nEscort %d (%s)\n", escorts[i].id, escorts[i].notation);


        printf( "Position: (%.2f, %.2f)\n", escorts[i].x, escorts[i].y);


        printf( "Impact Power: %.2f%%\n", escorts[i].impactPower * 100.0 );


        printf( "Reload Time: %.2f seconds\n", escorts[i].reloadTime );
    }
}
