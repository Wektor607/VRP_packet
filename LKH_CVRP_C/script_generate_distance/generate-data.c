#include "parameters-bin.h"
#include "generate-bin-lib.h"

int main() {
	FILESbin;

	for(int i = 0; i < countFilesBin; i++) {
		printf("%sstatus: %d/%d%s\n", YELLOW, i+1, countFilesBin, RESET);
		parseOneTown(mfiles[i], noteFile, i);
	}

	//parseOneTown("/Users/ivandruzkov/project/LKH0/20(20_задач)/20200925_093755.csv", "test", 0);
	//halfmatrix m;
	//town towns[21];
	//readOneTownByBinary(towns, &m, noteFile, 0);
	//printtownmatrix(&m);
	//printTownList(countTowns, towns);
	return 0;
}