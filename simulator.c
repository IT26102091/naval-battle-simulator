#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "simulator.h"


static double randomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


static void setBattleshipType(Battleship *battleship)
{
    switch (battleship->notation) {

        case 'U':
            strcpy(battleship->name, "USS Iowa (BB-61)");
            strcpy(battleship->gunName, "50-caliber Mark 7 gun");
            break;

        case 'M':
            strcpy(battleship->name, "MS King George V");
            strcpy(battleship->gunName, "(356 mm) Mark VII gun");
            break;

        case 'R':
            strcpy(battleship->name, "Richelieu");
            strcpy(battleship->gunName, "(15 inch) Mle 1935 gun");
            break;

        case 'S':
            strcpy(battleship->name, "Sovetsky Soyuz-class");
            strcpy(battleship->gunName, "(16 inch) B-37 gun");
            break;
    }
}


void setupBattleship(Battleship *battleship, double canvasSize)
{
    char type;

    do {
        printf("\nBattleship Types\n");
        printf("U - USS Iowa\n");
        printf("M - MS King George V\n");
        printf("R - Richelieu\n");
        printf("S - Sovetsky Soyuz-class\n");

        printf("Choose battleship type: ");
        scanf(" %c", &type);

        type = toupper(type);

    } while (type != 'U' &&
             type != 'M' &&
             type != 'R' &&
             type != 'S');


    battleship->notation = type;

    setBattleshipType(battleship);


    do {
        printf("Enter battleship X position (0 - %.2f): ",
               canvasSize);

        scanf("%lf", &battleship->x);

    } while (battleship->x < 0 ||
             battleship->x > canvasSize);


    do {
        printf("Enter battleship Y position (0 - %.2f): ",
               canvasSize);

        scanf("%lf", &battleship->y);

    } while (battleship->y < 0 ||
             battleship->y > canvasSize);


    do {
        printf("Enter battleship maximum shell velocity: ");
        scanf("%lf", &battleship->maxVelocity);

    } while (battleship->maxVelocity <= 0);


    battleship->alive = 1;
}


static void setEscortType(
    EscortShip *escort,
    int type,
    double battleshipMaxVelocity
)
{
    double angleRange;

    switch (type) {

        case 0:

            strcpy(escort->notation, "EA");
            strcpy(escort->typeName,
                   "1936A-class Destroyer");

            strcpy(escort->gunName,
                   "SK C/34 naval gun");

            escort->impactPower = 0.08;

            angleRange = 20;

            escort->maxVelocity =
                1.2 * battleshipMaxVelocity;

            break;


        case 1:

            strcpy(escort->notation, "EB");

            strcpy(escort->typeName,
                   "Gabbiano-class Corvette");

            strcpy(escort->gunName,
                   "L/47 dual-purpose gun");

            escort->impactPower = 0.06;

            angleRange = 30;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            break;


        case 2:

            strcpy(escort->notation, "EC");

            strcpy(escort->typeName,
                   "Matsu-class Destroyer");

            strcpy(escort->gunName,
                   "Type 89 dual-purpose gun");

            escort->impactPower = 0.07;

            angleRange = 25;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            break;


        case 3:

            strcpy(escort->notation, "ED");

            strcpy(escort->typeName,
                   "F-class Escort Ship");

            strcpy(escort->gunName,
                   "SK C/32 naval gun");

            escort->impactPower = 0.05;

            angleRange = 50;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

            break;


        default:

            strcpy(escort->notation, "EE");

            strcpy(escort->typeName,
                   "Japanese Kaibokan");

            strcpy(escort->gunName,
                   "(4.7 inch) naval gun");

            escort->impactPower = 0.04;

            angleRange = 70;

            escort->maxVelocity =
                randomDouble(
                    0.60 * battleshipMaxVelocity,
                    0.95 * battleshipMaxVelocity
                );

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

    for (i = 0; i < numberOfEscorts; i++) {

        escorts[i].id = i + 1;

        type = rand() % 5;

        setEscortType(
            &escorts[i],
            type,
            battleshipMaxVelocity
        );


        escorts[i].x =
            randomDouble(0, canvasSize);

        escorts[i].y =
            randomDouble(0, canvasSize);

        escorts[i].alive = 1;
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

    return sqrt(dx * dx + dy * dy);
}


static double degreesToRadians(double angle)
{
    return angle * M_PI / 180.0;
}


static double maximumAngleFactor(
    double minAngle,
    double maxAngle
)
{
    double minValue =
        sin(2 * degreesToRadians(minAngle));

    double maxValue =
        sin(2 * degreesToRadians(maxAngle));


    if (minAngle <= 45.0 &&
        maxAngle >= 45.0) {

        return 1.0;
    }


    if (minValue > maxValue)
        return minValue;

    return maxValue;
}


static double minimumAngleFactor(
    double minAngle,
    double maxAngle
)
{
    double first =
        sin(2 * degreesToRadians(minAngle));

    double second =
        sin(2 * degreesToRadians(maxAngle));


    if (first < second)
        return first;

    return second;
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
        (battleship.maxVelocity *
         battleship.maxVelocity)
         / GRAVITY;


    return distance <= maximumRange;
}


int escortCanHitBattleship(
    EscortShip escort,
    Battleship battleship
)
{
    double distance;

    double minimumRange;
    double maximumRange;

    double minFactor;
    double maxFactor;


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
        (escort.minVelocity *
         escort.minVelocity *
         minFactor)
         / GRAVITY;


    maximumRange =
        (escort.maxVelocity *
         escort.maxVelocity *
         maxFactor)
         / GRAVITY;


    return distance >= minimumRange &&
           distance <= maximumRange;
}


double calculateBattleshipHitTime(
    Battleship battleship,
    EscortShip escort
)
{
    double distance;
    double velocity;
    double angle = 45.0;
    double time;


    distance =
        calculateDistance(
            battleship.x,
            battleship.y,
            escort.x,
            escort.y
     );

    velocity =
        sqrt(distance * GRAVITY);


    time =
        (2 *
         velocity *
         sin(degreesToRadians(angle)))
         / GRAVITY;


    return time;
}


void displayBattlefield(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts
)
{
    int i;


    printf("      BATTLEFIELD DATA\n");
    


    printf("\nBattleship\n");

    printf("Type       : %c\n",
           battleship.notation);

    printf("Name       : %s\n",
           battleship.name);

    printf("Position   : (%.2f, %.2f)\n",
           battleship.x,
           battleship.y);

    printf("Max Velocity: %.2f\n",
           battleship.maxVelocity);


    printf("\nEscort Ships\n");


    for (i = 0; i < numberOfEscorts; i++)
    {

        printf("\nEscort %d\n", escorts[i].id);

        printf("Type       : %s\n",
               escorts[i].notation);

        printf("Position   : (%.2f, %.2f)\n",
               escorts[i].x,
               escorts[i].y);

        printf("Angles     : %.2f - %.2f\n",
               escorts[i].minAngle,
               escorts[i].maxAngle);

        printf("Velocity   : %.2f - %.2f\n",
               escorts[i].minVelocity,
               escorts[i].maxVelocity);
    }
}
void generateBattlePath(
    double path[][2],
    int numberOfPoints,
    double canvasSize
)
{
    int i;

    for (i = 0; i < numberOfPoints; i++) {

        path[i][0] =
            randomDouble(0, canvasSize);

        path[i][1] =
            randomDouble(0, canvasSize);
    }
}


void displayBattlePath(
    double path[][2],
    int numberOfPoints
)
{
    int i;

   
    printf("      BATTLESHIP PATH\n");

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
