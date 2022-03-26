#include <Python.h>
#include "generate-script/generate-bin-lib.h"
#include "cvrptw-logistic.c"
#include "logistic.h"

// Компиляция программы: gcc -I/usr/include/python3.8 -c vrp-main.c -lm -o mac 
// sudo python3 setup.py install
// python3

static PyObject *parseOneTwTownPy(PyObject *self, PyObject *args) {
   char *in;
   char *out;
   int tcountTown;


   if (!PyArg_ParseTuple(args, "ssi", &in, &out, &tcountTown)) {
      return NULL;
   }
   printf("%d\n", maxCapacity);

   parseOneTwTownNoIndex(in, out, tcountTown);
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static PyObject *parseOneTownPy(PyObject *self, PyObject *args) {
   char *in;
   char *out;
   int countTown;


   if (!PyArg_ParseTuple(args, "ssi", &in, &out, &countTown)) {
      return NULL;
   }
   printf("%d\n", maxCapacity);


   parseOneTownNoIndex(in, out, countTown);
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

#define CVRP(algfunc) \
   srand(time(NULL)); \
   FILE *out = fopen(fileout, "w");\
	if(out == NULL) {exit(-1);}\
   town towns[countTowns]; \
   halfmatrix m; \
	for(int i = 0; i < countFilesBin; i++) { readOneTownByBinaryNoIndex(towns, &m, in); for(int c = 0; c < countTowns; c++) { if(towns[c].weight > maxCapacity) { towns[c] = zerotown; printf("c: %d\n", c); } } town *sub = (town*)malloc((countTowns - 1) * sizeof(town)); int w = 0; town t; for(int i = 1; i < countTowns; i++) { t = getTownByName(i, countTowns, towns); if(t.name == -1){ printf("Error town: %d\n", t.name); continue; } sub[w] = t; w++; } int newCountTowns = w; sub = realloc(sub, newCountTowns * sizeof(town)); town temp[countTowns]; temp[0] = towns[0]; double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0; double runtime = clock(); for(int i = 0; i < countTasks;i++) { doShuffle(newCountTowns, sub); int cap = 0, l = 0; for(int g = 0; g < newCountTowns; g++) { if(cap + sub[g].weight <= maxCapacity) { temp[l] = sub[g]; l++; cap += sub[g].weight; } else { noneOptimalDistance += subtourdistance(temp, l, &m); if(l >= 3) { distanceInTourNew += algfunc(temp, l, &m); } else { distanceInTourNew += subtourdistance(temp, l, &m); } cap = 0; l = 0; g--; } } noneOptimalDistance += subtourdistance(temp, l, &m); if(l >= 3) { distanceInTourNew += algfunc(temp, l, &m); } else { distanceInTourNew += subtourdistance(temp, l, &m); } if(distanceInTourBest == -1.0) { fprintf(out, "%lf\t%lf\n", noneOptimalDistance, 0.0); distanceInTourBest = noneOptimalDistance; } if(distanceInTourNew < distanceInTourBest) { distanceInTourBest = distanceInTourNew; fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC); } distanceInTourNew = 0.0; } fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC); fputc('\n', out); free(sub); } fclose(out); finalizehalfmatrix(&m);


#define CVRPTW(algfunc) \
   srand(time(NULL)); \
   FILE *out = fopen(fileout, "w"); \
   twtown *towns; \
   towns = malloc(tcountTown * sizeof(twtown));\
   halfmatrix m; \
   readOneTwTownByBinaryNoIndex(towns, &m, in); \
   twtown town0 = getTwTownByName(0, countTowns, towns);double timer = town0.mTimeStart;double endTime = town0.mTimeEnd;printTwTownList(towns, tcountTown); for(int c = 0; c < tcountTown; c++) {     if(town0.mTimeStart < town0.mTimeEnd && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeEnd > towns[c].mTimeStart)    {        printf("1/11 ");        towns[c].t = zerotown;        printf("c: %d", c);    }    if(town0.mTimeEnd < towns[c].mTimeStart && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > town0.mTimeStart)     {        printf("12 ");        towns[c].t = zerotown;        printf("c: %d", c);    }    if(town0.mTimeStart > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeEnd)    {        printf("2 ");        towns[c].mTimeStart = town0.mTimeStart;    }    if(town0.mTimeEnd > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd < town0.mTimeStart)    {        printf("3 ");        continue;    }    if(town0.mTimeStart < towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && towns[c].mTimeEnd < towns[c].mTimeStart && town0.mTimeEnd < town0.mTimeStart)    {        printf("4 ");        continue;    }    if(town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart > town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd)    {        printf("5 ");        continue;    }    if(town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart < town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd)    {        printf("6 ");        continue;    }    if (town0.mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > towns[c].mTimeEnd)    {        printf("7 ");        continue;    }    if (towns[c].mTimeEnd > towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && town0.mTimeStart > town0.mTimeEnd)    {        printf("8 ");        continue;    }        if (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)    {        printf("9 ");        continue;    }    if (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)    {        printf("9 3/4 ");        towns[c].mTimeStart = town0.mTimeStart;        towns[c].mTimeEnd = town0.mTimeEnd;    }        if (towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd < town0.mTimeEnd && town0.mTimeStart > town0.mTimeEnd)    {        printf("10 ");        towns[c].mTimeStart = town0.mTimeStart;    }        if(towns[c].t.weight > maxCapacity || (towns[c].mTimeStart - towns[c].mTimeEnd) == 0) {        printf("%d", towns[c].t.weight);        towns[c].t = zerotown;        printf("c: %d", c);    }    if(town0.mTimeStart - town0.mTimeEnd == 0)    {        printf("Impossible to optimize tour");        exit(-1);    }}printtwtown(towns[1]);twtown *sub = (twtown*)malloc((countTowns - 1) * sizeof(twtown));int w = 0; twtown t;for(int i = 1; i < countTowns; i++){    t = getTwTownByName(i, countTowns, towns);    if(t.t.name == -1) {        printf("Error town: %d", t.t.name);        continue;    }    sub[w] = t;    w++;}int newCountTowns = w;sub = realloc(sub, newCountTowns * sizeof(twtown));printf("sub: ");for(int i = 0; i < newCountTowns; i++) {    printf("%d ", sub[i].t.name);} putchar('\n');printtwtown(sub[1]);twtown temp[countTowns];temp[0] = towns[0];double td;double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0;printf("%d", getTwTownByName(16, newCountTowns - 1, sub).t.weight);double runtime = clock();double serviseTime = 0;for(int i = 0; i < newCountTowns; i++) {    serviseTime += sub[i].mTimeService;}printf("%lf %d", serviseTime, newCountTowns);for(int i = 0; i < countTasks;i++){    int days = 1;    doShuffleTw(newCountTowns, sub);    int cap = 0, l = 0;    for(int g = 0; g < newCountTowns; g++) {                if(cap + sub[g].t.weight <= maxCapacity) {            temp[l] = sub[g];            l++;            cap += sub[g].t.weight;        } else {            noneOptimalDistance += subtourdistanceTw(temp, l, &m, timer, endTime);            if(l >= 3) {                td = algfunc(temp, l, &m, &timer, endTime);                if(td == -1) {                    days++;                    timer = town0.mTimeStart;                    td = algfunc(temp, l, &m, &timer, endTime);                    if(td == -1) {                        printf("Skipping Task.");                        continue;                    }                }                printf("\nFinal total time for subtour: %lf", td);                timer += td;                distanceInTourNew += td;            } else {                td = subtourdistanceTw(temp, l, &m, timer, endTime);                if(td == -1) {                    days++;                    timer = town0.mTimeStart;                    td = subtourdistanceTw(temp, l, &m, timer, endTime);                    if(td == -1) {                        printf("Skipping Task.");                        continue;                    }                }                timer += td;                distanceInTourNew += td;            }            cap = 0;            l = 0;            g--;        }    }    if(l >= 3) {        td = algfunc(temp, l, &m, &timer, endTime);        if(td == -1) {            days++;            timer = town0.mTimeStart;            td = algfunc(temp, l, &m, &timer, endTime);            if(td == -1) {                printf("Skipping Task.");                continue;            }        }        printf("\nFinal total time for subtour: %lf", td);        timer += td;        distanceInTourNew += td;    } else {        td = subtourdistanceTw(temp, l, &m, timer, endTime);        if(td == -1) {            days++;            timer = town0.mTimeStart;            td = subtourdistanceTw(temp, l, &m, timer, endTime);            if(td == -1) {                printf("Skipping Task.");                continue;            }        }        timer += td;        distanceInTourNew += td;    }    if(distanceInTourBest == -1.0) {        fprintf(out, "%lf\t%lf\n", (distanceInTourNew - serviseTime) * kmhToMM, 0.0);       distanceInTourBest = distanceInTourNew;    }    if(distanceInTourNew < distanceInTourBest) {        distanceInTourBest = distanceInTourNew;        fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);    }    distanceInTourNew = 0.0;    printf("\nAll days: %d\n", days);}fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);fputc('\n', out);free(sub);fclose(out);finalizehalfmatrix(&m);
   
static PyObject *modelMetaHeuristic(PyObject *self, PyObject *args) {
   char *in, *algname;
   int tcountTown; double maxCapacity;

   if (!PyArg_ParseTuple(args, "ssid", &algname, &in, &tcountTown, &maxCapacity)) {
      return NULL;
   }

   if(strcmp(algname, "cvrp_lkh") == 0) {
      CVRP(lkh3opt);
   } 
   else if(strcmp(algname, "cvrp_sa") == 0) {
      CVRP(sa);
   } 
   else if(strcmp(algname, "cvrptw_lkh") == 0) {
      CVRPTW(lkh3optTw);
      
   } 
   else if(strcmp(algname, "cvrptw_sa") == 0) {
      CVRPTW(saTw);
   } else {
      printf("Error algname: %s\n", algname);
      exit(-1);
   }

   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
   "parseOneTownPy(filename1, filename2, countTowns, maxCapacity): create a bin-file with distance-table and town-list\n"
   "parseOneTwTownPy(filename1, filename2, countTowns): analog parseOneTownPy, but with time parsing\n"
   "modelMetaHeuristic(algname, filename, countTowns, maxCapacity) - choose your algoritm and enter your bin-file\n";

static PyMethodDef helloworld_funcs[] = {
   {"parseOneTownPy", (PyCFunction)parseOneTownPy,
      METH_VARARGS, helloworld_docs},
   {"parseOneTwTownPy", (PyCFunction)parseOneTwTownPy,
      METH_VARARGS, helloworld_docs},
   {"modelMetaHeuristic", (PyCFunction)modelMetaHeuristic,
      METH_VARARGS, helloworld_docs},
   {NULL}
};

static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "cvrp", /* name of module */
    "Extension module example!",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    helloworld_funcs
};

PyMODINIT_FUNC PyInit_cvrp(void)
{
    return PyModule_Create(&cModPyDem);
}
