#include "generate-bin-lib.h"
#include "../distanceMatrix.h"

int main() {
	FILESbin;

	halfmatrix m;
	town towns[countTowns];

	for(int i = 0; i < countFilesBin; i++) {
		parseOneTown(mfiles[i], noteFile, i);
		printf("%sstatus: %d/%d%s\n", YELLOW, i+1, countFilesBin, RESET);
		readOneTownByBinary(towns, &m, noteFile, i);
		printtownmatrix(&m);
		printTownList(countTowns, towns);
	}

	return 0;
}