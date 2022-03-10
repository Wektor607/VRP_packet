
#pragma once
// for towns
//#define countTowns 21

// for cars
//#define maxCapacity 30
long long maxCountCar =  3;
double optimalRadius = 0.37;
//#define countFiles 1

#define STARTTOWNS \
	town towns[] = { \
		maketown(0, 0.77, 0.24, 0), \
		maketown(1, 0.1, 0.2, 30), \
		maketown(2, 0.15, 0.35, 23), \
		maketown(3, 0.43, 0.27, 10), \
		maketown(4, 0.9, 0.13, 2), \
		maketown(5, 0.81, 0.55, 73), \
		maketown(6, 0.28, 0.61, 48), \
		maketown(7, 0.37, 0.22, 51), \
		maketown(8, 0.21, 0.69, 7), \
		maketown(9, 0.7 , 0.9 , 24), \
		maketown(10,0.13, 0.91, 62), \
		maketown(11,0.89, 0.01, 27), \
		maketown(12,0.32, 0.51, 87), \
		maketown(13,0.48, 0.47, 41) \
	}

// for lkh

long long countTasks = 150000;
//#define timeUpdate 1500000
#define LKH lkh3opt
#define fileout "tmp/LKH_result_100.txt"
int maxCapacity = 50;
//for sa
long long tmax = 1000000000;
long long tmin = 1;

//parameters-bin
long long countTowns = 101;
#define countFilesBin 18

#define pathSavingTowns "bin-towns"
#define noteFile "test"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLACK   "\033[37;40m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[37;46m"


#define FILESbin \
	char *mfiles[] = { \
		"100(20 задач)/20201020_182616.csv", \
		"100(20 задач)/20201021_185504.csv", \
		"100(20 задач)/20201111_223641.csv", \
		"100(20 задач)/20201118_223519.csv", \
		"100(20 задач)/20201119_230826.csv", \
		"100(20 задач)/20201123_210620.csv", \
		"100(20 задач)/20201125_032739.csv", \
		"100(20 задач)/20201125_225552.csv", \
		"100(20 задач)/20201126_190637.csv", \
		"100(20 задач)/20201130_210949.csv", \
		"100(20 задач)/20201201_165823.csv", \
		"100(20 задач)/20201202_191224.csv", \
		"100(20 задач)/20201208_202137.csv", \
		"100(20 задач)/20201215_221946.csv", \
		"100(20 задач)/20201216_183747.csv", \
		"100(20 задач)/20201217_144624.csv", \
		"100(20 задач)/20201224_012524.csv", \
		"100(20 задач)/20210105_235706.csv" \
	}
