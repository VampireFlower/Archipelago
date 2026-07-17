#include <types.h>

#ifndef MGTT_LIES_H
#define MGTT_LIES_H


typedef enum LieID { // Lie enum without lie qualities. there are 65 (0...64 inclusive) values according to 804d93c0
    Tee=0,
    Fairway=1,
    Green=2,
    Bunker=3,
    Rough=4,
    Water_Hazard=5,
    RoughOB=6,
    Cart_Path=7,
    TeeBox=8,
    Rock=9,
    Semi_Rough=10,
    Fringe=11,
    Tree_Leaf=12,
    Tree=13, // lakitu tree trunks
    OB3=14,
    OB4=15,
    OB5=16,
    Fast_Fairway=17,
    Heath=18,
    TreeHazard2=19, // pcg tree leaves
    Water_Surface=20,
    WaterHazard3=21,
    HeavyRough=22,
    Waste_Area=23,
    Water_Surface_0=24,
    Water_Surface_1=25,
    Mushroom=26,
    PCG_Cloud=27,
    Water_Surface_2=28,
    Pipe=29,
    Pipe_Warp=30, // Piranha Plant Mouth too
    Chain_Chomp=31,
    Chain_Chomp_Pit=32,
    Grass_Bunker=33,
    WasteArea2=34,
    Wood_0=35,
    Beach_Bunker=36,
    Stone_Bridge=37,
    Wood_1=38, // Used in the ships
    Rock_3=39,
    Rock_PCG_Castle=40,
    Thwomp=41,
    Cactus=42,
    OB7=43,
    Boo=44,
    OB9=45,
    Lava=46,
    Bobomb=47,
    WoodOB=48,
    Brick=49,
    Star=50,
    Fence=51, // From Badlands Hole 13
    Ship_Sail=52,
    Piranha_Plant=53
} LieID;

_Static_assert(sizeof(LieID) == 4, "Unexpected enum size");


#endif