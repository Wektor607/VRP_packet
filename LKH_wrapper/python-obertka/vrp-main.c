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
   // printf("%d\n", maxCapacity);

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
   // printf("%d\n", maxCapacity);


   parseOneTownNoIndex(in, out, countTown);
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

void write_cvrp_subtour(FILE* res_f,town* sub, int len_sub)
{
   for(int i = 0; i < len_sub-1; i++)
   {
      fprintf(res_f, "%d ", sub[i].name);
   }
   fprintf(res_f, "%d#", sub[len_sub-1].name);
}

void write_cvrp_end_tour(FILE* res_f, double distanceInTour)
{
   fprintf(res_f, "@%lf@", distanceInTour);
}

void write_cvrptw_subtour(FILE* res_f, twtown* sub, int len_sub)
{
   for(int i = 0; i < len_sub-1; i++)
   {
      fprintf(res_f, "%d ", sub[i].t.name);
   }
   fprintf(res_f, "%d#", sub[len_sub-1].t.name);
}

void write_cvrptw_end_tour(FILE* res_f, double distanceInTour)
{
   fprintf(res_f, "@%lf@", distanceInTour);
}

#define CVRP(algfunc) \
   srand(time(NULL)); \
   FILE *out = fopen(fileout, "w");\
   FILE *res_distance = fopen("res_distance.txt", "w");\
	if(out == NULL) {exit(-1);}\
   town towns[countTowns]; \
   halfmatrix m; \
   readOneTownByBinaryNoIndex(towns, &m, in);\
   for(int c = 0; c < countTowns; c++) {\
      if(towns[c].weight > maxCapacity) \
      {\
         towns[c] = zerotown; \
         printf("c: %d\n", c);\
      }\
   }\
   town *sub = (town*)malloc((countTowns - 1) * sizeof(town));\
   int w = 0;\
   town t;\
   for(int i = 1; i < countTowns; i++) {\
      t = getTownByName(i, countTowns, towns);\
      if(t.name == -1){\
         printf("Error town: %d\n", t.name);\
         continue;\
      }\
      sub[w] = t;\
      w++;\
   }\
   int newCountTowns = w;\
   sub = realloc(sub, newCountTowns * sizeof(town));\
   town temp[countTowns];\
   temp[0] = towns[0];\
   double distanceInTourBest = -1.0, distanceInTourNew = 0.0, noneOptimalDistance = 0.0;\
   double runtime = clock();\
   for(int i = 0; i < countTasks;i++) {\
      doShuffle(newCountTowns, sub);\
      int cap = 0, l = 0;\
      for(int g = 0; g < newCountTowns; g++) {\
         if(cap + sub[g].weight <= maxCapacity) {\
            temp[l] = sub[g];\
            l++;\
            cap += sub[g].weight;\
         }\
         else {\
            noneOptimalDistance += subtourdistance(temp, l, &m);\
            if(l >= 3) {\
               distanceInTourNew += algfunc(temp, l, &m);\
               write_cvrp_subtour(res_distance, temp, l); \
            }\
            else {\
               distanceInTourNew += subtourdistance(temp, l, &m);\
               write_cvrp_subtour(res_distance, temp, l); \
            }\
            cap = 0; l = 0; g--; }\
         }\
      noneOptimalDistance += subtourdistance(temp, l, &m);\
      if(l >= 3) {\
         distanceInTourNew += algfunc(temp, l, &m);\
         write_cvrp_subtour(res_distance, temp, l); \
      }\
      else {\
         distanceInTourNew += subtourdistance(temp, l, &m);\
         write_cvrp_subtour(res_distance, temp, l); \
      }\
      if(distanceInTourBest == -1.0) {\
         fprintf(out, "%lf\t%lf\n", noneOptimalDistance, 0.0);\
         distanceInTourBest = noneOptimalDistance;}\
      if(distanceInTourNew < distanceInTourBest) {\
         distanceInTourBest = distanceInTourNew;\
         write_cvrp_end_tour(res_distance, distanceInTourBest);\
         fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC); }\
      else {\
         write_cvrp_end_tour(res_distance, -1);\
      } \
      distanceInTourNew = 0.0; }\
   fprintf(out, "%lf\t%lf\n", distanceInTourBest, (clock() - runtime) / CLOCKS_PER_SEC);\
   fputc('\n', out);\
   free(sub); \
   fclose(out); \
   fclose(res_distance); \
   finalizehalfmatrix(&m);

#define CVRPTW(algfunc) \
   srand(time(NULL)); \
   FILE *out = fopen(fileout, "w"); \
   FILE *res_distance = fopen("res_distance.txt", "w");\
   if(res_distance == NULL) {exit(-1);}\
   if(out == NULL) {exit(-1);}\
   twtown *towns; \
   towns = malloc(tcountTown * sizeof(twtown));\
   halfmatrix m; \
   readOneTwTownByBinaryNoIndex(towns, &m, in); \
   twtown town0 = getTwTownByName(0, countTowns, towns);\
   double timer = town0.mTimeStart;\
   double endTime = town0.mTimeEnd;\
   printTwTownList(towns, tcountTown);\
   for(int c = 0; c < countTowns; c++)\
   {\
      if((town0.mTimeStart < town0.mTimeEnd && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeEnd > towns[c].mTimeStart) || \
           (town0.mTimeEnd < towns[c].mTimeStart && town0.mTimeStart > towns[c].mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > town0.mTimeStart))\
      {\
         towns[c].t = zerotown;\
         printf("c: %d\n", c);\
      }\
      else if((town0.mTimeStart > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeEnd) || \
         (towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd < town0.mTimeEnd && town0.mTimeStart > town0.mTimeEnd))\
      {\
         towns[c].mTimeStart = town0.mTimeStart;\
      }\
      else if(towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart < town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd)\
      {\
         towns[c].mTimeStart = town0.mTimeStart;\
         towns[c].mTimeEnd = town0.mTimeEnd;\
      }\
      else if((town0.mTimeEnd > towns[c].mTimeStart && town0.mTimeStart < towns[c].mTimeEnd && town0.mTimeEnd < town0.mTimeStart) || \
         (town0.mTimeStart < towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && towns[c].mTimeEnd < towns[c].mTimeStart && town0.mTimeEnd < town0.mTimeStart) || \
         (town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart > town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd) || \
         (town0.mTimeStart < towns[c].mTimeEnd && towns[c].mTimeStart < town0.mTimeEnd && towns[c].mTimeStart > towns[c].mTimeEnd) || \
         (town0.mTimeStart > towns[c].mTimeEnd && town0.mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > towns[c].mTimeEnd) || \
         (towns[c].mTimeEnd > towns[c].mTimeStart && town0.mTimeEnd > towns[c].mTimeEnd && town0.mTimeStart > town0.mTimeEnd) || \
         (towns[c].mTimeEnd > towns[c].mTimeStart && towns[c].mTimeStart > town0.mTimeStart && towns[c].mTimeEnd > town0.mTimeEnd))\
      {\
         continue;\
      }\
      if(towns[c].t.weight > maxCapacity || (towns[c].mTimeStart - towns[c].mTimeEnd) == 0) {\
         printf("%d", towns[c].t.weight);\
         towns[c].t = zerotown;\
         printf("c: %d\n", c);\
      }\
      if(town0.mTimeStart - town0.mTimeEnd == 0)\
      {\
         printf("Impossible to optimize tour");\
         exit(-1);\
      }\
   }\
   printtwtown(towns[1]);\
   twtown *sub = (twtown*)malloc((countTowns - 1) * sizeof(twtown));\
   int w = 0;\
   twtown t;\
   for(int i = 1; i < countTowns; i++){\
      t = getTwTownByName(i, countTowns, towns);\
      if(t.t.name == -1) {printf("Error town: %d", t.t.name); continue;}\
      sub[w] = t;\
      w++;\
   }\
   int newCountTowns = w;\
   sub = realloc(sub, newCountTowns * sizeof(twtown));\
   printf("sub: ");\
   for(int i = 0; i < newCountTowns; i++) {\
      printf("%d ", sub[i].t.name);\
   }\
   putchar('\n');\
   printtwtown(sub[1]);\
   twtown temp[newCountTowns];\
   temp[0] = towns[0];\
   double td;\
   double distanceInTourBest = -1.0, distanceInTourNew = 0.0;\
   printf("%d", getTwTownByName(16, newCountTowns - 1, sub).t.weight);\
   double runtime = clock();\
   double serviseTime = 0;\
   for(int i = 0; i < newCountTowns; i++) {\
      printf("TUTA %lf %d\n", serviseTime, i);\
      serviseTime += sub[i].mTimeService;\
   }\
   printf("%lf %d\n", serviseTime, newCountTowns);\
   int days, cap, l;\
   for(int i = 0; i < countTasks;i++){\
      days = 1;\
      doShuffleTw(newCountTowns, sub);\
      cap = 0;\
      l = 0;\
      for(int g = 0; g < newCountTowns; g++) {\
         if(cap + sub[g].t.weight <= maxCapacity) {\
            temp[l] = sub[g];\
            l++;\
            cap += sub[g].t.weight;\
         } else {\
            if(l >= 3) {\
               td = algfunc(temp, l, &m, &timer, endTime);\
               if(td == -1) {\
                  days++;\
                  timer = town0.mTimeStart;\
                  td = algfunc(temp, l, &m, &timer, endTime);\
                  if(td == -1) {write_cvrptw_end_tour(res_distance, -1);printf("Skipping Task."); continue;}\
               }\
               write_cvrptw_subtour(res_distance, temp, l); \
               timer += td;\
               distanceInTourNew += td;\
            } else {\
               td = subtourdistanceTw(temp, l, &m, timer, endTime);\
               if(td == -1) {\
                  days++;\
                  timer = town0.mTimeStart;\
                  td = subtourdistanceTw(temp, l, &m, timer, endTime);\
                  if(td == -1) {write_cvrptw_end_tour(res_distance, -1);printf("Skipping Task.");continue;}\
               }\
               write_cvrptw_subtour(res_distance, temp, l); \
               timer += td;\
               distanceInTourNew += td;\
            }\
            cap = 0;l = 0;g--;\
         }\
      }\
      if(l >= 3) {\
         td = algfunc(temp, l, &m, &timer, endTime);\
         if(td == -1) {\
            days++;\
            timer = town0.mTimeStart;\
            td = algfunc(temp, l, &m, &timer, endTime);\
            if(td == -1) {write_cvrptw_end_tour(res_distance, -1);printf("Skipping Task.");continue;}\
         }\
         write_cvrptw_subtour(res_distance, temp, l);\
         timer += td;\
         distanceInTourNew += td;\
      } else {\
         td = subtourdistanceTw(temp, l, &m, timer, endTime);\
         if(td == -1) {\
            days++;\
            timer = town0.mTimeStart;\
            td = subtourdistanceTw(temp, l, &m, timer, endTime);\
            if(td == -1) {write_cvrptw_end_tour(res_distance, -1);printf("Skipping Task.");continue;}\
         }\
         write_cvrptw_subtour(res_distance, temp, l); \
         timer += td;\
         distanceInTourNew += td;\
      }\
      if(distanceInTourBest == -1.0) {\
         fprintf(out, "%lf\t%lf\n", (distanceInTourNew - serviseTime) * kmhToMM, 0.0);\
         distanceInTourBest = distanceInTourNew;   } \
      if(distanceInTourNew < distanceInTourBest) {\
         distanceInTourBest = distanceInTourNew;\
         printf("\nAll days: %d\n", days);\
         write_cvrptw_end_tour(res_distance, (distanceInTourBest - serviseTime) * kmhToMM);\
         fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);\
      }\
      else {\
         write_cvrptw_end_tour(res_distance, -1);\
      }\
      distanceInTourNew = 0.0;\
   }\
   fprintf(out, "%lf\t%lf\n", (distanceInTourBest - serviseTime) * kmhToMM, (clock() - runtime) / CLOCKS_PER_SEC);\
   fputc('\n', out);\
   free(sub);\
   free(towns);\
   fclose(out);\
   fclose(res_distance); \
   finalizehalfmatrix(&m);\
   
static PyObject *modelMetaHeuristic(PyObject *self, PyObject *args) {
   char *in, *algname;
   int tcountTown; 
   double maxCapacity;

   if (!PyArg_ParseTuple(args, "ssid", &algname, &in, &tcountTown, &maxCapacity)) {
      return NULL;
   }
   countTowns = tcountTown;

   if(strcmp(algname, "cvrp_lkh_2opt") == 0) {
      char fileout[] = "LKH_2opt_CVRP_result.txt";
      CVRP(lkh2opt);
   } 
   else if(strcmp(algname, "cvrp_lkh_3opt") == 0) {
      char fileout[] = "LKH_3opt_CVRP_result.txt";
      CVRP(lkh3opt);
   } 
   else if(strcmp(algname, "cvrp_sa") == 0) {
      char fileout[] = "SA_CVRP_result.txt";
      CVRP(sa);
   } 
   else if(strcmp(algname, "cvrptw_lkh_2opt") == 0) {
      char fileout[] = "LKH_2opt_CVRPTW_result.txt";
      CVRPTW(lkh2optTw); 
   } 
   else if(strcmp(algname, "cvrptw_lkh_3opt") == 0) {
      char fileout[] = "LKH_3opt_CVRPTW_result.txt";
      CVRPTW(lkh3optTw); 
   } 
   else if(strcmp(algname, "cvrptw_sa") == 0) {
      char fileout[] = "SA_CVRPTW_result.txt";
      CVRPTW(saTw);
   } else {
      printf("Error algname: %s\n", algname);
      exit(-1);
   }

   return Py_BuildValue("f", "Hello, Python extensions!!");
}

static char helloworld_docs1[] = 
   "Для просмотра документации к программе необходимо:\n"
   "  Если Вы работает в ОС Windows:\n"
   "     1. Открыть Проводник и в пути до папки с файлами написать: cmd. Автоматически должна открыть командная строка с полным путем до основной папки.\n"
   "     2. Написать в командной строке команду: start /sphinx/_build/html/index.html .\n"
   "  Если Вы работает в ОС Linux:\n"
   "     1. Прописать в командной строке полный путь до папки с файлами.\n"
   "     2. Написать команду: open \\sphinx\\_build\\html\\index.html .\n"
   "  После чего автоматически откроется документация в одном из Ваших браузеров на локальном компьютере.\n"
   "Для выхода из файла для помощи доступа к документации введите команду: :wq .";

static PyMethodDef helloworld_funcs[] = {
   {"OpenDocumentation", (PyCFunction)parseOneTownPy,
      METH_VARARGS, helloworld_docs1},
   {NULL}
};

static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "VRP", /* name of module */
    "Extension module example!",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    helloworld_funcs
};

PyMODINIT_FUNC PyInit_vrp_c(void)
{
    return PyModule_Create(&cModPyDem);
}
