#include <stdio.h>

#include "parameters.h"
#include "generate-script/generate-bin-lib.h"
// #include "generate-script/parameters-bin.h"

#include "logistic.h"
#include "distanceMatrix.h"


int main()
{
	srand(time(NULL));
	//STARTTOWNS;
	//FILES;


	halfmatrix m;
	//inithalfmatrix(&m, countTowns-1);


	FILE *out = fopen(fileout, "w");
	if(out == NULL) {
		exit(-1);
	}

	town towns[countTowns];
	for(int i = 0; i < countFilesBin; i++)
	{
		readOneTownByBinary(towns, &m, "test", i);
		// printf("twn: ");
		// for(int i = 0; i < countTowns; i++) {
		// 	printf("%d ", towns[i].name);
		// } putchar('\n');
		// printtown(towns[1]);		
	
		for(int c = 0; c < countTowns; c++) {
			if(towns[c].weight > maxCapacity) {
				towns[c] = zerotown;
				printf("c: %d\n", c);
			}
		}

		// printtownmatrix(&m);
		//printf("%lf\n", getByTown(&m, 2, 11));
		town *sub = (town*)malloc((countTowns - 1) * sizeof(town)); // города
		
		int w = 0; town t;
		for(int i = 1; i < countTowns; i++)
		{
			t = getTownByName(i, countTowns, towns);
			if(t.name == -1){
				printf("Error town: %d\n", t.name);
				continue;
			}
			sub[w] = t;
			w++;
		}
		int newCountTowns = w;
		sub = realloc(sub, newCountTowns * sizeof(town));

		// printf("sub: ");
		// for(int i = 0; i < newCountTowns; i++) {
		// 	printf("%d ", sub[i].name);
		// } putchar('\n');
		// printtown(sub[1]);

		town temp[countTowns];// координаты |
		temp[0] = towns[0];
		
		double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0;
		// printf("%d\n", getTownByName(16, countTowns - 1, sub).weight);
		double runtime = clock();
		for(int i = 0; i < countTasks;i++)
		{

			doShuffle(newCountTowns, sub);
			//printTownList(newCountTowns, sub);

			int cap = 0, l = 0;
			for(int g = 0; g < newCountTowns; g++) {
				
				if(cap + sub[g].weight <= maxCapacity) {
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
				//printf("I\'m in if\n");
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
		free(sub);
	}
	fclose(out);
	finalizehalfmatrix(&m);
	return 0;
}