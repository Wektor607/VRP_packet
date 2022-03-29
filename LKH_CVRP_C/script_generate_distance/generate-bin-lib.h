#include <stdio.h>
#include <stdlib.h>
#include "../parameters.h"
#include "../distanceMatrix.h"
#include "../logistic.h"

void parseOneTown(const char pathFile[], const char newFileName[], int index)
{

	halfmatrix m;
	inithalfmatrix(&m, countTowns-1);
	town towns[countTowns];
	


	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s/town%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
	snprintf(pathTable, 2000, "%s/table%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
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

	read_file(pathFile, towns, countTowns);

	printTownList(countTowns, towns);

	double d1, d2, d3;
	printf("???\n");
	for(int i = 0; i < (int)((countTowns - countTowns % 2) / 2); i++)
	{
		printf("i: %d\n", i);
		for(int j = i+1; j < countTowns-2; j++)
		{
			printf("j: %d\n", j);

			printf("exit code: %d\n" , getDistanceTriangle( \
				getTownByName(i, countTowns, towns), \
				getTownByName(j, countTowns, towns), \
				getTownByName(j+1, countTowns, towns), \
				&d1, &d2, &d3));

			printf("d1, d2, d3: %lf, %lf, %lf\n", d1, d2, d3);
			pointAthalfmatrix(&m, i, j, d1);
			pointAthalfmatrix(&m, j, j+1, d2);
			pointAthalfmatrix(&m, i, j+1, d3);
		}
	}
	printtownmatrix(&m);
	

	fwrite(&m.width, sizeof(int), 1, outtable);
	for(int i = 0; i < m.width; i++) {
		for(int j = 0; j < m.width - i; j++) {
			fwrite(&m.data[i][j], sizeof(double), 1, outtable);
		}
	}
	int tmp = countTowns;
	fwrite(&tmp, sizeof(int), 1, outtown);
	for(int i = 0; i < countTowns; i++) {
		fwrite(&towns[i], sizeof(struct town), 1, outtown);
	}

	//fwrite(&m, sizeof(struct halfmatrix), 1, outtable);
	//fwrite(towns, sizeof(struct town*), 1, outtown);
	finalizehalfmatrix(&m);
	fclose(outtown);
	fclose(outtable);

}

void parseOneTownNoIndex(const char pathFile[], const char newFileName[], int countTowns) //TODO:
{

	halfmatrix m;
	inithalfmatrix(&m, countTowns-1);
	town towns[countTowns];
	
	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s-town.bin", newFileName);
	snprintf(pathTable, 2000, "%s-table.bin", newFileName);

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

	read_file(pathFile, towns, countTowns);

	printTownList(countTowns, towns);

	int maxCapacity = -1;
	
	for(int c = 0; c < countTowns; c++) {
		if(towns[c].weight > maxCapacity) {
			maxCapacity = towns[c].weight;
		}
	}
	maxCapacity *= 4;

	for(int i = 0; i < countTowns; i++)
	{
		for(int j = 0; j < countTowns-i-1; j++)
		{
			if(getTownByName(i, countTowns, towns).weight + getTownByName(m.width-j, countTowns, towns).weight > maxCapacity) {
				pointAthalfmatrix(&m, i, j, -1.0);
				continue;
			}
			pointAthalfmatrix(&m, i, j, getDistance(getTownByName(i, countTowns, towns), getTownByName(m.width-j, countTowns, towns)));
		}
	}
	printtownmatrix(&m);
	
	fwrite(&m.width, sizeof(int), 1, outtable);
	for(int i = 0; i < m.width; i++) {
		for(int j = 0; j < m.width - i; j++) {
			fwrite(&m.data[i][j], sizeof(double), 1, outtable);
		}
	}

	int tmp = countTowns;
	fwrite(&tmp, sizeof(int), 1, outtown);
	for(int i = 0; i < countTowns; i++) {
		fwrite(&towns[i], sizeof(struct town), 1, outtown);
	}

	finalizehalfmatrix(&m);
	fclose(outtown);
	fclose(outtable);

}

void readOneTownByBinary(town *towns, halfmatrix *m, const char newFileName[], int index) {
	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s/town%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
	snprintf(pathTable, 2000, "%s/table%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
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

void readOneTownByBinaryNoIndex(town *towns, halfmatrix *m, const char newFileName[]) {
	char pathTown[2000], pathTable[2000];

	snprintf(pathTown, 2000, "%s-town.bin", newFileName);
	snprintf(pathTable, 2000, "%s-table.bin", newFileName);
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


