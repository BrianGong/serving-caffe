#pragma once

extern "C" {
  void Py_Initialize();
  void Py_Finalize();

  const char* Py_GetVersion();
  char* Py_GetPath();
  int  PyImport_AppendInittab(const char *name, void (*initfunc)(void));
  void PySys_SetPath(const char *path);
  int PyRun_SimpleString(const char *command);
  void PyErr_PrintEx(int set_sys_last_vars);

  typedef struct {} PyObject;

  PyObject* PyErr_Occurred();
  void PyErr_Clear();
}