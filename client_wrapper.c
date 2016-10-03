#include <Python.h>

static PyObject * client_wrapper(PyObject* self, PyObject* args){
	char* input;
	char* result;
	PyObject* ret;

//parse arguments
	if(~PyArg_ParseTuple(args, "s", &input)){
		return NULL;
	}

//run function
	result = main(input);
	
//build into Python object
	ret = PyString_FromString(result);
	free(result);

	return ret;
}

static PyMethodDef ClientMain_Methods[] = {
	{"main", client_wrapper, METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}
};

DL_EXPORT(void) initclient(void){
	Py_InitModule("main", ClientMainMethods);
}

