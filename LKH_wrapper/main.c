#include <stdio.h>

#include "parameters.h"
#include "generate-script/generate-bin-lib.h"
#include "generate-script/parameters-bin.h"

#include "logistic.h"
#include "distanceMatrix.h"


int main()
{
	srand(time(NULL));

	halfmatrix m;

	FILE *out = fopen(fileout, "w");
	if(out == NULL) {
		exit(-1);
	}

	town towns[countTowns];
	
	for(int i = 0; i < countFilesBin; i++)
	{
		readOneTownByBinary(towns, &m, "test", i);

		int max_vehicle_car = 30;
		int maxCapacity = 0;
		for(int c = 0; c < countTowns; c++) {
			if(towns[c].weight > maxCapacity) {
				maxCapacity = towns[c].weight;
			}
		}
		int count = 1;
		while(maxCapacity > max_vehicle_car)
		{
			max_vehicle_car = max_vehicle_car / count;
			count++;
			max_vehicle_car *= count;
		}
	
		printtownmatrix(&m);

		town sub[countTowns - 1]; // города
		for(int i = 1; i < countTowns; i++)
		{
			sub[i-1] = getTownByName(i, countTowns, towns);
		}


		for(int i = 0; i < countTowns - 1; i++) {
			printf("%d ", sub[i].name);
		} putchar('\n');


		town temp[countTowns];// координаты |
		temp[0] = towns[0];
		
		double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0;
		// printf("%d\n", getTownByName(16, countTowns - 1, sub).weight);
		double runtime = clock();
		for(int i = 0; i < countTasks;i++)
		{

			doShuffle(countTowns - 1, sub);
			printTownList(countTowns - 1, sub);

			int cap = 0, l = 0;
			for(int g = 0; g < countTowns - 1; g++) {
				
				if(cap + sub[g].weight <= max_vehicle_car) {
					temp[l] = sub[g];
					l++;
					cap += sub[g].weight;
				} else {
					noneOptimalDistance += subtourdistance(temp, l, &m);
					//printTownList(l, temp);
					if(l >= 3) {
						distanceInTourNew += LKH(temp, l, &m);
					} else {
						distanceInTourNew += subtourdistance(temp, l, &m);
					}
					cap = 0;
					l = 0;
					g--;
				}
			}
			//printTownList(l, temp);
			noneOptimalDistance += subtourdistance(temp, l, &m);
			//printTownList(l, temp);
			if(l >= 3) {
				distanceInTourNew += LKH(temp, l, &m);
			} else {
				distanceInTourNew += subtourdistance(temp, l, &m);
			}


			if(distanceInTourBest == -1.0) {
				printf("I\'m in if\n");
				fprintf(out, "%lf\t%lf\n", noneOptimalDistance, 0.0);
				distanceInTourBest = noneOptimalDistance;
			}


			if(distanceInTourNew < distanceInTourBest) {
				distanceInTourBest = distanceInTourNew;
			    fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC);
			}
			distanceInTourNew = 0.0;
		}
		fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC);
		fputc('\n', out);
	}
	
	fclose(out);
	finalizehalfmatrix(&m);
	return 0;
}