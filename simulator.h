#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_ESCORTS 100
#define GRAVITY 9.81

typedef struct
{
    char notation;
    char name[50];
    char gunName[60];

    double x;
    double y;

    double maxVelocity;

    double reloadTime;
    double nextFireTime;

    int alive;

} Battleship;


typedef struct
{
    int id;

    char notation[3];
    char typeName[50];
    char gunName[60];

    double impactPower;

    double minAngle;
    double maxAngle;

    double minVelocity;
    double maxVelocity;

    double x;
    double y;

    double reloadTime;
    double nextFireTime;

    int alive;

} EscortShip;


void setupBattleship(
    Battleship *battleship,
    double canvasSize
);


void generateEscortShips(
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize,
    double battleshipMaxVelocity
);

double calculateDistance(
    double x1,
    double y1,
    double x2,
    double y2
);


int battleshipCanHit(
    Battleship battleship,
    EscortShip escort
);


int escortCanHitBattleship(
    EscortShip escort,
    Battleship battleship
);


double calculateBattleshipFiringAngle(
    Battleship battleship,
    EscortShip escort
);


int battleshipCanHitJammed(
    Battleship battleship,
    EscortShip escort,
    double minimumAngle
);


int selectBestTarget(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    int gunJammed,
    double minimumAngle
);


void generateBattlePath(
    double path[][2],
    int numberOfPoints,
    double canvasSize
);


void displayBattlePath(
    double path[][2],
    int numberOfPoints
);


void displayBattlefield(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts
);

void displayVisualBattlefield(
    Battleship battleship,
    EscortShip escorts[],
    int numberOfEscorts,
    double canvasSize
);

#endif
