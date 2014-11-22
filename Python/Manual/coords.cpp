// ==========================================================
// Filename:    space.cpp
//
// Description: Contains the python wrappers for the Cartesian::space
//              objects.
//
// See also:    http://docs.python.org/extending/newtypes.html
//              http://docs.python.org/c-api/complex.html
//              https://docs.python.org/2/reference/datamodel.html
//
// Author:      L.R. McFarland
// Created:     2014 Nov 21
// ==========================================================

#include <Python.h> // must be first
#include <structmember.h> // part of python

#include <sstream>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

// ===================
// ===== statics =====
// ===================

static PyObject* sCoordsException; // exception holder

// TODO: make precision configuralble on build, not hardcoded.
static const unsigned int sPrintPrecision(12); // matches defaut %s precision for unit test

// Angle

// char* kwlist[] init strings
static char sDegreeStr[] = "degees";
static char sMinuteStr[] = "minutes";
static char sSecondStr[] = "seconds";
static char sMinimumStr[] = "minimum";
static char sMaximumStr[] = "maximum";

static char sValueStr[] = "value";
static char sRadiansStr[] = "radians";

// Cartesian

// char* kwlist[] init strings
static char sXstr[] = "x";
static char sYstr[] = "y";
static char sZstr[] = "z";

// TODO spherical


// -----------------
// ----- Angle -----
// -----------------

// ------------------------
// ----- constructors -----
// ------------------------


// Angle object definition.
typedef struct {
  PyObject_HEAD
  Coords::angle m_angle;
} Angle;


// Forward declarations for as_number methods. Wraps Type definition.
static void new_AngleType(Angle** an_angle);
static int is_AngleType(PyObject* an_angle);


static PyObject* Angle_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  Angle* self(NULL);
  self = (Angle*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}


static int Angle_init(Angle* self, PyObject* args, PyObject* kwds) {

  double degrees(0);
  double minutes(0);
  double seconds(0);

  static char* kwlist[] = {sDegreeStr, sMinuteStr, sSecondStr, NULL};

  if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ddd", kwlist, &degrees, &minutes, &seconds))
    return -1;

  // value initialized to 0 by new.
  self->m_angle.value(Coords::degrees2seconds(degrees, minutes, seconds)/3600);

  return 0;
}


static void Angle_dealloc(Angle* self) {
  self->ob_type->tp_free((PyObject*)self);
}


// -----------------
// ----- print -----
// -----------------

PyObject* Angle_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Angle*)self)->m_angle;
  return PyString_FromString(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* Angle_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Angle*)self)->m_angle;
  return PyString_FromString(result.str().c_str());
}


// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- value -----

static PyObject* Angle_getValue(Angle* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.value());
}

static int Angle_setValue(Angle* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "Cannot delete value");
    return 0;
  }

  if (!PyFloat_Check(value) && !PyInt_Check(value)) {
    PyErr_SetString(sCoordsException, "value must be a float");
    return 0;
  }

  self->m_angle.value(PyFloat_AsDouble(value));

  return 0;
}

// ----- radians -----

static PyObject* Angle_getRadians(Angle* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.radians());
}

static int Angle_setRadians(Angle* self, PyObject* radians, void* closure) {

  if (radians == NULL) {
    PyErr_SetString(sCoordsException, "Cannot delete radians");
    return 0;
  }

  if (!PyFloat_Check(radians) && !PyInt_Check(radians)) {
    PyErr_SetString(sCoordsException, "radians must be a float");
    return 0;
  }

  self->m_angle.radians(PyFloat_AsDouble(radians));

  return 0;
}

// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* Angle_nb_add(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create angle");
    return NULL;
  }

  Coords::angle the_sum(((Angle*)o1)->m_angle + ((Angle*)o2)->m_angle);

  // copy because m_angle constructor is TBD.
  result_angle->m_angle = the_sum;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_subtract(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create angle");
    return NULL;
  }

  Coords::angle the_difference(((Angle*)o1)->m_angle - ((Angle*)o2)->m_angle);

  // copy because m_angle constructor is TBD.
  result_angle->m_angle = the_difference;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_negative(PyObject* o1) {
  // Unitary minus

  if (!is_AngleType(o1)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "negative failed to create angle");
    return NULL;
  }

  Coords::angle the_inverse = -((Angle*)o1)->m_angle;

  // copy because m_angle constructor is TBD.
  result_angle->m_angle = the_inverse;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_multiply(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create angle");
    return NULL;
  }

  result_angle->m_angle = ((Angle*)o1)->m_angle * ((Angle*)o2)->m_angle;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_divide(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create angle");
    return NULL;
  }

  try {
    result_angle->m_angle = ((Angle*)o1)->m_angle / ((Angle*)o2)->m_angle;
  } catch (Coords::Error err) {
    PyErr_SetString(sCoordsException, err.what());
    return NULL;
  }

  return (PyObject*) result_angle;
}


static PyObject* Angle_tp_richcompare(PyObject* o1, PyObject* o2, int op) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }


  // TODO >, >=, <, <=

  if (op == Py_LT) {

    if (((Angle*)o1)->m_angle < ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_LE) {

    if (((Angle*)o1)->m_angle <= ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_EQ) {

    if (((Angle*)o1)->m_angle == ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_NE) {

    if (((Angle*)o1)->m_angle != ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GT) {

    if (((Angle*)o1)->m_angle > ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GE) {

    if (((Angle*)o1)->m_angle >= ((Angle*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else {

    PyErr_SetString(PyExc_TypeError, "richcompare op not supported");
    return NULL;

  }

}

// ---------------------------
// ----- inplace methods -----
// ---------------------------

static PyObject* Angle_nb_inplace_add(PyObject* o1, PyObject* o2) {
  // TODO can this be implement directly using space::operator+=()?
  // problem with refence going out of scope, segfault.
  return Angle_nb_add(o1, o2);
}

static PyObject* Angle_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  // TOOD implement directly?
  return Angle_nb_subtract(o1, o2);
}

static PyObject* Angle_nb_inplace_multiply(PyObject* o1, PyObject* o2) {
  // TOOD implement directly?
  return Angle_nb_multiply(o1, o2);
}

static PyObject* Angle_nb_inplace_divide(PyObject* o1, PyObject* o2) {
  // TOOD implement directly?
  return Angle_nb_divide(o1, o2);
}

// --------------------------
// ----- Python structs -----
// --------------------------

// ----- deg2rad -----

PyDoc_STRVAR(coords_deg2rad__doc__, "converts degrees into radians");

static PyObject* deg2rad(PyObject* self, PyObject *args) {
  double radians(0);
  if (!PyArg_ParseTuple(args, "d", &radians))
    return NULL;
  double degrees = Coords::angle::deg2rad(radians);
  return (PyObject*)  Py_BuildValue("d", degrees);
}


// ----- rad2deg -----

PyDoc_STRVAR(coords_rad2deg__doc__, "converts radians into degrees");

static PyObject* rad2deg(PyObject* self, PyObject *args) {
  double radians(0);
  if (!PyArg_ParseTuple(args, "d", &radians))
    return NULL;
  double degrees = Coords::angle::rad2deg(radians);
  return (PyObject*)  Py_BuildValue("d", degrees);
}


static PyMethodDef Angle_methods[] = {
  {"deg2rad", (PyCFunction) deg2rad, METH_VARARGS, coords_deg2rad__doc__},
  {"rad2deg", (PyCFunction) rad2deg, METH_VARARGS, coords_rad2deg__doc__},
  {NULL}  /* Sentinel */
};


static PyMemberDef Angle_members[] = {
  {NULL}  /* Sentinel */
};

static PyGetSetDef Angle_getseters[] = {
  {sValueStr, (getter)Angle_getValue, (setter)Angle_setValue, sValueStr, NULL},
  {sRadiansStr, (getter)Angle_getRadians, (setter)Angle_setRadians, sRadiansStr, NULL},
  {NULL}  /* Sentinel */
};


// see http://docs.python.org/c-api/typeobj.html
static PyNumberMethods Angle_as_number = {
  (binaryfunc) Angle_nb_add,
  (binaryfunc) Angle_nb_subtract,
  (binaryfunc) Angle_nb_multiply,
  (binaryfunc) Angle_nb_divide,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Angle_nb_negative,
  (unaryfunc) 0,   // nb_positive
  (unaryfunc) 0,   // nb_absolute
  (inquiry) 0,     // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,   // nb_invert
  (binaryfunc) 0,  // nb_lshift
  (binaryfunc) 0,  // nb_rshift
  (binaryfunc) 0,  // nb_and
  (binaryfunc) 0,  // nb_xor
  (binaryfunc) 0,  // nb_or
  (coercion) 0,    // Used by the coerce() function
  (unaryfunc) 0,   // nb_int
  (unaryfunc) 0,   // nb_long
  (unaryfunc) 0,   // nb_float
  (unaryfunc) 0,   // nb_oct
  (unaryfunc) 0,   // nb_hex

  // added in release 2.0

  (binaryfunc) Angle_nb_inplace_add,
  (binaryfunc) Angle_nb_inplace_subtract,
  (binaryfunc) Angle_nb_inplace_multiply,
  (binaryfunc) Angle_nb_inplace_divide,
  (binaryfunc) 0,  // nb_inplace_remainder
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  // added in release 2.2
  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) 0,  // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

};


PyTypeObject AngleType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "angle",                                  /* tp_name */
  sizeof(Angle),                     /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) Angle_dealloc,        /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  Angle_repr,                        /* tp_repr */
  &Angle_as_number,                  /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  Angle_str,                         /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES, /* tp_flags */
  "Angle objects",                   /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  Angle_tp_richcompare,              /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  Angle_methods,                     /* tp_methods */
  Angle_members,                     /* tp_members */
  Angle_getseters,                   /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)Angle_init,              /* tp_init */
  0,                                        /* tp_alloc */
  Angle_new,                         /* tp_new */
};


// Create new objects with PyObject_New() for binary operators that
// return a new instance of Angle, like add.
static void new_AngleType(Angle** an_angle) {
  *an_angle = PyObject_New(Angle, &AngleType);
}

static int is_AngleType(PyObject* an_angle) {
  //wrapper for type check
  return PyObject_TypeCheck(an_angle, &AngleType);
}

// --------------------------
// ----- module methods -----
// --------------------------


// -----------------------
// ----- method list -----
// -----------------------

PyMethodDef coords_module_methods[] = {
  {NULL, NULL}  /* Sentinel */
};


// ------------------------------
// ----- init coords module -----
// ------------------------------

// PyMODINIT_FUNC declares extern "C" too.
PyMODINIT_FUNC initcoords(void) {

  PyObject* m(Py_InitModule3("coords", coords_module_methods, "python wrappers for coords objects."));

  // error
  char eMsgStr[] = "coords.Error";
  sCoordsException = PyErr_NewException(eMsgStr, NULL, NULL);
  Py_INCREF(sCoordsException);
  PyModule_AddObject(m, "Error", sCoordsException);



  AngleType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&AngleType) < 0)
    return;
  Py_INCREF(&AngleType);
  PyModule_AddObject(m, "angle", (PyObject *)&AngleType);












}


