#include <stdio.h>
#include <stdlib.h>
#include "parameters-bin.h"
#include "../distanceMatrix.h"
#include "../logistic.h"

void parseOneTown(const char pathFile[], const char newFileName[], int index)
{

	halfmatrix m;
	inithalfmatrix(&m, countTownsBin-1);
	town towns[countTownsBin];
	
	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s/town%d/%s%d.bin", pathSavingTowns, countTownsBin - 1, newFileName, index);
	snprintf(pathTable, 2000, "%s/table%d/%s%d.bin", pathSavingTowns, countTownsBin - 1, newFileName, index);
	//printf("%s\n", pathTown);
	FILE* outtown = fopen(pathTown, "wb");
	if(outtown == NULL) {
		printf("Error writing file: %s", pathTown);
		exit(-1);
	}
	FILE* outtable = fopen(pathTable, "wb");
	if(outtable == NULL) {
		fclose(outtown);
		printf("Error writing file: %s", pathTable);
		exit(-1);
	}

	read_file(pathFile, towns, countTownsBin);

	printTownList(countTownsBin, towns);

	int maxCapacity = MaxCapacity;

	for(int i = 0; i < countTownsBin; i++)
	{
		for(int j = 0; j < countTownsBin-i-1; j++)
		{
			if(getTownByName(i, countTownsBin, towns).weight + getTownByName(m.width-j, countTownsBin, towns).weight > maxCapacity) 
			{
				pointAthalfmatrix(&m, i, j, -1.0);
				continue;
			}
			pointAthalfmatrix(&m, i, j, getDistance(getTownByName(i, countTownsBin, towns), getTownByName(m.width-j, countTownsBin, towns)));
		}
	}
	printtownmatrix(&m);
	

	fwrite(&m.width, sizeof(int), 1, outtable);
	for(int i = 0; i < m.width; i++) {
		for(int j = 0; j < m.width - i; j++) {
			fwrite(&m.data[i][j], sizeof(double), 1, outtable);
		}
	}
	int tmp = countTownsBin;
	fwrite(&tmp, sizeof(int), 1, outtown);
	for(int i = 0; i < countTownsBin; i++) {
		fwrite(&towns[i], sizeof(struct town), 1, outtown);
	}

	//fwrite(&m, sizeof(struct halfmatrix), 1, outtable);
	//fwrite(towns, sizeof(struct town*), 1, outtown);
	finalizehalfmatrix(&m);
	fclose(outtown);
	fclose(outtable);

}

void readOneTownByBinary(town *towns, halfmatrix *m, const char newFileName[], int index) {
	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s/town%d/%s%d.bin", pathSavingTowns, countTownsBin - 1, newFileName, index);
	snprintf(pathTable, 2000, "%s/table%d/%s%d.bin", pathSavingTowns, countTownsBin - 1, newFileName, index);
	printf("%s\n", pathTown);
	FILE* intown = fopen(pathTown, "r");
	if(intown == NULL) {
		printf("Error writing file: %s", pathTown);
		exit(-1);
	}
	FILE* intable = fopen(pathTable, "r");
	if(intable == NULL) {
		fclose(intown);
		printf("Error writing file: %s", pathTable);
		exit(-1);
	}

	//fread(towns, sizeof(struct town*), 1, intown);
	//fread(m, sizeof(struct halfmatrix), 1, intable);
	fread(&m->width, sizeof(int), 1, intable);
	inithalfmatrix(m, m->width);
	for(int i = 0; i < m->width; i++) {
		for(int j = 0; j < m->width - i; j++) {
			//printf("%d %d\n", i, j);
			fread(&m->data[i][j], sizeof(double), 1, intable);
		}
	}

	int tmp;
	fread(&tmp, sizeof(int), 1, intown);
	for(int i = 0; i < tmp; i++) {
		fread(&towns[i], sizeof(struct town), 1, intown);
	}
}