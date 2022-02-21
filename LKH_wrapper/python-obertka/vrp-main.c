#include "/Library/Frameworks/Python.framework/Versions/3.10/include/python3.10/Python.h"
#include "../script_generate_distance/generate-bin-lib.h"
static PyObject* helloworld(PyObject* self) {
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static PyObject *foo_add_subtract(PyObject *self, PyObject *args) {
   char *in;
   char *out;
   int tcountTown;
   int maxCapacity;


   if (!PyArg_ParseTuple(args, "ssii", &in, &out, &tcountTown, &maxCapacity)) {
      return NULL;
   }
   printf("%d\n", maxCapacity);


   parseOneTownNoIndex(in, out, tcountTown, maxCapacity);
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
   "helloworld( ): Any message you want to put here!!\n";

static PyMethodDef helloworld_funcs[] = {
   {"helloworld", (PyCFunction)helloworld,
      METH_NOARGS, helloworld_docs},
   {"foo_add_subtract", (PyCFunction)foo_add_subtract,
      METH_VARARGS, helloworld_docs},
    {NULL}
};

static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "helloworld", /* name of module */
    "Extension module example!",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    helloworld_funcs
};

PyMODINIT_FUNC PyInit_helloworld(void)
{
    return PyModule_Create(&cModPyDem);
}