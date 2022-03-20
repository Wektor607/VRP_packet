#include <stdio.h>

#include "parameters.h"
#include "generate-script/generate-bin-lib.h"
// #include "generate-script/parameters-bin.h"

#include "logistic_tw.h"
#include "distanceMatrix.h"

int main()
{
    srand(time(NULL));
    FILE *out = fopen(fileout, "w");
    twtown towns[21];
    halfmatrix m;

    // parseOneTwTown("20/20201101_150113.csv", "twtowntest", 1);

    readOneTwTownByBinary(towns, &m, "twtowntest", 1);
    twtown town0 = getTwTownByName(0, countTowns, towns);

    double timer = town0.mTimeStart;
    double endTime = town0.mTimeEnd;

    printTwTownList(towns, 21);

    int arctown0, arctownc;

    for(int c = 0; c < countTowns; c++) 
    {

        if((town0.mTimeStart < town0.mTimeEnd && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeEnd > towns[c].mTimeStart) || \
           (town0.mTimeEnd < towns[c].mTimeStart && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > town0.mTimeStart))
        {
            towns[c].t = zerotown;
            printf("c: %d\n", c);
        }
        else if((town0.mTimeStart > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeEnd) || \
           (towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd < town0.mTimeEnd && town0.mTimeStart > town0.mTimeEnd))
        {
            towns[c].mTimeStart = town0.mTimeStart;
        }
        else if(towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)
        {
            towns[c].mTimeStart = town0.mTimeStart;
            towns[c].mTimeEnd = town0.mTimeEnd;
        }
        else if((town0.mTimeEnd > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd < town0.mTimeStart) || \
           (town0.mTimeStart < towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && towns[c].mTimeEnd < towns[c].mTimeStart && town0.mTimeEnd < town0.mTimeStart) || \
           (town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart > town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd) || \
           (town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart < town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd) || \
           (town0.mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > towns[c].mTimeEnd) || \
           (towns[c].mTimeEnd > towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && town0.mTimeStart > town0.mTimeEnd) || \
           (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd))
        {
            continue;
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
            fprintf(out, "%lf\t%lf\n", (distanceInTourNew - serviseTime) * kmhToMM, 0.0);
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