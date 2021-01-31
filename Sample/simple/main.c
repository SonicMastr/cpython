#include <stdlib.h>
#include <Python.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	Py_SetProgramName("app0:eboot.bin");
	setenv("PYTHONHOME", "app0:", 1);
	setenv("PYTHONTHREADDEBUG", "1", 1);
	Py_Initialize();

	PyRun_SimpleString("import sys\n");
	PyRun_SimpleString("import os\n");
	PyRun_SimpleString("print os.name\n");
	PyRun_SimpleString("print sys.platform\n");
	PyRun_SimpleString("print sys.path\n");
	PyRun_SimpleString("print sys.builtin_module_names\n");

	Py_Finalize();
	return 0;
}
