#include <stdio.h>
#include "logistic.h"


typedef struct timeWindowTown {
    town t;
    double mTimeStart, mTimeEnd;
    double mTimeService;
} twtown;

twtown maketwtown(town t, double mTimeStart, double mTimeEnd, double mTimeService) {
    twtown tw;
    tw.t = t;
    tw.mTimeService = mTimeService;
    tw.mTimeStart = mTimeStart;
    tw.mTimeEnd = mTimeEnd;
    return tw;
}

#define errortwtown maketwtown(errortown, -1, -1, -1)

twtown getTwTownByName(int name, int counttown, const twtown* towns)
{
    // towns = malloc(counttown * sizeof(twtown));
    for(int i = 0; i < counttown; i++)
    {
        printf("IF\n");
        if(towns[i].t.name == name) {
            printf("ЕЕЕЕ\n");
            return towns[i];
        }
    }
    return errortwtown;
}

void printtwtown(const twtown t) {
    printf("\n--*--\n");
    printtown(t.t);
    printf("%lf-%lf service:%lf\n", t.mTimeStart, t.mTimeEnd, t.mTimeService);
}

void swapTw(twtown *town1, twtown *town2)
{
    twtown m = *town1;
    *town1 = *town2;
    *town2 = m;
}

void doShuffleTw(int counttown, twtown *towns)
{
    int j, k;
    for(int i = 0; i < counttown; i++) {
        k = rand() % counttown;
        j = rand() % counttown;
        swapTw(&towns[j], &towns[k]);
    }
}

double subtourdistanceTw(twtown *sub, int lenSub, halfmatrix* m, const double timer, const double endTime)
{
    // 1 2 3 4 #4
    // 01 12 23 34 40 #5
    //printTownList(lenSub, sub);
    if(lenSub == 0) {
        return 0;
    }
    //printf("lenSub: %d\n", lenSub);
    //printf("all sub: [");
    /*
    for(int i = 0; i < lenSub; i++) {
        printf("%d ", sub[i].name);
    }
    printf("]\n");
    printf("sub[lenSub-1].name: %d\n", sub[lenSub-1].name);*/
    double localtimer = timer;

    localtimer += getByTown(m, 0, sub[0].t.name) + sub[0].mTimeService;
    if(!(localtimer >= sub[0].mTimeStart && localtimer <= sub[0].mTimeEnd && localtimer <= endTime)) {
        return -1;
    }

    localtimer += getByTown(m, 0, sub[lenSub-1].t.name);
    if(!(localtimer >= sub[lenSub-1].mTimeStart && localtimer <= sub[lenSub-1].mTimeEnd && localtimer <= endTime)) {
        return -1;
    }

    //double r = getByTown(m, 0, sub[0].t.name) + getByTown(m, 0, sub[lenSub-1].t.name);
    //printf("@%lf %lf\n", getByTown(m, 0, sub[0].name), getByTown(m, 0, sub[lenSub-1].name));

    for(int i = 0; i < lenSub-1; i++)
    {
        //printf("@%lf\n", getByTown(m, sub[i].name, sub[i+1].name));
        //printf("sub[i+1].name: %d\n", sub[i+1].name);
        localtimer += getByTown(m, sub[i].t.name, sub[i+1].t.name) + sub[i+1].mTimeService;
        if(!(localtimer >= sub[i].mTimeStart && localtimer <= sub[i].mTimeEnd && localtimer <= endTime)) {
            return -1;
        }
        //r += getByTown(m, sub[i].t.name, sub[i+1].t.name);
    }
    return localtimer - timer;
}


void printTwTownList(const twtown* towns, int counttowns) {
    printf("[");
    for(int i = 0; i < counttowns; i++) {
        printf("%d ", towns[i].t.name);
    }
    printf("]");
}

void reverseTownTw(twtown *sub, int i, int j)
{
    int s = (j + i) / 2;
    if((j + i) % 2 == 1) {
        for(int k = 0; k < (j - i + 1) / 2; k++)
        {
            swapTw(&sub[s - k], &sub[s+1+k]);
        }
    } else {
        for(int k = 0; k < (j - i) / 2; k++)
        {
            swapTw(&sub[s-1-k], &sub[s+1+k]);
        }
    }
}

int moveElemsTw(twtown *sub, int start1, int end1, int start2, int end2)
{
    int difference = (end1 - start1 - (end2 - start2));
    // 0 1 2 3 4 5 6 7 8 9 10
    //[0 1 2 3]4 5 6[7 8 9]10
    //difference = 3 - 0 - (9 - 7) = 1

    twtown tmp;

    twtown *mtmp = (twtown*)malloc(abs(difference) * sizeof(twtown));
    if(mtmp == NULL) {
        return -1;
    }
    for(int i = 0; i < abs(difference); i++) {
        if(difference > 0) {
            mtmp[i] = sub[end1 + 1 - difference + i];
            //printf("%d ", mtmp[i].name);
        } else if(difference < 0) {
            mtmp[i] = sub[start2 + i];
        }
    }
    //putchar('\n');
    if(difference > 0) {
        //0[1 2 3 4]5 6 7[8 9]10
        //0[1 2 3 4 5]6[7]8 9 10
        //printTownList(11, sub);
        for(int i = 0; i < end2 - end1; i++) { //start2 + start1 - (end1 - start1 - (end2 - start2)) = 2*start2 + 2 *start1 - end1 - end2
            sub[end1 + 1 - difference + i] = sub[end1 + 1 + i];
        }
        //0[1 2 5 6]7 8 9[* *]10
        //0[1 6 7 * *]*[*]8 9 10
        //printTownList(11, sub);
        for(int i = 0; i < end2 - start2 + 1; i++) {
            tmp = sub[start1 + i];
            sub[start1 + i] = sub[start2 + i - difference];
            sub[start2 + i - difference] = tmp;
        }
        //printTownList(11, sub);
        //0[8 9 5 6]7 1 2[* *]10
        for(int i = 0; i < abs(difference); i++) {
            sub[end2 + 1 - difference +i] = mtmp[i];
        }
        //printTownList(11, sub);
        //0[8 9 5 6]7 1 2[3 4]10
        //0{8 9}5 6 7{1 2 3 4}10
        //printTownList(11, sub);

    } else if(difference < 0) {
        //[0 1 2]3 4[5 6 7 8 9]10
        //printTownList(11, sub);
        for(int i = 0; i < end1 - start1 + 1; i++) {
            tmp = sub[start1 + i];
            sub[start1 + i] = sub[start2 - difference + i];
            sub[start2 - difference + i] = tmp;
        }
        //printTownList(11, sub);
        //[7 8 9]3 4[5 6 0 1 2]10

        for(int i = 0; i < start2-start1; i++) {
            sub[start2 - difference - 1 - i] = sub[start2 - 1 - i];
        }
        //[* * 7]8 9[3 4 0 1 2]10
        //printTownList(11, sub);
        for(int i = 0; i < abs(difference); i++) {
            sub[start1 + i] = mtmp[i];
        }
        //{5 6 7 8 9}3 4{0 1 2}10
        //printTownList(11, sub);
    }

    //[7 8 9 3]4 5 6[0 1 2]10

    //[7 8 9 4]5 6 0[1 2 3]10
    free(mtmp);
    return 0;
}


double lkh3optTw(twtown *sub, int lenSub, halfmatrix *m, double *timer, const double endTime) // timer - is a now time. Global time on the world.
{

    /*
    2-opt
    0: Or O  O
    1: O  Or O
    2: O  O  Or
    3-opt
    3: O  [O  O ]
    4: Or [O  O ]
    5: O  [Or O ]
    6: O  [O  Or]
    */
    twtown *subcopy = (twtown*)malloc(lenSub * sizeof(twtown));
    //цикл копирования sub -> subcopy
    for(int i = 0; i < lenSub; i++)
    {
        subcopy[i] = sub[i];
    }

    double best = subtourdistanceTw(subcopy, lenSub, m, *timer, endTime), newd;
    if(best == 0) {
        return -1;
    }

    printf("\n--*--\nOld total time: %lf\n", best);
    printf("Old list: "); printTwTownList(subcopy, lenSub); putchar('\n');
    // int a0, b0, a, b, 
    int mode;
    double localtimer = *timer;

    double runtime = clock(); 
    //for(int i = 0; 0; i++)//ALGFOR(i); i++)
    //{
        for(int a = 0; a < lenSub; a++) {
            for(int b = a + 1; b < lenSub; b++) {
                /*
                mode = rand() % 7;
                // mode = 6;
                a0 = rand() % (lenSub - 1);
                b0 = rand() % (lenSub - 1);
                
                while(a0==b0) {
                    b0 = rand() % (lenSub - 1);
                }
                a = my_min(a0, b0);
                b = my_max(a0, b0);
                */
                for(mode = 0; mode < 7; mode++) {
                    switch(mode){
                        case(0): {reverseTownTw(subcopy, 0, a);break;}
                        case(1): {reverseTownTw(subcopy, a+1, b);break;}
                        case(2): {reverseTownTw(subcopy, b+1, lenSub-1);break;}
                        case(3): {moveElemsTw(subcopy, a+1, b, b+1,lenSub-1);break;}
                        // case 4, 5, 6 - crash program: Segmentation Fault
                        case(4): {
                            
                            reverseTownTw(subcopy, 0, a);
                            moveElemsTw(subcopy, a+1, b, b+1,lenSub-1);
                            
                            break;
                        }
                        case(5): {
                            
                            reverseTownTw(subcopy, a+1, b);
                            moveElemsTw(subcopy, a+1, b, b+1,lenSub-1);
                        
                            break;
                        }
                        case(6): {
                            // printf("BEFOR_REVERSE: ");
                            // for(int i = 0; i < lenSub; i++)
                            // {
                            //  printf("%d, ", subcopy[i].name);
                            // }
                            // putchar('\n');
                            // printf("a:%d\n", a);
                            // printf("b:%d\n", b);
                            reverseTownTw(subcopy, b+1, lenSub-1);
                            // printf("AFTER_REVERSE: ");
                            // for(int i = 0; i < lenSub; i++)
                            // {
                            //  printf("%d, ", subcopy[i].name);
                            // }
                            // putchar('\n');
                            moveElemsTw(subcopy, a+1, b, b+1,lenSub-1);
                            // printf("AFTER_MOVE: ");
                            // for(int i = 0; i < lenSub; i++)
                            // {
                            //  printf("%d, ", subcopy[i].name);
                            // }
                            // putchar('\n');
                            break;
                        }
                    }

                    newd = subtourdistanceTw(subcopy, lenSub, m, *timer, endTime);
                    if(best == -1 && newd != -1) {
                        best = newd;
                        //цикл копирования subcopy -> sub
                        for(int j = 0; j < lenSub; j++)
                        {
                            sub[j] = subcopy[j];
                        }
                    }

                    if(newd != -1 && newd < best) {
                        best = newd;
                        //цикл копирования subcopy -> sub
                        for(int j = 0; j < lenSub; j++)
                        {
                            sub[j] = subcopy[j];
                        }
                    } else {
                        for(int j = 0; j < lenSub; j++)
                        {
                            subcopy[j] = sub[j];
                        }
                    }
                }
            }
        }
    //}
    free(subcopy);
    
    printf("New total time: %lf\n", best);
    printf("New list: "); printTwTownList(sub, lenSub);
    if(best != -1) {
        *timer += best;  
    }
    
    return best;
}

void GenerateStateCandidateTw(twtown *sub, twtown *best, int lenSub) 
{
    int indexA = rand() % lenSub;
    int indexB = rand() % lenSub;
    while(indexA == indexB){
        indexB = rand() % lenSub;
    }

    for(int i = 0; i < my_min(indexA,indexB); i++) 
    {
        sub[i] = best[i];
    }

    for(int i = 0; i < my_max(indexA, indexB) - my_min(indexA, indexB) + 1; i++) 
    { 
        sub[my_min(indexA, indexB) + i] = best[my_max(indexA, indexB) - i]; 
    }

    for(int i = my_max(indexA, indexB) + 1; i < lenSub; i++) 
    {
        sub[i] = best[i];
    }
}

double saTw(twtown *sub, int lenSub, halfmatrix *m, double* timer, const double endTime) {
    twtown subcopy[lenSub];
    //цикл копирования sub -> subcopy
    for(int i = 0; i < lenSub; i++)
    {
        subcopy[i] = sub[i];
    }

    double best = subtourdistanceTw(subcopy, lenSub, m, *timer, endTime), newd, p;
    double runtime = clock(); 
    int T = tmax;
    for(int k = 0; T >= tmin && clock() - runtime < 600000000; T = tmax / (k + 1), k++) {
        GenerateStateCandidateTw(subcopy, sub, lenSub);

        newd = subtourdistanceTw(subcopy, lenSub, m, *timer, endTime);
        if(best == -1 && newd != -1) {
            best = newd;
            //цикл копирования subcopy -> sub
            for(int j = 0; j < lenSub; j++)
            {
                sub[j] = subcopy[j];
            }
        }

        if(newd != -1 && newd < best) {
            best = newd;
            for(int i = 0; i < lenSub; i++) 
            {
                sub[i] = subcopy[i];
            }
        } else if(newd != -1) {
            p = exp((best - newd) / T);
            if(p > (rand() % 10000 / 10000.0)) 
            {
                best = newd;
                for(int i = 0; i < lenSub; i++) 
                {
                    sub[i] = subcopy[i];
                }
            }
        }
    }
    *timer += best;
    return best;
}



int read_file_tw(const char* name_file, twtown *towns, int counttowns)
{
    FILE* in;
    in = fopen(name_file, "r");
    if(in == NULL)
    {
        //printf("11 Error %d \n", errno);
        return -1;
    }
    int symbol;
    for(int i = 0; (symbol = fgetc(in)) != EOF && symbol != '\n'; i++);


    double x, y, cap, st;
    int hst, mst, het, met;
    for(int i = counttowns-1; i >= 0 ; i--)
    {
        fscanf(in, "%lf\t%lf\t%lf\t%d:%d-%d:%d\t%lf\n", &x, &y, &cap, &hst, &mst, &het, &met, &st);
        towns[i] = maketwtown(maketown(i, x, y, cap), hst * 60 + mst, het * 60 + met, st);
    }
    //fscanf(in, "%lf\t%lf\t\n", &depot[0], &depot[1]);
    fclose(in);
    return 0;
}

void parseOneTwTown(const char pathFile[], const char newFileName[], int index)
{

    halfmatrix m;
    inithalfmatrix(&m, countTowns-1);
    twtown towns[countTowns];
    


    char pathTown[2000], pathTable[2000];

    snprintf(pathTown, 2000, "%s/town%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
    snprintf(pathTable, 2000, "%s/table%lld/%s%d.bin", pathSavingTowns, countTowns - 1, newFileName, index);
    printf("%s\n", pathTown);
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

    read_file_tw(pathFile, towns, countTowns);

    printTwTownList(towns, countTowns);
    printf("After town printing!\n");
    //printtown(getTownByName(0, countTowns, towns));
    //printtown(getTownByName(1, countTowns, towns));
    //printAllMap(countTowns, towns);
    /*
    int maxCapacity = -1;
    
    for(int c = 0; c < countTowns; c++) {
        if(towns[c].t.weight > maxCapacity) {
            maxCapacity = towns[c].t.weight;
        }
    }
    maxCapacity *= 4;*/


    for(int i = 0; i < countTowns; i++)
    {
        for(int j = 0; j < countTowns-i-1; j++)
        {
            //printf("t%d %d t%d %lf\n", i, j, m.width-j, getDistance(towns[i], towns[m.width-j]));
            //m.data[i][j] = getDistance(towns[j], towns[i]);
            /*if(getTwTownByName(i, countTowns, towns).t.weight + getTwTownByName(m.width-j, countTowns, towns).t.weight > maxCapacity) {
                pointAthalfmatrix(&m, i, j, -1.0);
                continue;
            }*/
            pointAthalfmatrix(&m, i, j, getDistance(getTwTownByName(i, countTowns, towns).t, getTwTownByName(m.width-j, countTowns, towns).t) / kmhToMM);
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
        fwrite(&towns[i], sizeof(struct timeWindowTown), 1, outtown);
    }

    //fwrite(&m, sizeof(struct halfmatrix), 1, outtable);
    //fwrite(towns, sizeof(struct town*), 1, outtown);
    finalizehalfmatrix(&m);
    fclose(outtown);
    fclose(outtable);

}


void parseOneTwTownNoIndex(const char pathFile[], const char newFileName[], int tcountTowns)
{

    halfmatrix m;
    inithalfmatrix(&m, tcountTowns-1);
    twtown towns[tcountTowns];
    


    char pathTown[2000], pathTable[2000];

    snprintf(pathTown, 2000, "%s-town.bin", newFileName);
    snprintf(pathTable, 2000, "%s-table.bin", newFileName);
    printf("%s\n", pathTown);
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

    read_file_tw(pathFile, towns, tcountTowns);

    printTwTownList(towns, tcountTowns);
    printf("After town printing!\n");
    //printtown(getTownByName(0, countTowns, towns));
    //printtown(getTownByName(1, countTowns, towns));
    //printAllMap(countTowns, towns);
    /*
    int maxCapacity = -1;
    
    for(int c = 0; c < countTowns; c++) {
        if(towns[c].t.weight > maxCapacity) {
            maxCapacity = towns[c].t.weight;
        }
    }
    maxCapacity *= 4;*/


    for(int i = 0; i < tcountTowns; i++)
    {
        for(int j = 0; j < tcountTowns-i-1; j++)
        {
            //printf("t%d %d t%d %lf\n", i, j, m.width-j, getDistance(towns[i], towns[m.width-j]));
            //m.data[i][j] = getDistance(towns[j], towns[i]);
            /*if(getTwTownByName(i, countTowns, towns).t.weight + getTwTownByName(m.width-j, countTowns, towns).t.weight > maxCapacity) {
                pointAthalfmatrix(&m, i, j, -1.0);
                continue;
            }*/
            pointAthalfmatrix(&m, i, j, getDistance(getTwTownByName(i, tcountTowns, towns).t, getTwTownByName(m.width-j, tcountTowns, towns).t) / kmhToMM);
        }
    }
    printtownmatrix(&m);
    

    fwrite(&m.width, sizeof(int), 1, outtable);
    for(int i = 0; i < m.width; i++) {
        for(int j = 0; j < m.width - i; j++) {
            fwrite(&m.data[i][j], sizeof(double), 1, outtable);
        }
    }
    int tmp = tcountTowns;
    fwrite(&tmp, sizeof(int), 1, outtown);
    for(int i = 0; i < tcountTowns; i++) {
        fwrite(&towns[i], sizeof(struct timeWindowTown), 1, outtown);
    }

    //fwrite(&m, sizeof(struct halfmatrix), 1, outtable);
    //fwrite(towns, sizeof(struct town*), 1, outtown);
    finalizehalfmatrix(&m);
    fclose(outtown);
    fclose(outtable);

}





void readOneTwTownByBinary(twtown *towns, halfmatrix *m, const char newFileName[], int index) {
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
        fread(&towns[i], sizeof(struct timeWindowTown), 1, intown);
    }
}

void readOneTwTownByBinaryNoIndex(twtown *towns, halfmatrix *m, const char newFileName[]) {
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
    printf("End readOneTownByBinary\n");
    fread(&m->width, sizeof(int), 1, intable);

    inithalfmatrix(m, m->width);

    // printf("YES7");
    for(int i = 0; i < m->width; i++) {
        for(int j = 0; j < m->width - i; j++) {
            //printf("%d %d\n", i, j);
            fread(&m->data[i][j], sizeof(double), 1, intable);
        }
    }

    int tmp;
    fread(&tmp, sizeof(int), 1, intown);
    printf("%d\n", tmp);
    for(int i = 0; i < tmp; i++) {
        // printf("YES11\n");
        // printf("%lf", malloc(sizeof(struct timeWindowTown)));
        fread(&towns[i], sizeof(struct timeWindowTown), 1, intown);
        // printf("YES12\n");
    }
    printf("End readOneTownByBinary\n");

}


int main()
{
    srand(time(NULL));
    FILE *out = fopen(fileout, "w");
    twtown towns[21];
    halfmatrix m;
    // read_file_tw(towns, 21);
    // for(int i = 0; i < 21; i++) {
    //     printtwtown(towns[i]);
    // }
    // parseOneTwTown("20200925_093755.csv", "twtowntest", 2);

    readOneTwTownByBinary(towns, &m, "twtowntest", 2);
    //printtwtown(towns[0]);
    twtown town0 = getTwTownByName(0, countTowns, towns);
    /*
    В некоторых ситуациях скорее всего "Карета" будет превращаться в "Тыкву"(Работать некоректно) - я думаю при прохождении через 00:00
    town0.mTimeStart = 18 * 60;
    town0.mTimeEnd =   12 * 60;
    */
    // town0.mTimeStart = 21 * 60;
    // town0.mTimeEnd =   23 * 60;
    double timer = town0.mTimeStart;
    double endTime = town0.mTimeEnd;

    printTwTownList(towns, 21);

    // int arctown0, arctownc;

    for(int c = 0; c < countTowns; c++) {

        // 1 и 11
        if(town0.mTimeStart < town0.mTimeEnd && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeEnd > towns[c].mTimeStart)
        {
            printf("1/11 ");
            towns[c].t = zerotown;
            printf("c: %d\n", c);
        }

        // 12
        if(town0.mTimeEnd < towns[c].mTimeStart && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > town0.mTimeStart) 
        {
            printf("12 ");
            towns[c].t = zerotown;
            printf("c: %d\n", c);
        }

        // 2
        if(town0.mTimeStart > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeEnd)
        {
            printf("2 ");
            towns[c].mTimeStart = town0.mTimeStart;
        }

        // 3
        if(town0.mTimeEnd > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd < town0.mTimeStart)
        {
            printf("3 ");
            continue;
        }

        // 4
        if(town0.mTimeStart < towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && towns[c].mTimeEnd < towns[c].mTimeStart && town0.mTimeEnd < town0.mTimeStart)
        {
            printf("4 ");
            continue;
        }

        //5
        if(town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart > town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd)
        {
            printf("5 ");
            continue;
        }

        //6
        if(town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart < town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd)
        {
            printf("6 ");
            continue;
        }

        // 7
        if (town0.mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > towns[c].mTimeEnd)
        {
            printf("7 ");
            continue;
        }
        // 8
        if (towns[c].mTimeEnd > towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && town0.mTimeStart > town0.mTimeEnd)
        {
            printf("8 ");
            continue;
        }
        
        // 9
        if (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)
        {
            printf("9 ");
            continue;
        }

        // 9 и 3/4
        if (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)
        {
            printf("9 3/4 ");
            towns[c].mTimeStart = town0.mTimeStart;
            towns[c].mTimeEnd = town0.mTimeEnd;
        }
        
        // 10
        if (towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd < town0.mTimeEnd && town0.mTimeStart > town0.mTimeEnd)
        {
            printf("10 ");
            towns[c].mTimeStart = town0.mTimeStart;
        }
        
        if(towns[c].t.weight > maxCapacity || (towns[c].mTimeStart - towns[c].mTimeEnd) == 0) {
            printf("%d", towns[c].t.weight);
            towns[c].t = zerotown;
            printf("c: %d\n", c);
        }

        if(town0.mTimeStart - town0.mTimeEnd == 0)
        {
            printf("Impossible to optimize tour");
            exit(-1);
        }
    }

    printtwtown(towns[1]);

    twtown *sub = (twtown*)malloc((countTowns - 1) * sizeof(twtown));
    int w = 0; twtown t;
    for(int i = 1; i < countTowns; i++)
    {
        t = getTwTownByName(i, countTowns, towns);
        if(t.t.name == -1) {
            printf("Error town: %d\n", t.t.name);
            continue;
        }
        sub[w] = t;
        w++;
    }

    int newCountTowns = w;
    sub = realloc(sub, newCountTowns * sizeof(twtown));

    printf("sub: ");
    for(int i = 0; i < newCountTowns; i++) {
        printf("%d ", sub[i].t.name);
    } putchar('\n');
    printtwtown(sub[1]);

    twtown temp[countTowns];// координаты |
    temp[0] = towns[0];
    double td;
    
    double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0;
    printf("%d\n", getTwTownByName(16, newCountTowns - 1, sub).t.weight);
    double runtime = clock();

    double serviseTime = 0;

    for(int i = 0; i < newCountTowns; i++) {
        serviseTime += sub[i].mTimeService;
    }
    printf("%lf %d\n", serviseTime, newCountTowns);
    for(int i = 0; i < countTasks;i++)
    {
        int days = 1;
        doShuffleTw(newCountTowns, sub);
        //printTownList(newCountTowns, sub);

        int cap = 0, l = 0;
        for(int g = 0; g < newCountTowns; g++) {
            
            if(cap + sub[g].t.weight <= maxCapacity) {
                temp[l] = sub[g];
                l++;
                cap += sub[g].t.weight;
            } else {
                noneOptimalDistance += subtourdistanceTw(temp, l, &m, timer, endTime);
                //printTownList(l, temp);
                if(l >= 3) {
                    td = LKH(temp, l, &m, &timer, endTime);
                    if(td == -1) {
                        days++;
                        timer = town0.mTimeStart;
                        td = LKH(temp, l, &m, &timer, endTime);
                        if(td == -1) {
                            printf("Skipping Task.\n");
                            continue;
                        }
                    }
                    printf("!td: %lf\n", td);
                    timer += td;
                    distanceInTourNew += td;
                } else {
                    td = subtourdistanceTw(temp, l, &m, timer, endTime);
                    if(td == -1) {
                        days++;
                        timer = town0.mTimeStart;
                        td = subtourdistanceTw(temp, l, &m, timer, endTime);
                        if(td == -1) {
                            printf("Skipping Task.\n");
                            continue;
                        }
                    }
                    timer += td;
                    distanceInTourNew += td;
                }
                cap = 0;
                l = 0;
                g--;
            }
        }
        //printTownList(l, temp);
        //noneOptimalDistance += subtourdistanceTw(temp, l, &m, timer, endTime);
        //printTownList(l, temp);
        if(l >= 3) {
            td = LKH(temp, l, &m, &timer, endTime);
            if(td == -1) {
                days++;
                timer = town0.mTimeStart;
                td = LKH(temp, l, &m, &timer, endTime);
                if(td == -1) {
                    printf("Skipping Task.\n");
                    continue;
                }
            }
            printf("!td: %lf\n", td);
            timer += td;
            distanceInTourNew += td;
        } else {
            td = subtourdistanceTw(temp, l, &m, timer, endTime);
            if(td == -1) {
                days++;
                timer = town0.mTimeStart;
                td = subtourdistanceTw(temp, l, &m, timer, endTime);
                if(td == -1) {
                    printf("Skipping Task.\n");
                    continue;
                }
            }
            timer += td;
            distanceInTourNew += td;
        }


        if(distanceInTourBest == -1.0) {
            //printf("I\'m in if\n");
            fprintf(out, "%lf\t%lf\n", (distanceInTourNew - serviseTime) * kmhToMM, 0.0);//noneOptimalDistance, 0.0);
            distanceInTourBest = distanceInTourNew;
        }


        if(distanceInTourNew < distanceInTourBest) {
            distanceInTourBest = distanceInTourNew;
            fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);
        }
        distanceInTourNew = 0.0;
        printf("All days: %d\n", days);
    }
    fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);
    fputc('\n', out);
    free(sub);

    fclose(out);
    finalizehalfmatrix(&m);

    return 0;
}