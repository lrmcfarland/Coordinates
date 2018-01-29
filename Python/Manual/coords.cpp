// ==========================================================
// Filename:    coords.cpp
//
// Description: Contains the python wrappers for the Coordinates
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
#include <datetime.h>
#include <spherical.h>


#if PY_MAJOR_VERSION >= 3

#define COORDS_INT_CHECK PyLong_Check
#define COORDS_STR_CHECK PyMapping_Check
#define COORDS_STR_FROM_STR PyUnicode_FromString

#define COORDS_TPFLAGS Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE // coersion implicit in python3?

#define COORDS_STR_AS_STR(arg) PyBytes_AS_STRING(PyUnicode_AsEncodedString(arg, "utf-8", "Error encoding string"))


#define MOD_ERROR_VAL NULL
#define MOD_SUCCESS_VAL(val) val
#define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
#define MOD_DEF(ob, name, doc, methods) \
  static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
  ob = PyModule_Create(&moduledef);


#else

#define COORDS_INT_CHECK PyInt_Check
#define COORDS_STR_CHECK PyString_Check
#define COORDS_STR_FROM_STR PyString_FromString

#define COORDS_TPFLAGS Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES

#define COORDS_STR_AS_STR(arg) PyString_AsString(arg)

#define MOD_ERROR_VAL
#define MOD_SUCCESS_VAL(val)
#define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
#define MOD_DEF(ob, name, doc, methods) ob = Py_InitModule3(name, methods, doc);

// PyMODINIT_FUNC declares extern "C" too.

#endif



// ===================
// ===== statics =====
// ===================

static PyObject* sCoordsException; // exception holder

// TODO: make precision configuralble on build, not hardcoded.
static const unsigned int sPrintPrecision(12); // matches defaut %s precision for unit test

// -----------------
// ----- Angle -----
// -----------------

static char sDegreeStr[] = "degees";
static char sMinuteStr[] = "minutes";
static char sSecondStr[] = "seconds";

static char sValueStr[] = "value";
static char sRadiansStr[] = "radians";

typedef struct {
  PyObject_HEAD
  Coords::angle m_angle;
} Angle;

// Forward declarations for as_number methods. Wraps Type definition.
static void new_AngleType(Angle** an_angle);
static int is_AngleType(PyObject* an_angle);

// --------------------
// ----- Latitude -----
// --------------------

typedef struct {
  PyObject_HEAD
  Coords::Latitude m_angle;
} Latitude;

static void new_LatitudeType(Latitude** an_angle);
static int is_LatitudeType(PyObject* an_angle);

// -----------------------
// ----- Declination -----
// -----------------------

// typedef of Latitude

typedef struct {
  PyObject_HEAD
  Coords::Declination m_angle;
} Declination;

static void new_DeclinationType(Declination** an_angle);
static int is_DeclinationType(PyObject* an_angle);

// ---------------------
// ----- Cartesian -----
// ---------------------

static char sXstr[] = "x";
static char sYstr[] = "y";
static char sZstr[] = "z";

typedef struct {
  PyObject_HEAD
  Coords::Cartesian m_Cartesian;
} Cartesian;

// Forward declarations for as_number methods. Wraps CartesianType definition.
static void new_CartesianType(Cartesian** a_Cartesian);
static int is_CartesianType(PyObject* a_Cartesian);

static PyObject* Cartesian_normalized(PyObject* self, PyObject *args);

// -------------------
// ----- rotator -----
// -------------------

static char sAxisStr[] = "axis";

typedef struct {
  PyObject_HEAD
  Coords::rotator m_rotator;
} rotator;

// Forward declarations for as_number methods. Wraps RotatorType definition.
static void new_rotatorType(rotator** a_Rotator);
static int is_rotatorType(PyObject* a_Rotator);

// ---------------------
// ----- spherical -----
// ---------------------

static char sRstr[] = "r";
static char sThetaStr[] = "theta";
static char sPhiStr[] = "phi";

typedef struct {
  PyObject_HEAD
  Coords::spherical m_spherical;
} spherical;

// Forward declarations for as_number methods. Wraps sphericalType definition.
static void new_sphericalType(spherical** a_spherical);
static int is_sphericalType(PyObject* a_spherical);


// --------------------
// ----- datetime -----
// --------------------

static char sYearStr[]  = "year";
static char sMonthStr[] = "month";
static char sDayStr[]   = "day";
static char sHourStr[]  = "hour";
static char sTimeZoneStr[]  = "timezone";
static char sUTStr[]  = "UT";

static char sJulianDateStr[]  = "JulianDate";
static char sLilianDateStr[]  = "LilianDate";
static char sModifiedJulianDateStr[]  = "ModifiedJulianDate";
static char sTruncatedJulianDateStr[]  = "TruncatedJulianDate";
static char sJ2000Str[]  = "J2000";

// reuse minutes, seconds above

// object definition.
typedef struct {
  PyObject_HEAD
  Coords::DateTime m_datetime;
} datetime;

// Forward declarations for as_number methods. Wraps datetimeType definition.
static void new_datetimeType(datetime** a_datetime);
static int is_datetimeType(PyObject* a_datetime);


// ---------------------
// ----- utilities -----
// ---------------------

// helper functions for parsing numeric arguments. Allows arg to be
// double or int.

int parse_int_arg(PyObject* arg, int& val) {

  if (arg) {

    if (PyFloat_Check(arg) || COORDS_INT_CHECK(arg)) {
      val = PyFloat_AsDouble(arg);
      return 0;

    } else if (COORDS_STR_CHECK(arg)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg must be an int or float");
      return -1;
    }

  }

  return 0; // fall through to leave default val unchanged
}

int parse_double_arg(PyObject* arg, double& val) {

  if (arg) {

    if (PyFloat_Check(arg) || COORDS_INT_CHECK(arg)) {
      val = PyFloat_AsDouble(arg);
      return 0;

    } else if (COORDS_STR_CHECK(arg)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg must be an int or float");
      return -1;
    }

  }

  return 0; // fall through to leave default val unchanged
}


// =================
// ===== Angle =====
// =================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* Angle_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  Angle* self(NULL);
  self = (Angle*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int Angle_init(Angle* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sDegreeStr, sMinuteStr, sSecondStr, NULL};

  double degrees(0); // default value
  double minutes(0); // default value
  double seconds(0); // default value

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &arg0, &arg1, &arg2))
    return -1;

  if (arg0) {

    // copy constructor
    if (is_AngleType(arg0)) {
      self->m_angle.value(((Angle*)arg0)->m_angle.value());
      return 0;

    } else if (PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0)) {
      degrees = PyFloat_AsDouble(arg0);

    } else if (COORDS_STR_CHECK(arg0)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be an angle, int or float");
      return -1;
    }

  }

  if (parse_double_arg(arg1, minutes))
      return -1;

  if (parse_double_arg(arg2, seconds))
      return -1;

  self->m_angle.value(Coords::degrees2seconds(degrees, minutes, seconds)/3600);

  return 0;

}


static void Angle_dealloc(Angle* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* Angle_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Angle*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* Angle_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Angle*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
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
    PyErr_SetString(sCoordsException, "can not delete value");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "value must be a float or int");
    return -1;
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
    PyErr_SetString(sCoordsException, "can not delete radians");
    return -1;
  }

  if (!PyFloat_Check(radians) && !COORDS_INT_CHECK(radians)) {
    PyErr_SetString(sCoordsException, "radians must be a float or int");
    return -1;
  }

  self->m_angle.radians(PyFloat_AsDouble(radians));

  return 0;
}

// -------------------
// ----- methods -----
// -------------------

static PyObject* normalize(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1)) {
    // TODO coords.Error?
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  // TODO kwlist?

  double begin(0);
  double end(0);

  if (!PyArg_ParseTuple(o2, "dd", &begin, &end))
    return NULL;

  ((Angle*)o1)->m_angle.normalize(begin, end);

  Py_INCREF(Py_None);
  return Py_None;

}

static PyObject* complement(PyObject* o1) {

  if (!is_AngleType(o1)) {
    // TODO coords.Error?
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.angle");
    return NULL;
  }

  result_angle->m_angle = ((Angle*)o1)->m_angle.complement();

  return (PyObject*) result_angle;
}

// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* Angle_nb_add(PyObject* o1, PyObject* o2) {

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    // TODO coords.Error?
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_sum(((Angle*)o1)->m_angle + ((Angle*)o2)->m_angle);

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
    PyErr_SetString(sCoordsException, "subtract failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_difference(((Angle*)o1)->m_angle - ((Angle*)o2)->m_angle);

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
    PyErr_SetString(sCoordsException, "negative failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_inverse = -((Angle*)o1)->m_angle;

  result_angle->m_angle = the_inverse;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_multiply(PyObject* o1, PyObject* o2) {

  // TODO support o2 as double?

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create coord.angle");
    return NULL;
  }

  result_angle->m_angle = ((Angle*)o1)->m_angle * ((Angle*)o2)->m_angle;

  return (PyObject*) result_angle;
}


static PyObject* Angle_nb_divide(PyObject* o1, PyObject* o2) {

  // TODO support o2 as double?

  if (!is_AngleType(o1) || !is_AngleType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL);
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create coord.angle");
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
  // TODO o1 is implicitly always angleType?
  // TODO support o2 as double?
  if (is_AngleType(o1) && is_AngleType(o2)) {
    ((Angle*)o1)->m_angle.operator+=(((Angle*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "angle::operator+=() only supports angle types");
  return NULL;
}

static PyObject* Angle_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  // TODO o1 is implicitly always angleType?
  // TODO support o2 as double?
  if (is_AngleType(o1) && is_AngleType(o2)) {
    ((Angle*)o1)->m_angle.operator-=(((Angle*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "angle::operator-=() only supports angle types");
  return NULL;
}

static PyObject* Angle_nb_inplace_multiply(PyObject* o1, PyObject* o2) {
  // TODO o1 is implicitly always angleType?
  // TODO support o2 as double?
  if (is_AngleType(o1) && is_AngleType(o2)) {
    ((Angle*)o1)->m_angle.operator*=(((Angle*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "angle::operator*=() only supports angle types");
  return NULL;
}

static PyObject* Angle_nb_inplace_divide(PyObject* o1, PyObject* o2) {
  // TODO o1 is implicitly always angleType?
  // TODO support o2 as double?
  if (is_AngleType(o1) && is_AngleType(o2)) {
    ((Angle*)o1)->m_angle.operator/=(((Angle*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "angle::operator/=() only supports angle types");
  return NULL;
}


// ----------------------------
// ----- instance methods -----
// ----------------------------

// TODO as module methods?

// ----- deg2rad -----

static PyObject* deg2rad(PyObject* self, PyObject *args) {
  double radians(0);
  if (!PyArg_ParseTuple(args, "d", &radians))
    return NULL;
  double degrees = Coords::angle::deg2rad(radians);
  return (PyObject*)  Py_BuildValue("d", degrees);
}

// ----- rad2deg -----

static PyObject* rad2deg(PyObject* self, PyObject *args) {
  double radians(0);
  if (!PyArg_ParseTuple(args, "d", &radians))
    return NULL;
  double degrees = Coords::angle::rad2deg(radians);
  return (PyObject*)  Py_BuildValue("d", degrees);
}

// --------------------------
// ----- Python structs -----
// --------------------------

PyDoc_STRVAR(coords_deg2rad__doc__, "converts degrees into radians");
PyDoc_STRVAR(coords_rad2deg__doc__, "converts radians into degrees");
PyDoc_STRVAR(coords_normalize__doc__, "normalizes the value to given range");
PyDoc_STRVAR(coords_complement__doc__, "returns the complement of the angle");

static PyMethodDef Angle_methods[] = {
  {"deg2rad", (PyCFunction) deg2rad, METH_VARARGS, coords_deg2rad__doc__},
  {"rad2deg", (PyCFunction) rad2deg, METH_VARARGS, coords_rad2deg__doc__},
  {"normalize", (PyCFunction) normalize, METH_VARARGS, coords_normalize__doc__},
  {"complement", (PyCFunction) complement, METH_VARARGS, coords_complement__doc__},
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


#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods Angle_as_number = {
  (binaryfunc) Angle_nb_add,
  (binaryfunc) Angle_nb_subtract,
  (binaryfunc) Angle_nb_multiply,
  (binaryfunc) 0,   // nb_remainder
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

  (unaryfunc) 0,   // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,   // nb_float

  (binaryfunc) Angle_nb_inplace_add,
  (binaryfunc) Angle_nb_inplace_subtract,
  (binaryfunc) Angle_nb_inplace_multiply,
  (binaryfunc) Angle_nb_inplace_divide,
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) Angle_nb_divide, // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};

#else

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

#endif


PyTypeObject AngleType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "angle",                           /* tp_name */
  sizeof(Angle),                     /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor) Angle_dealloc,        /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  Angle_repr,                        /* tp_repr */
  &Angle_as_number,                  /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  Angle_str,                         /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "angle objects",                   /* tp_doc */
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  Angle_tp_richcompare,              /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  Angle_methods,                     /* tp_methods */
  Angle_members,                     /* tp_members */
  Angle_getseters,                   /* tp_getset */
  0,                                 /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)Angle_init,              /* tp_init */
  0,                                 /* tp_alloc */
  Angle_new,                         /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_AngleType(Angle** an_angle) {
  *an_angle = PyObject_New(Angle, &AngleType);
}

static int is_AngleType(PyObject* an_angle) {
  //wrapper for type check
  return PyObject_TypeCheck(an_angle, &AngleType);
}


// ====================
// ===== Latitude =====
// ====================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* Latitude_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  Latitude* self(NULL);
  self = (Latitude*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int Latitude_init(Latitude* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sDegreeStr, sMinuteStr, sSecondStr, NULL};

  double degrees(0); // default value
  double minutes(0); // default value
  double seconds(0); // default value

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &arg0, &arg1, &arg2))
    return -1;

  if (arg0) {

    // copy constructor
    if (is_LatitudeType(arg0)) {
      self->m_angle.value(((Latitude*)arg0)->m_angle.value());
      return 0;

    } else if (PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0)) {
      degrees = PyFloat_AsDouble(arg0);

    } else if (COORDS_STR_CHECK(arg0)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be an int or float");
      return -1;
    }

  }

  if (parse_double_arg(arg1, minutes))
      return -1;

  if (parse_double_arg(arg2, seconds))
      return -1;

  self->m_angle.value(Coords::degrees2seconds(degrees, minutes, seconds)/3600);

  return 0;

}


static void Latitude_dealloc(Latitude* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* Latitude_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Latitude*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* Latitude_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Latitude*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- value -----

static PyObject* Latitude_getValue(Latitude* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.value());
}

static int Latitude_setValue(Latitude* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete value");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "value must be a float or int");
    return -1;
  }

  self->m_angle.value(PyFloat_AsDouble(value));

  return 0;
}

// ----- radians -----

static PyObject* Latitude_getRadians(Latitude* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.radians());
}

static int Latitude_setRadians(Latitude* self, PyObject* radians, void* closure) {

  if (radians == NULL) {
    PyErr_SetString(sCoordsException, "can not delete radians");
    return -1;
  }

  if (!PyFloat_Check(radians) && !COORDS_INT_CHECK(radians)) {
    PyErr_SetString(sCoordsException, "radians must be a float or int");
    return -1;
  }

  self->m_angle.radians(PyFloat_AsDouble(radians));

  return 0;
}

// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* Latitude_nb_add(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_LatitudeType(o1) || !is_LatitudeType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_sum(((Latitude*)o1)->m_angle + ((Latitude*)o2)->m_angle);

  result_angle->m_angle = the_sum;

  return (PyObject*) result_angle;
}


static PyObject* Latitude_nb_subtract(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_LatitudeType(o1) || !is_LatitudeType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_difference(((Latitude*)o1)->m_angle - ((Latitude*)o2)->m_angle);

  result_angle->m_angle = the_difference;

  return (PyObject*) result_angle;
}


static PyObject* Latitude_nb_negative(PyObject* o1) {
  // Unitary minus

  if (!is_LatitudeType(o1)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "negative failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_inverse = -((Latitude*)o1)->m_angle;

  result_angle->m_angle = the_inverse;

  return (PyObject*) result_angle;
}


static PyObject* Latitude_nb_multiply(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_LatitudeType(o1) || !is_LatitudeType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create coord.angle");
    return NULL;
  }

  result_angle->m_angle = ((Latitude*)o1)->m_angle * ((Latitude*)o2)->m_angle;

  return (PyObject*) result_angle;
}


static PyObject* Latitude_nb_divide(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_LatitudeType(o1) || !is_LatitudeType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create coord.angle");
    return NULL;
  }

  try {
    result_angle->m_angle = ((Latitude*)o1)->m_angle / ((Latitude*)o2)->m_angle;
  } catch (Coords::Error err) {
    PyErr_SetString(sCoordsException, err.what());
    return NULL;
  }

  return (PyObject*) result_angle;
}


static PyObject* Latitude_tp_richcompare(PyObject* o1, PyObject* o2, int op) {

  if (!is_LatitudeType(o1) || !is_LatitudeType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  if (op == Py_LT) {

    if (((Latitude*)o1)->m_angle < ((Latitude*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_LE) {

    if (((Latitude*)o1)->m_angle <= ((Latitude*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_EQ) {

    if (((Latitude*)o1)->m_angle == ((Latitude*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_NE) {

    if (((Latitude*)o1)->m_angle != ((Latitude*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GT) {

    if (((Latitude*)o1)->m_angle > ((Latitude*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GE) {

    if (((Latitude*)o1)->m_angle >= ((Latitude*)o2)->m_angle)
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

static PyObject* Latitude_nb_inplace_add(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_LatitudeType(o1) && is_LatitudeType(o2)) {
    ((Latitude*)o1)->m_angle.operator+=(((Latitude*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Latitude::operator+=() only supports Latitude types");
  return NULL;
}

static PyObject* Latitude_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_LatitudeType(o1) && is_LatitudeType(o2)) {
    ((Latitude*)o1)->m_angle.operator-=(((Latitude*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Latitude::operator-=() only supports Latitude types");
  return NULL;
}

static PyObject* Latitude_nb_inplace_multiply(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_LatitudeType(o1) && is_LatitudeType(o2)) {
    ((Latitude*)o1)->m_angle.operator*=(((Latitude*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Latitude::operator*=() only supports Latitude types");
  return NULL;
}

static PyObject* Latitude_nb_inplace_divide(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_LatitudeType(o1) && is_LatitudeType(o2)) {
    ((Latitude*)o1)->m_angle.operator/=(((Latitude*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Latitude::operator/=() only supports Latitude types");
  return NULL;
}

// --------------------------
// ----- Python structs -----
// --------------------------

static PyMethodDef Latitude_methods[] = {
  {NULL}  /* Sentinel */
};


static PyMemberDef Latitude_members[] = {
  {NULL}  /* Sentinel */
};

static PyGetSetDef Latitude_getseters[] = {
  {sValueStr, (getter)Latitude_getValue, (setter)Latitude_setValue, sValueStr, NULL},
  {sRadiansStr, (getter)Latitude_getRadians, (setter)Latitude_setRadians, sRadiansStr, NULL},
  {NULL}  /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods Latitude_as_number = {
  (binaryfunc) Latitude_nb_add,
  (binaryfunc) Latitude_nb_subtract,
  (binaryfunc) Latitude_nb_multiply,
  (binaryfunc) 0, // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Latitude_nb_negative,
  (unaryfunc) 0,   // nb_positive
  (unaryfunc) 0,   // nb_absolute
  (inquiry) 0,     // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,   // nb_invert
  (binaryfunc) 0,  // nb_lshift
  (binaryfunc) 0,  // nb_rshift
  (binaryfunc) 0,  // nb_and
  (binaryfunc) 0,  // nb_xor
  (binaryfunc) 0,  // nb_or

  (unaryfunc) 0,   // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,   // nb_float

  (binaryfunc) Latitude_nb_inplace_add,
  (binaryfunc) Latitude_nb_inplace_subtract,
  (binaryfunc) Latitude_nb_inplace_multiply,
  (binaryfunc) Latitude_nb_inplace_divide,
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) Latitude_nb_divide, // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};


#else

// see http://docs.python.org/c-api/typeobj.html
static PyNumberMethods Latitude_as_number = {
  (binaryfunc) Latitude_nb_add,
  (binaryfunc) Latitude_nb_subtract,
  (binaryfunc) Latitude_nb_multiply,
  (binaryfunc) Latitude_nb_divide,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Latitude_nb_negative,
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

  (binaryfunc) Latitude_nb_inplace_add,
  (binaryfunc) Latitude_nb_inplace_subtract,
  (binaryfunc) Latitude_nb_inplace_multiply,
  (binaryfunc) Latitude_nb_inplace_divide,
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

#endif


PyTypeObject LatitudeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "Latitude",                           /* tp_name */
  sizeof(Latitude),                     /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor) Latitude_dealloc,        /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  Latitude_repr,                        /* tp_repr */
  &Latitude_as_number,                  /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  Latitude_str,                         /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "Latitude objects",                   /* tp_doc */
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  Latitude_tp_richcompare,              /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  Latitude_methods,                     /* tp_methods */
  Latitude_members,                     /* tp_members */
  Latitude_getseters,                   /* tp_getset */
  0,                                 /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)Latitude_init,              /* tp_init */
  0,                                 /* tp_alloc */
  Latitude_new,                         /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_LatitudeType(Latitude** an_angle) {
  *an_angle = PyObject_New(Latitude, &LatitudeType);
}

static int is_LatitudeType(PyObject* an_angle) {
  //wrapper for type check
  return PyObject_TypeCheck(an_angle, &LatitudeType);
}


// =======================
// ===== Declination =====
// =======================

// typedef of Latitude

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* Declination_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  Declination* self(NULL);
  self = (Declination*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int Declination_init(Declination* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sDegreeStr, sMinuteStr, sSecondStr, NULL};

  double degrees(0); // default value
  double minutes(0); // default value
  double seconds(0); // default value

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &arg0, &arg1, &arg2))
    return -1;

  if (arg0) {

    // copy constructor
    if (is_DeclinationType(arg0)) {
      self->m_angle.value(((Declination*)arg0)->m_angle.value());
      return 0;

    } else if (PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0)) {
      degrees = PyFloat_AsDouble(arg0);

    } else if (COORDS_STR_CHECK(arg0)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be an int or float");
      return -1;
    }

  }

  if (parse_double_arg(arg1, minutes))
      return -1;

  if (parse_double_arg(arg2, seconds))
      return -1;

  self->m_angle.value(Coords::degrees2seconds(degrees, minutes, seconds)/3600);

  return 0;

}


static void Declination_dealloc(Declination* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* Declination_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Declination*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* Declination_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Declination*)self)->m_angle;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- value -----

static PyObject* Declination_getValue(Declination* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.value());
}

static int Declination_setValue(Declination* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete value");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "value must be a float or int");
    return -1;
  }

  self->m_angle.value(PyFloat_AsDouble(value));

  return 0;
}

// ----- radians -----

static PyObject* Declination_getRadians(Declination* self, void* closure) {
  return PyFloat_FromDouble(self->m_angle.radians());
}

static int Declination_setRadians(Declination* self, PyObject* radians, void* closure) {

  if (radians == NULL) {
    PyErr_SetString(sCoordsException, "can not delete radians");
    return -1;
  }

  if (!PyFloat_Check(radians) && !COORDS_INT_CHECK(radians)) {
    PyErr_SetString(sCoordsException, "radians must be a float or int");
    return -1;
  }

  self->m_angle.radians(PyFloat_AsDouble(radians));

  return 0;
}

// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* Declination_nb_add(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_DeclinationType(o1) || !is_DeclinationType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_sum(((Declination*)o1)->m_angle + ((Declination*)o2)->m_angle);

  result_angle->m_angle = the_sum;

  return (PyObject*) result_angle;
}


static PyObject* Declination_nb_subtract(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_DeclinationType(o1) || !is_DeclinationType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_difference(((Declination*)o1)->m_angle - ((Declination*)o2)->m_angle);

  result_angle->m_angle = the_difference;

  return (PyObject*) result_angle;
}


static PyObject* Declination_nb_negative(PyObject* o1) {
  // Unitary minus

  if (!is_DeclinationType(o1)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle

  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "negative failed to create coord.angle");
    return NULL;
  }

  Coords::angle the_inverse = -((Declination*)o1)->m_angle;

  result_angle->m_angle = the_inverse;

  return (PyObject*) result_angle;
}


static PyObject* Declination_nb_multiply(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_DeclinationType(o1) || !is_DeclinationType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create coord.angle");
    return NULL;
  }

  result_angle->m_angle = ((Declination*)o1)->m_angle * ((Declination*)o2)->m_angle;

  return (PyObject*) result_angle;
}


static PyObject* Declination_nb_divide(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?

  if (!is_DeclinationType(o1) || !is_DeclinationType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Angle* result_angle(NULL); // switches to Angle
  new_AngleType(&result_angle);

  if (result_angle == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create coord.angle");
    return NULL;
  }

  try {
    result_angle->m_angle = ((Declination*)o1)->m_angle / ((Declination*)o2)->m_angle;
  } catch (Coords::Error err) {
    PyErr_SetString(sCoordsException, err.what());
    return NULL;
  }

  return (PyObject*) result_angle;
}


static PyObject* Declination_tp_richcompare(PyObject* o1, PyObject* o2, int op) {

  if (!is_DeclinationType(o1) || !is_DeclinationType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  if (op == Py_LT) {

    if (((Declination*)o1)->m_angle < ((Declination*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_LE) {

    if (((Declination*)o1)->m_angle <= ((Declination*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_EQ) {

    if (((Declination*)o1)->m_angle == ((Declination*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_NE) {

    if (((Declination*)o1)->m_angle != ((Declination*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GT) {

    if (((Declination*)o1)->m_angle > ((Declination*)o2)->m_angle)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_GE) {

    if (((Declination*)o1)->m_angle >= ((Declination*)o2)->m_angle)
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

static PyObject* Declination_nb_inplace_add(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_DeclinationType(o1) && is_DeclinationType(o2)) {
    ((Declination*)o1)->m_angle.operator+=(((Declination*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Declination::operator+=() only supports Declination types");
  return NULL;
}

static PyObject* Declination_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_DeclinationType(o1) && is_DeclinationType(o2)) {
    ((Declination*)o1)->m_angle.operator-=(((Declination*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Declination::operator-=() only supports Declination types");
  return NULL;
}

static PyObject* Declination_nb_inplace_multiply(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_DeclinationType(o1) && is_DeclinationType(o2)) {
    ((Declination*)o1)->m_angle.operator*=(((Declination*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Declination::operator*=() only supports Declination types");
  return NULL;
}

static PyObject* Declination_nb_inplace_divide(PyObject* o1, PyObject* o2) {
  // TODO support o2 as angle? double?
  if (is_DeclinationType(o1) && is_DeclinationType(o2)) {
    ((Declination*)o1)->m_angle.operator/=(((Declination*)o2)->m_angle);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Declination::operator/=() only supports Declination types");
  return NULL;
}

// --------------------------
// ----- Python structs -----
// --------------------------

static PyMethodDef Declination_methods[] = {
  {NULL}  /* Sentinel */
};


static PyMemberDef Declination_members[] = {
  {NULL}  /* Sentinel */
};

static PyGetSetDef Declination_getseters[] = {
  {sValueStr, (getter)Declination_getValue, (setter)Declination_setValue, sValueStr, NULL},
  {sRadiansStr, (getter)Declination_getRadians, (setter)Declination_setRadians, sRadiansStr, NULL},
  {NULL}  /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods Declination_as_number = {
  (binaryfunc) Declination_nb_add,
  (binaryfunc) Declination_nb_subtract,
  (binaryfunc) Declination_nb_multiply,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Declination_nb_negative,
  (unaryfunc) 0,   // nb_positive
  (unaryfunc) 0,   // nb_absolute
  (inquiry) 0,     // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,   // nb_invert
  (binaryfunc) 0,  // nb_lshift
  (binaryfunc) 0,  // nb_rshift
  (binaryfunc) 0,  // nb_and
  (binaryfunc) 0,  // nb_xor
  (binaryfunc) 0,  // nb_or

  (unaryfunc) 0,   // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,   // nb_float

  (binaryfunc) Declination_nb_inplace_add,
  (binaryfunc) Declination_nb_inplace_subtract,
  (binaryfunc) Declination_nb_inplace_multiply,
  (binaryfunc) Declination_nb_inplace_divide,  // nb_inplace_remainder
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) Declination_nb_divide, // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};

#else

// see http://docs.python.org/c-api/typeobj.html

static PyNumberMethods Declination_as_number = {
  (binaryfunc) Declination_nb_add,
  (binaryfunc) Declination_nb_subtract,
  (binaryfunc) Declination_nb_multiply,
  (binaryfunc) Declination_nb_divide,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Declination_nb_negative,
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

  (binaryfunc) Declination_nb_inplace_add,
  (binaryfunc) Declination_nb_inplace_subtract,
  (binaryfunc) Declination_nb_inplace_multiply,
  (binaryfunc) Declination_nb_inplace_divide,
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

#endif


PyTypeObject DeclinationType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "Declination",                           /* tp_name */
  sizeof(Declination),                     /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor) Declination_dealloc,        /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  Declination_repr,                        /* tp_repr */
  &Declination_as_number,                  /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  Declination_str,                         /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "Declination objects",                   /* tp_doc */
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  Declination_tp_richcompare,              /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  Declination_methods,                     /* tp_methods */
  Declination_members,                     /* tp_members */
  Declination_getseters,                   /* tp_getset */
  0,                                 /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)Declination_init,              /* tp_init */
  0,                                 /* tp_alloc */
  Declination_new,                         /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_DeclinationType(Declination** an_angle) {
  *an_angle = PyObject_New(Declination, &DeclinationType);
}

static int is_DeclinationType(PyObject* an_angle) {
  //wrapper for type check
  return PyObject_TypeCheck(an_angle, &DeclinationType);
}


// =====================
// ===== Cartesian =====
// =====================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* Cartesian_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  Cartesian* self(NULL);
  self = (Cartesian*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int Cartesian_init(Cartesian* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sXstr, sYstr, sZstr, NULL};

  double x(0); // default value
  double y(0); // default value
  double z(0); // default value

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);


  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &arg0, &arg1, &arg2))
    return -1;

  if (arg0) {

    if (is_CartesianType(arg0)) {
      // copy constructor
      self->m_Cartesian.x(((Cartesian*)arg0)->m_Cartesian.x());
      self->m_Cartesian.y(((Cartesian*)arg0)->m_Cartesian.y());
      self->m_Cartesian.z(((Cartesian*)arg0)->m_Cartesian.z());
      return 0;

    } else if (is_sphericalType(arg0)) {
      // spherical conversion constructor
      Coords::Cartesian from_spherical(((spherical*)arg0)->m_spherical);
      self->m_Cartesian.x(from_spherical.x());
      self->m_Cartesian.y(from_spherical.y());
      self->m_Cartesian.z(from_spherical.z());
      return 0;

    } else if (PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0)) {
      x = PyFloat_AsDouble(arg0);

    } else if (COORDS_STR_CHECK(arg0)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be a coords.Cartesian, coords.spherical, int or float");
      return -1;
    }

  }

  if (parse_double_arg(arg1, y))
      return -1;

  if (parse_double_arg(arg2, z))
      return -1;

  self->m_Cartesian.x(x);
  self->m_Cartesian.y(y);
  self->m_Cartesian.z(z);

  return 0;

}

static void Cartesian_dealloc(Cartesian* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

// TODO switch repr and str so repr is xml and str is (x, y, z)?

PyObject* Cartesian_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((Cartesian*)self)->m_Cartesian;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

PyObject* Cartesian_repr(PyObject* self) {
  const Coords::Cartesian& a_Cartesian(((Cartesian*)self)->m_Cartesian);
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << "("
	 << a_Cartesian.x() << ", "
	 << a_Cartesian.y() << ", "
	 << a_Cartesian.z() << ")";
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- X -----

static PyObject* Cartesian_getx(Cartesian* self, void* closure) {
  return PyFloat_FromDouble(self->m_Cartesian.x());
}

static int Cartesian_setx(Cartesian* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete x");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "x must be a float or int");
    return -1;
  }

  self->m_Cartesian.x(PyFloat_AsDouble(value));

  return 0;
}

// ----- Y -----

static PyObject* Cartesian_gety(Cartesian* self, void* closure) {
  return PyFloat_FromDouble(self->m_Cartesian.y());
}

static int Cartesian_sety(Cartesian* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete y");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "y must be a float or int");
    return -1;
  }

  self->m_Cartesian.y(PyFloat_AsDouble(value));

  return 0;
}

// ----- Z -----

static PyObject* Cartesian_getz(Cartesian* self, void* closure) {
  return PyFloat_FromDouble(self->m_Cartesian.z());
}

static int Cartesian_setz(Cartesian* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete z");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "z must be a float or int");
    return -1;
  }

  self->m_Cartesian.z(PyFloat_AsDouble(value));

  return 0;
}

// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* Cartesian_nb_add(PyObject* o1, PyObject* o2) {

  if (!is_CartesianType(o1) || !is_CartesianType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Cartesian* result_Cartesian(NULL);

  new_CartesianType(&result_Cartesian);

  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.Cartesian");
    return NULL;
  }

  Coords::Cartesian the_sum(((Cartesian*)o1)->m_Cartesian + ((Cartesian*)o2)->m_Cartesian);

  result_Cartesian->m_Cartesian = the_sum;

  return (PyObject*) result_Cartesian;

}


static PyObject* Cartesian_nb_subtract(PyObject* o1, PyObject* o2) {

  if (!is_CartesianType(o1) || !is_CartesianType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Cartesian* result_Cartesian(NULL);

  new_CartesianType(&result_Cartesian);

  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create coord.Cartesian");
    return NULL;
  }

  Coords::Cartesian the_difference(((Cartesian*)o1)->m_Cartesian - ((Cartesian*)o2)->m_Cartesian);

  result_Cartesian->m_Cartesian = the_difference;

  return (PyObject*) result_Cartesian;

}


static PyObject* Cartesian_nb_negative(PyObject* o1) {
  // Unitary minus

  if (!is_CartesianType(o1)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Cartesian* result_Cartesian(NULL);

  new_CartesianType(&result_Cartesian);

  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "unitary minus failed to create coord.Cartesian");
    return NULL;
  }

  Coords::Cartesian the_inverse = -((Cartesian*)o1)->m_Cartesian;

  result_Cartesian->m_Cartesian = the_inverse;

  return (PyObject*) result_Cartesian;
}


static PyObject* Cartesian_nb_multiply(PyObject* o1, PyObject* o2) {

  // This returns the dot product of the Cartesian vectors as a double
  // if o1 and o2 are both Cartesian. Returns a scaled version of o1
  // or o2 if one is Cartesian and the other double or int. Returns
  // not implemented otherwise.

  if (is_CartesianType(o1) && is_CartesianType(o2)) {
    double a_dot_product(((Cartesian*)o1)->m_Cartesian * ((Cartesian*)o2)->m_Cartesian);
    return Py_BuildValue("d", a_dot_product);
  }

  Cartesian* result_Cartesian(NULL);
  new_CartesianType(&result_Cartesian);

  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create coord.Cartesian");
    return NULL;
  }

  if (is_CartesianType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    result_Cartesian->m_Cartesian = ((Cartesian*)o1)->m_Cartesian * PyFloat_AsDouble(o2);
    return (PyObject*) result_Cartesian;
  }

  if ((PyFloat_Check(o1) || COORDS_INT_CHECK(o1)) && is_CartesianType(o2)) {
    result_Cartesian->m_Cartesian = PyFloat_AsDouble(o1) * ((Cartesian*)o2)->m_Cartesian;
    return (PyObject*) result_Cartesian;

  }

  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;

}


static PyObject* Cartesian_nb_divide(PyObject* o1, PyObject* o2) {
  // This returns a Cartesian object scaled by the divisor.  o1 must be
  // CartesianType, o2 a float or int otherwise this will raise a
  // NotImplemented error.

  Cartesian* result_Cartesian(NULL);
  new_CartesianType(&result_Cartesian);

  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create coord.Cartesian");
    return NULL;
  }

  if (is_CartesianType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {

    try {
      result_Cartesian->m_Cartesian = ((Cartesian*)o1)->m_Cartesian / PyFloat_AsDouble(o2);
    } catch (Coords::DivideByZeroError& err) {
      Py_DECREF(result_Cartesian);
      PyErr_SetString(sCoordsException, err.what());
      return NULL;
    }

  } else {
    Py_DECREF(result_Cartesian);
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  return (PyObject*) result_Cartesian;
}


static PyObject* Cartesian_tp_richcompare(PyObject* o1, PyObject* o2, int op) {

  if (!is_CartesianType(o1) || !is_CartesianType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  if (op == Py_EQ) {

    if (((Cartesian*)o1)->m_Cartesian == ((Cartesian*)o2)->m_Cartesian)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_NE) {

    if (((Cartesian*)o1)->m_Cartesian != ((Cartesian*)o2)->m_Cartesian)
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

static PyObject* Cartesian_nb_inplace_add(PyObject* o1, PyObject* o2) {
  if (is_CartesianType(o1) && is_CartesianType(o2)) {
    ((Cartesian*)o1)->m_Cartesian.operator+=(((Cartesian*)o2)->m_Cartesian);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Cartesian::operator+=() only supports Cartesian types");
  return NULL;
}

static PyObject* Cartesian_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  if (is_CartesianType(o1) && is_CartesianType(o2)) {
    ((Cartesian*)o1)->m_Cartesian.operator-=(((Cartesian*)o2)->m_Cartesian);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "Cartesian::operator-=() only supports Cartesian types");
  return NULL;
}

static PyObject* Cartesian_nb_inplace_multiply(PyObject* o1, PyObject* o2) {

  // This only scales the vector. If it returned the dot product, it
  // would switch the object to type double.

  if (is_CartesianType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    ((Cartesian*)o1)->m_Cartesian.operator*=(PyFloat_AsDouble(o2));
    Py_INCREF(o1);
    return o1;
  }

  PyErr_SetString(sCoordsException, "Cartesian::operator*=() only supports Cartesian types");
  return NULL;

}

static PyObject* Cartesian_nb_inplace_divide(PyObject* o1, PyObject* o2) {
  // This only scales the vector.

  if (is_CartesianType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    ((Cartesian*)o1)->m_Cartesian.operator/=(PyFloat_AsDouble(o2));
    Py_INCREF(o1);
    return o1;
  }

  PyErr_SetString(sCoordsException, "Cartesian::operator/=() only supports Cartesian types");
  return NULL;

}

// ----------------------------
// ----- instance methods -----
// ----------------------------

// ----- Cartesian_magnitude -----
static PyObject* Cartesian_magnitude(PyObject* self, PyObject *args) {
  double a_magnitude(((Cartesian*)self)->m_Cartesian.magnitude());
  return (PyObject*)  Py_BuildValue("d", a_magnitude);
}

// --------------------------
// ----- Python structs -----
// --------------------------

PyDoc_STRVAR(Cartesian_magnitude__doc__, "Returns the magnitude of the Cartesian object");
PyDoc_STRVAR(Cartesian_normalized__doc__, "Returns the normalized version of the Cartesian object");

static PyMethodDef Cartesian_methods[] = {
  {"magnitude", (PyCFunction) Cartesian_magnitude, METH_VARARGS, Cartesian_magnitude__doc__},
  {"normalized", (PyCFunction) Cartesian_normalized, METH_VARARGS, Cartesian_normalized__doc__},
  {NULL}  /* Sentinel */
};


static PyMemberDef Cartesian_members[] = {
  {NULL}  /* Sentinel */
};


static PyGetSetDef Cartesian_getseters[] = {
  {sXstr, (getter)Cartesian_getx, (setter)Cartesian_setx, sXstr, NULL},
  {sYstr, (getter)Cartesian_gety, (setter)Cartesian_sety, sYstr, NULL},
  {sZstr, (getter)Cartesian_getz, (setter)Cartesian_setz, sZstr, NULL},
  {NULL}  /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods Cartesian_as_number = {
  (binaryfunc) Cartesian_nb_add,
  (binaryfunc) Cartesian_nb_subtract,
  (binaryfunc) Cartesian_nb_multiply,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Cartesian_nb_negative,
  (unaryfunc) 0,   // nb_positive
  (unaryfunc) 0,   // nb_absolute
  (inquiry) 0,     // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,   // nb_invert
  (binaryfunc) 0,  // nb_lshift
  (binaryfunc) 0,  // nb_rshift
  (binaryfunc) 0,  // nb_and
  (binaryfunc) 0,  // nb_xor
  (binaryfunc) 0,  // nb_or

  (unaryfunc) 0,   // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,   // nb_float


  (binaryfunc) Cartesian_nb_inplace_add,
  (binaryfunc) Cartesian_nb_inplace_subtract,
  (binaryfunc) Cartesian_nb_inplace_multiply,
  (binaryfunc) Cartesian_nb_inplace_divide, // nb_inplace_remainder
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) Cartesian_nb_divide, // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};


#else

// see http://docs.python.org/c-api/typeobj.html

static PyNumberMethods Cartesian_as_number = {
  (binaryfunc) Cartesian_nb_add,
  (binaryfunc) Cartesian_nb_subtract,
  (binaryfunc) Cartesian_nb_multiply,
  (binaryfunc) Cartesian_nb_divide,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) Cartesian_nb_negative,
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

  (binaryfunc) Cartesian_nb_inplace_add,
  (binaryfunc) Cartesian_nb_inplace_subtract,
  (binaryfunc) Cartesian_nb_inplace_multiply,
  (binaryfunc) Cartesian_nb_inplace_divide,
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

#endif


PyTypeObject CartesianType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "Cartesian",                              /* tp_name */
  sizeof(Cartesian),                        /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) Cartesian_dealloc,           /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  Cartesian_repr,                           /* tp_repr */
  &Cartesian_as_number,                     /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  Cartesian_str,                            /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "Cartesian objects",                      /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  Cartesian_tp_richcompare,                 /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  Cartesian_methods,                        /* tp_methods */
  Cartesian_members,                        /* tp_members */
  Cartesian_getseters,                      /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)Cartesian_init,                 /* tp_init */
  0,                                        /* tp_alloc */
  Cartesian_new,                            /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_CartesianType(Cartesian** a_Cartesian) {
  *a_Cartesian = PyObject_New(Cartesian, &CartesianType);
}

static int is_CartesianType(PyObject* a_Cartesian) {
  return PyObject_TypeCheck(a_Cartesian, &CartesianType);
}

// ----- Cartesian_normalized -----
static PyObject* Cartesian_normalized(PyObject* self, PyObject *args) {

  Cartesian* result_Cartesian(NULL);
  result_Cartesian = PyObject_New(Cartesian, &CartesianType); // alloc and inits
  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "normalized failed to create Cartesian type");
    return NULL;
  }

  Coords::Cartesian a_normalized(((Cartesian*)self)->m_Cartesian.normalized());

  result_Cartesian->m_Cartesian = a_normalized;

  return (PyObject*) result_Cartesian;

}

// ====================
// ===== rotator =====
// ====================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* rotator_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  rotator* self(NULL);
  self = (rotator*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int rotator_init(rotator* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sAxisStr, NULL};

  PyObject* arg0(NULL);

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &arg0))
    return -1;

  if (arg0) {

    // copy constructor
    if (is_rotatorType(arg0)) {
      self->m_rotator.axis(((rotator*)arg0)->m_rotator.axis());
      return 0;

    } else if (is_CartesianType(arg0)) {
      self->m_rotator.axis(((Cartesian*)arg0)->m_Cartesian);
      return 0;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be a coords.rotator or coords.Cartesian");
      return -1;
    }

  }

  return 0;

}


static void rotator_dealloc(rotator* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* rotator_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((rotator*)self)->m_rotator.axis();
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* rotator_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((rotator*)self)->m_rotator.axis();
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------
// ----- methods -----
// -------------------

static PyObject* rotator_rotate(PyObject* o1, PyObject* o2) {

  // TODO kwlist?

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);

  if (!PyArg_ParseTuple(o2, "OO", &arg0, &arg1))
    return NULL;

  if (!is_rotatorType(o1)) {
    PyErr_SetString(sCoordsException, "not rotator method"); // TODO
    return NULL;
  }

  if (!is_CartesianType(arg0)) {
    PyErr_SetString(sCoordsException, "rotator::rotate() arg0 must be a Cartesian vector");
    return NULL;
  }

  if (!is_AngleType(arg1)) {
    PyErr_SetString(sCoordsException, "rotator::rotate() arg1 must be an angle");
    return NULL;
  }

  Cartesian* result_Cartesian(NULL);
  result_Cartesian = PyObject_New(Cartesian, &CartesianType); // alloc and inits
  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "rotate failed to create coord.Cartesian");
    return NULL;
  }


  if (((rotator*)o1)->m_rotator.axis() == Coords::Cartesian::Uo) {
    PyErr_SetString(sCoordsException, "rotator has Uo rotation axis");
    return NULL;
  }

  result_Cartesian->m_Cartesian = ((rotator*)o1)->m_rotator.rotate(((Cartesian*)arg0)->m_Cartesian,
								   ((Angle*)arg1)->m_angle);

  return (PyObject*) result_Cartesian;

}


// --------------------------
// ----- Python structs -----
// --------------------------

PyDoc_STRVAR(rotator_rotate__doc__, "Returns the vector rotate by the angle about the axis");

static PyMethodDef rotator_methods[] = {
  {"rotate", (PyCFunction) rotator_rotate, METH_VARARGS, rotator_rotate__doc__},
  {NULL}  /* Sentinel */
};


static PyMemberDef rotator_members[] = {
  {NULL}  /* Sentinel */
};

static PyGetSetDef rotator_getseters[] = {
  {NULL}  /* Sentinel */
};


PyTypeObject rotatorType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "rotator",                         /* tp_name */
  sizeof(rotator),                   /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor) rotator_dealloc,      /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  rotator_repr,                      /* tp_repr */
  0,                                 /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  rotator_str,                       /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "rotator objects",                   /* tp_doc */
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  0,                                 /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  rotator_methods,                  /* tp_methods */
  rotator_members,                  /* tp_members */
  rotator_getseters,                /* tp_getset */
  0,                                 /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)rotator_init,           /* tp_init */
  0,                                 /* tp_alloc */
  rotator_new,                      /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_rotatorType(rotator** a_rotator) {
  *a_rotator = PyObject_New(rotator, &rotatorType);
}

static int is_rotatorType(PyObject* a_rotator) {
  //wrapper for type check
  return PyObject_TypeCheck(a_rotator, &rotatorType);
}


// =====================
// ===== spherical =====
// =====================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* spherical_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  spherical* self(NULL);
  self = (spherical*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int spherical_init(spherical* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sRstr, sThetaStr, sPhiStr, NULL};

  double        r(0);
  Coords::angle theta;
  Coords::angle phi;

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);


  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &arg0, &arg1, &arg2))
    return -1;

  if (arg0) {

    if (is_sphericalType(arg0)) {
      // copy constructor
      self->m_spherical.r(((spherical*)arg0)->m_spherical.r());
      self->m_spherical.theta(((spherical*)arg0)->m_spherical.theta());
      self->m_spherical.phi(((spherical*)arg0)->m_spherical.phi());
      return 0;

    } else if (is_CartesianType(arg0)) {
      // Cartesian conversion constructor
      Coords::spherical from_cartesian(((Cartesian*)arg0)->m_Cartesian);
      self->m_spherical.r(from_cartesian.r());
      self->m_spherical.theta(from_cartesian.theta());
      self->m_spherical.phi(from_cartesian.phi());
      return 0;

    } else if ((PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0))) {
      r = PyFloat_AsDouble(arg0);

    } else if (COORDS_STR_CHECK(arg0)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg0 must be a coords.spherical, coords.Cartesian, int or float");
      return -1;
    }

  }

  if (arg1) {

    if (is_AngleType(arg1)) {
      theta = ((Angle*)arg1)->m_angle;

    } else if (is_LatitudeType(arg1)) {
      theta = Coords::angle(90.0) - ((Latitude*)arg1)->m_angle;

    } else if (is_DeclinationType(arg1)) {
      theta = Coords::angle(90.0) - ((Declination*)arg1)->m_angle;

    } else if (COORDS_STR_CHECK(arg1)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg1 must be a coords.angle, coords.Latitude or coords.Declination");
      return -1;
    }

  }


  if (arg2) {

    if (is_AngleType(arg2)) {
      phi = ((Angle*)arg2)->m_angle;

    } else if (COORDS_STR_CHECK(arg2)) {
      PyErr_SetString(sCoordsException, "Direct conversion from string is not supported. Use float(arg).");
      return -1;

    } else {
      PyErr_SetString(sCoordsException, "arg2 must be an angle");
      return -1;
    }

  }

  self->m_spherical.r(r);
  self->m_spherical.theta(theta);
  self->m_spherical.phi(phi);

  return 0;

}

static void spherical_dealloc(spherical* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* spherical_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((spherical*)self)->m_spherical;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

PyObject* spherical_repr(PyObject* self) {
  const Coords::spherical& a_spherical(((spherical*)self)->m_spherical);
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << "("
	 << a_spherical.r() << ", "
	 << a_spherical.theta() << ", "
	 << a_spherical.phi() << ")";
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- R -----

static PyObject* spherical_getR(spherical* self, void* closure) {
  return PyFloat_FromDouble(self->m_spherical.r());
}

static int spherical_setR(spherical* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete r");
    return 0;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "r must be a float or int");
    return 0;
  }

  self->m_spherical.r(PyFloat_AsDouble(value));

  return 0;
}

// ----- theta -----

static PyObject* spherical_getTheta(spherical* self, void* closure) {

  Angle* a_theta(NULL);
  new_AngleType(&a_theta);

  if (a_theta == NULL) {
    PyErr_SetString(sCoordsException, "get theta failed to create coord.angle");
    return NULL;
  }

  a_theta->m_angle = self->m_spherical.theta();

  return (PyObject*) a_theta;

}

static int spherical_setTheta(spherical* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete theta");
    return -1;
  }

  if (!is_AngleType(value)) {
    PyErr_SetString(sCoordsException, "theta must be an angle");
    return -1;
  }

  self->m_spherical.theta(((Angle*)value)->m_angle);

  return 0;
}

// ----- phi -----

static PyObject* spherical_getPhi(spherical* self, void* closure) {

  Angle* a_phi(NULL);
  new_AngleType(&a_phi);

  if (a_phi == NULL) {
    PyErr_SetString(sCoordsException, "get phi failed to create coord.angle");
    return NULL;
  }

  a_phi->m_angle = self->m_spherical.phi();

  return (PyObject*) a_phi;
}

static int spherical_setPhi(spherical* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete phi");
    return -1;
  }

  if (!is_AngleType(value)) {
    PyErr_SetString(sCoordsException, "phi must be an angle");
    return -1;
  }

  self->m_spherical.phi(((Angle*)value)->m_angle);

  return 0;
}

// --------------------------
// ----- number methods -----
// --------------------------

static PyObject* spherical_nb_add(PyObject* o1, PyObject* o2) {

  if (!is_sphericalType(o1) || !is_sphericalType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  spherical* result_spherical(NULL);

  new_sphericalType(&result_spherical);

  if (result_spherical == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.spherical");
    return NULL;
  }

  Coords::spherical the_sum(((spherical*)o1)->m_spherical + ((spherical*)o2)->m_spherical);

  result_spherical->m_spherical = the_sum;

  return (PyObject*) result_spherical;

}


static PyObject* spherical_nb_subtract(PyObject* o1, PyObject* o2) {

  if (!is_sphericalType(o1) || !is_sphericalType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  spherical* result_spherical(NULL);

  new_sphericalType(&result_spherical);

  if (result_spherical == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create coord.spherical");
    return NULL;
  }

  Coords::spherical the_difference(((spherical*)o1)->m_spherical - ((spherical*)o2)->m_spherical);

  result_spherical->m_spherical = the_difference;

  return (PyObject*) result_spherical;

}


static PyObject* spherical_nb_negative(PyObject* o1) {
  // Unitary minus

  if (!is_sphericalType(o1)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  spherical* result_spherical(NULL);

  new_sphericalType(&result_spherical);

  if (result_spherical == NULL) {
    PyErr_SetString(sCoordsException, "unitary minus failed to create coord.spherical");
    return NULL;
  }

  Coords::spherical the_inverse = -((spherical*)o1)->m_spherical;

  result_spherical->m_spherical = the_inverse;

  return (PyObject*) result_spherical;
}


static PyObject* spherical_nb_multiply(PyObject* o1, PyObject* o2) {

  // Returns a scaled version of o1 or o2 if one is spherical and the
  // other double or int. Returns not implemented otherwise.

  // TODO dot product?

  spherical* result_spherical(NULL);
  new_sphericalType(&result_spherical);

  if (result_spherical == NULL) {
    PyErr_SetString(sCoordsException, "multiply failed to create coord.spherical");
    return NULL;
  }

  if (is_sphericalType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    result_spherical->m_spherical = ((spherical*)o1)->m_spherical * PyFloat_AsDouble(o2);
    return (PyObject*) result_spherical;
  }

  if ((PyFloat_Check(o1) || COORDS_INT_CHECK(o1)) && is_sphericalType(o2)) {
    result_spherical->m_spherical = PyFloat_AsDouble(o1) * ((spherical*)o2)->m_spherical;
    return (PyObject*) result_spherical;

  }

  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;

}


static PyObject* spherical_nb_divide(PyObject* o1, PyObject* o2) {
  // This returns a spherical object scaled by the divisor.  o1 must be
  // sphericalType, o2 a float or int otherwise this will raise a
  // NotImplemented error.

  spherical* result_spherical(NULL);
  new_sphericalType(&result_spherical);

  if (result_spherical == NULL) {
    PyErr_SetString(sCoordsException, "divide failed to create coord.spherical");
    return NULL;
  }

  if (is_sphericalType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {

    try {
      result_spherical->m_spherical = ((spherical*)o1)->m_spherical / PyFloat_AsDouble(o2);
    } catch (Coords::DivideByZeroError& err) {
      Py_DECREF(result_spherical);
      PyErr_SetString(sCoordsException, err.what());
      return NULL;
    }

  } else {
    Py_DECREF(result_spherical);
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  return (PyObject*) result_spherical;
}


static PyObject* spherical_tp_richcompare(PyObject* o1, PyObject* o2, int op) {

  if (!is_sphericalType(o1) || !is_sphericalType(o2)) {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  if (op == Py_EQ) {

    if (((spherical*)o1)->m_spherical == ((spherical*)o2)->m_spherical)
      return Py_True;
    else
      return Py_False;

  } else if (op == Py_NE) {

    if (((spherical*)o1)->m_spherical != ((spherical*)o2)->m_spherical)
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

static PyObject* spherical_nb_inplace_add(PyObject* o1, PyObject* o2) {
  if (is_sphericalType(o1) && is_sphericalType(o2)) {
    ((spherical*)o1)->m_spherical.operator+=(((spherical*)o2)->m_spherical);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "spherical::operator+=() only supports spherical types");
  return NULL;
}

static PyObject* spherical_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  if (is_sphericalType(o1) && is_sphericalType(o2)) {
    ((spherical*)o1)->m_spherical.operator-=(((spherical*)o2)->m_spherical);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "spherical::operator-=() only supports spherical types");
  return NULL;
}

static PyObject* spherical_nb_inplace_multiply(PyObject* o1, PyObject* o2) {
  // This only scales the vector. If it returned the dot product, it
  // would switch the object to type double.

  if (is_sphericalType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    ((spherical*)o1)->m_spherical.operator*=(PyFloat_AsDouble(o2));
    Py_INCREF(o1);
    return o1;
  }

  PyErr_SetString(sCoordsException, "spherical::operator*=() only supports spherical types");
  return NULL;

}

static PyObject* spherical_nb_inplace_divide(PyObject* o1, PyObject* o2) {

    // This only scales the vector.

  if (is_sphericalType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    ((spherical*)o1)->m_spherical.operator/=(PyFloat_AsDouble(o2));
    Py_INCREF(o1);
    return o1;
  }

  PyErr_SetString(sCoordsException, "spherical::operator/=() only supports spherical types");
  return NULL;

}


// --------------------------
// ----- Python structs -----
// --------------------------

static PyMethodDef spherical_methods[] = {
  {NULL}  /* Sentinel */
};


static PyMemberDef spherical_members[] = {
  {NULL}  /* Sentinel */
};


static PyGetSetDef spherical_getseters[] = {
  {sRstr, (getter)spherical_getR, (setter)spherical_setR, sRstr, NULL},
  {sThetaStr, (getter)spherical_getTheta, (setter)spherical_setTheta, sThetaStr, NULL},
  {sPhiStr, (getter)spherical_getPhi, (setter)spherical_setPhi, sPhiStr, NULL},
  {NULL}  /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods spherical_as_number = {
  (binaryfunc) spherical_nb_add,
  (binaryfunc) spherical_nb_subtract,
  (binaryfunc) spherical_nb_multiply,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) spherical_nb_negative,
  (unaryfunc) 0,   // nb_positive
  (unaryfunc) 0,   // nb_absolute
  (inquiry) 0,     // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,   // nb_invert
  (binaryfunc) 0,  // nb_lshift
  (binaryfunc) 0,  // nb_rshift
  (binaryfunc) 0,  // nb_and
  (binaryfunc) 0,  // nb_xor
  (binaryfunc) 0,  // nb_or

  (unaryfunc) 0,   // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,   // nb_float

  (binaryfunc) spherical_nb_inplace_add,
  (binaryfunc) spherical_nb_inplace_subtract,
  (binaryfunc) spherical_nb_inplace_multiply,
  (binaryfunc) spherical_nb_inplace_divide, // nb_inplace_remainder
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) spherical_nb_divide,  // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};

#else

// see http://docs.python.org/c-api/typeobj.html

static PyNumberMethods spherical_as_number = {
  (binaryfunc) spherical_nb_add,
  (binaryfunc) spherical_nb_subtract,
  (binaryfunc) spherical_nb_multiply,
  (binaryfunc) spherical_nb_divide,
  (binaryfunc) 0,  // nb_remainder
  (binaryfunc) 0,  // nb_divmod
  (ternaryfunc) 0, // nb_power
  (unaryfunc) spherical_nb_negative,
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

  (binaryfunc) spherical_nb_inplace_add,
  (binaryfunc) spherical_nb_inplace_subtract,
  (binaryfunc) spherical_nb_inplace_multiply,
  (binaryfunc) spherical_nb_inplace_divide,
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


#endif


PyTypeObject sphericalType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "spherical",                              /* tp_name */
  sizeof(spherical),                        /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) spherical_dealloc,           /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  spherical_repr,                           /* tp_repr */
  &spherical_as_number,                     /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  spherical_str,                            /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "spherical objects",                      /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  spherical_tp_richcompare,                 /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  spherical_methods,                        /* tp_methods */
  spherical_members,                        /* tp_members */
  spherical_getseters,                      /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  (initproc)spherical_init,                 /* tp_init */
  0,                                        /* tp_alloc */
  spherical_new,                            /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_sphericalType(spherical** a_spherical) {
  *a_spherical = PyObject_New(spherical, &sphericalType);
}

static int is_sphericalType(PyObject* a_spherical) {
  return PyObject_TypeCheck(a_spherical, &sphericalType);
}

// ====================
// ===== datetime =====
// ====================

// ------------------------
// ----- constructors -----
// ------------------------

static PyObject* datetime_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  datetime* self(NULL);
  self = (datetime*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

static int datetime_init(datetime* self, PyObject* args, PyObject* kwds) {

  static char* kwlist[] = {sYearStr, sMonthStr, sDayStr, sHourStr, sMinuteStr, sSecondStr, sTimeZoneStr, NULL};

  int    year(1970);  // default value
  int    month(1);    // default value
  int    day(1);      // default value
  int    hour(0);     // default value
  int    minute(0);   // default value
  double second(0);   // default value
  double timezone(0); // default value

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);
  PyObject* arg2(NULL);
  PyObject* arg3(NULL);
  PyObject* arg4(NULL);
  PyObject* arg5(NULL);
  PyObject* arg6(NULL);

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOOOOOO", kwlist, &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6))
    return -1;

  if (arg0) {

    if (COORDS_STR_CHECK(arg0)) {

      // ISO-8601 string constructor
      try {

	Coords::DateTime a_datetime(COORDS_STR_AS_STR(arg0));
	self->m_datetime = a_datetime;

      } catch (Coords::Error err) {
	PyErr_SetString(sCoordsException, err.what());
	return -1;
      }

      return 0;

    } else if (is_datetimeType(arg0)) {

      // copy constrctor
      self->m_datetime = ((datetime*)arg0)->m_datetime;
      return 0;

    } else if (PyFloat_Check(arg0) || COORDS_INT_CHECK(arg0)) {
      year = PyFloat_AsDouble(arg0);

    } else {

      PyErr_SetString(sCoordsException, "arg0 must be a datetime, int or float");
      return -1;
    }

  }

  if (parse_int_arg(arg1, month))
      return -1;

  if (parse_int_arg(arg2, day))
      return -1;

  if (parse_int_arg(arg3, hour))
      return -1;

  if (parse_int_arg(arg4, minute))
      return -1;

  if (parse_double_arg(arg5, second))
      return -1;

  if (parse_double_arg(arg6, timezone))
      return -1;

  // create datetime

  try {

    Coords::DateTime a_datetime(year, month, day, hour, minute, second, timezone);
    self->m_datetime = a_datetime;

  } catch (Coords::Error err) {
    PyErr_SetString(sCoordsException, err.what());
    return -1;
  }

  return 0;

}


static void datetime_dealloc(datetime* self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

// -----------------
// ----- print -----
// -----------------

PyObject* datetime_str(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((datetime*)self)->m_datetime;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// TODO a different repr? for constructor?
PyObject* datetime_repr(PyObject* self) {
  std::stringstream result;
  result.precision(sPrintPrecision);
  result << ((datetime*)self)->m_datetime;
  return COORDS_STR_FROM_STR(result.str().c_str());
}

// -------------------------------
// ----- getters and setters -----
// -------------------------------

// ----- Julian Date -----

static PyObject* datetime_getJulianDate(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.toJulianDate());
}

static int datetime_setJulianDate(datetime* self, PyObject* value, void* closure) {

  if (value == NULL) {
    PyErr_SetString(sCoordsException, "can not delete Julian Date");
    return -1;
  }

  if (!PyFloat_Check(value) && !COORDS_INT_CHECK(value)) {
    PyErr_SetString(sCoordsException, "Julian Date must be float or int");
    return -1;
  }

  self->m_datetime.fromJulianDate(PyFloat_AsDouble(value));

  return 0;
}


// TODO boost different from manual accssors

// ----- time zone -----
static PyObject* datetime_getTimeZone(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.timezone());
}

static int datetime_setTimeZone(datetime* self, PyObject* a_timezone, void* closure) {

  if (a_timezone == NULL) {
    PyErr_SetString(sCoordsException, "can not delete timezone");
    return -1;
  }

  if (!PyFloat_Check(a_timezone) && !COORDS_INT_CHECK(a_timezone)) {
    PyErr_SetString(sCoordsException, "timezone must be a float or int");
    return -1;
  }

  self->m_datetime.timezone(PyFloat_AsDouble(a_timezone));

  return 0;
}


// ----- UT -----
static PyObject* datetime_getUT(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.UT());
}

// ----- Lilian Date -----
static PyObject* datetime_getLilianDate(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.LilianDate());
}

// ----- Modified Julian Date -----
static PyObject* datetime_getModifiedJulianDate(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.ModifiedJulianDate());
}

// ----- Truncated Julian Date -----
static PyObject* datetime_getTruncatedJulianDate(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.TruncatedJulianDate());
}

// ----- Truncated Julian Date -----
static PyObject* datetime_getJ2000(datetime* self, void* closure) {
  return PyFloat_FromDouble(self->m_datetime.J2000());
}


// --------------------------
// ----- number methods -----
// --------------------------


static PyObject* datetime_nb_add(PyObject* o1, PyObject* o2) {

  datetime* result_datetime(NULL);
  new_datetimeType(&result_datetime);

  if (result_datetime == NULL) {
    PyErr_SetString(sCoordsException, "add failed to create coord.datetime");
    return NULL;
  }

  if (is_datetimeType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    result_datetime->m_datetime = ((datetime*)o1)->m_datetime + PyFloat_AsDouble(o2);
    return (PyObject*) result_datetime;
  }

  if ((PyFloat_Check(o1) || COORDS_INT_CHECK(o1)) && is_datetimeType(o2)) {
    result_datetime->m_datetime = PyFloat_AsDouble(o1) + ((datetime*)o2)->m_datetime;
    return (PyObject*) result_datetime;
  }

  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;

}


static PyObject* datetime_nb_subtract(PyObject* o1, PyObject* o2) {

  datetime* result_datetime(NULL);
  new_datetimeType(&result_datetime);

  if (result_datetime == NULL) {
    PyErr_SetString(sCoordsException, "subtract failed to create coord.datetime");
    return NULL;
  }

  if (is_datetimeType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    result_datetime->m_datetime = ((datetime*)o1)->m_datetime - PyFloat_AsDouble(o2);
    return (PyObject*) result_datetime;
  }

  if (is_datetimeType(o1) && is_datetimeType(o2)) {
    double delta = ((datetime*)o1)->m_datetime - ((datetime*)o2)->m_datetime;
    return (PyObject*) Py_BuildValue("d", delta);
  }

  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;

}


// ---------------------------
// ----- inplace methods -----
// ---------------------------

static PyObject* datetime_nb_inplace_add(PyObject* o1, PyObject* o2) {
  if (is_datetimeType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    double jdate = PyFloat_AsDouble(o2);
    ((datetime*)o1)->m_datetime.operator+=(jdate);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "datetime::operator+=() only supports datetime types");
  return NULL;
}

static PyObject* datetime_nb_inplace_subtract(PyObject* o1, PyObject* o2) {
  if (is_datetimeType(o1) && (PyFloat_Check(o2) || COORDS_INT_CHECK(o2))) {
    double jdate = PyFloat_AsDouble(o2);
    ((datetime*)o1)->m_datetime.operator-=(jdate);
    Py_INCREF(o1);
    return o1;
  }
  PyErr_SetString(sCoordsException, "datetime::operator-=() only supports datetime types");
  return NULL;
}

// -------------------
// ----- methods -----
// -------------------

static PyObject* datetime_fromJulianDate(PyObject* o1, PyObject* o2) {

  double jdate(0);
  if (!PyArg_ParseTuple(o2, "d", &jdate))
    return NULL;

  ((datetime*)o1)->m_datetime.fromJulianDate(jdate);

  Py_INCREF(Py_None);
  return Py_None;
}

// --------------------------
// ----- Python structs -----
// --------------------------

PyDoc_STRVAR(datetime_toJulianDate__doc__, "Returns the Julian date of the datetime object");
PyDoc_STRVAR(datetime_fromJulianDate__doc__, "Sets the Julian date of the datetime object");
PyDoc_STRVAR(datetime_get_timezone__doc__, "Returns the time zone of the datetime object");
PyDoc_STRVAR(datetime_set_timezone__doc__, "Sets the time zone of the datetime object");
PyDoc_STRVAR(datetime_UT__doc__, "Returns universal time of the datetime object");

static PyMethodDef datetime_methods[] = {
  {"toJulianDate", (PyCFunction) datetime_getJulianDate, METH_VARARGS, datetime_toJulianDate__doc__},
  {"fromJulianDate", (PyCFunction) datetime_fromJulianDate, METH_VARARGS, datetime_fromJulianDate__doc__},
  {"getTimezone", (PyCFunction) datetime_getTimeZone, METH_VARARGS, datetime_get_timezone__doc__},
  {"setTimezone", (PyCFunction) datetime_setTimeZone, METH_VARARGS, datetime_set_timezone__doc__},
  {"UT", (PyCFunction) datetime_getUT, METH_VARARGS, datetime_UT__doc__},
  {NULL}  /* Sentinel */
};


static PyMemberDef datetime_members[] = {
  {NULL}  /* Sentinel */
};

static PyGetSetDef datetime_getseters[] = {
  {sJulianDateStr, (getter)datetime_getJulianDate, (setter)datetime_setJulianDate, sJulianDateStr, NULL},
  {sTimeZoneStr, (getter)datetime_getTimeZone, (setter)datetime_setTimeZone, sTimeZoneStr, NULL},
  {sUTStr, (getter)datetime_getUT, NULL, sUTStr, NULL},
  {sLilianDateStr, (getter)datetime_getLilianDate, NULL, sLilianDateStr, NULL},
  {sModifiedJulianDateStr, (getter)datetime_getModifiedJulianDate, NULL, sModifiedJulianDateStr, NULL},
  {sTruncatedJulianDateStr, (getter)datetime_getTruncatedJulianDate, NULL, sTruncatedJulianDateStr, NULL},
  {sJ2000Str, (getter)datetime_getJ2000, NULL, sJ2000Str, NULL},
  {NULL}  /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

// https://docs.python.org/3/c-api/typeobj.html

static PyNumberMethods datetime_as_number = {
  (binaryfunc) datetime_nb_add,
  (binaryfunc) datetime_nb_subtract,
  (binaryfunc) 0,   // nb_multiply,
  (binaryfunc) 0,   // nb_remainder
  (binaryfunc) 0,   // nb_divmod
  (ternaryfunc) 0,  // nb_power
  (unaryfunc) 0,    // nb_negative,
  (unaryfunc) 0,    // nb_positive
  (unaryfunc) 0,    // nb_absolute
  (inquiry) 0,      // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,    // nb_invert
  (binaryfunc) 0,   // nb_lshift
  (binaryfunc) 0,   // nb_rshift
  (binaryfunc) 0,   // nb_and
  (binaryfunc) 0,   // nb_xor
  (binaryfunc) 0,   // nb_or

  (unaryfunc) 0,    // nb_int
  (void*) 0,       // nb_reserved
  (unaryfunc) 0,    // nb_float


  (binaryfunc) datetime_nb_inplace_add,
  (binaryfunc) datetime_nb_inplace_subtract,
  (binaryfunc) 0,  // nb_inplace_multiply,
  (binaryfunc) 0,  // nb_inplace_remainder
  (ternaryfunc) 0, // nb_inplace_power
  (binaryfunc) 0,  // nb_inplace_lshift
  (binaryfunc) 0,  // nb_inplace_rshift
  (binaryfunc) 0,  // nb_inplace_and
  (binaryfunc) 0,  // nb_inplace_xor
  (binaryfunc) 0,  // nb_inplace_or

  (binaryfunc) 0,  // nb_floor_divide
  (binaryfunc) 0,  // nb_true_divide
  (binaryfunc) 0,  // nb_inplace_floor_divide
  (binaryfunc) 0,  // nb_inplace_true_divide

  (unaryfunc) 0, // nb_index

  (binaryfunc) 0, // nb_matrix_multiply;
  (binaryfunc) 0 // nb_inplace_matrix_multiply;

};


#else

// see http://docs.python.org/c-api/typeobj.html

static PyNumberMethods datetime_as_number = {
  (binaryfunc) datetime_nb_add,
  (binaryfunc) datetime_nb_subtract,
  (binaryfunc) 0,   // nb_multiply,
  (binaryfunc) 0,   // nb_divide,
  (binaryfunc) 0,   // nb_remainder
  (binaryfunc) 0,   // nb_divmod
  (ternaryfunc) 0,  // nb_power
  (unaryfunc) 0,    // nb_negative,
  (unaryfunc) 0,    // nb_positive
  (unaryfunc) 0,    // nb_absolute
  (inquiry) 0,      // nb_nonzero. Used by PyObject_IsTrue.
  (unaryfunc) 0,    // nb_invert
  (binaryfunc) 0,   // nb_lshift
  (binaryfunc) 0,   // nb_rshift
  (binaryfunc) 0,   // nb_and
  (binaryfunc) 0,   // nb_xor
  (binaryfunc) 0,   // nb_or
  (coercion) 0,     // Used by the coerce() function
  (unaryfunc) 0,    // nb_int
  (unaryfunc) 0,    // nb_long
  (unaryfunc) 0,    // nb_float
  (unaryfunc) 0,    // nb_oct
  (unaryfunc) 0,    // nb_hex

  // added in release 2.0

  (binaryfunc) datetime_nb_inplace_add,
  (binaryfunc) datetime_nb_inplace_subtract,
  (binaryfunc) 0,  // nb_inplace_multiply,
  (binaryfunc) 0,  // nb_inplace_divide,
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


#endif


PyTypeObject datetimeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "datetime",                        /* tp_name */
  sizeof(datetime),                  /* tp_basicsize */
  0,                                 /* tp_itemsize */
  (destructor) datetime_dealloc,     /* tp_dealloc */
  0,                                 /* tp_print */
  0,                                 /* tp_getattr */
  0,                                 /* tp_setattr */
  0,                                 /* tp_compare */
  datetime_repr,                        /* tp_repr */
  &datetime_as_number,                  /* tp_as_number */
  0,                                 /* tp_as_sequence */
  0,                                 /* tp_as_mapping */
  0,                                 /* tp_hash */
  0,                                 /* tp_call */
  datetime_str,                         /* tp_str */
  0,                                 /* tp_getattro */
  0,                                 /* tp_setattro */
  0,                                 /* tp_as_buffer */
  COORDS_TPFLAGS, /* tp_flags */
  "datetime objects",                /* tp_doc */
  0,                                 /* tp_traverse */
  0,                                 /* tp_clear */
  0,                                 /* tp_richcompare */
  0,                                 /* tp_weaklistoffset */
  0,                                 /* tp_iter */
  0,                                 /* tp_iternext */
  datetime_methods,                     /* tp_methods */
  datetime_members,                     /* tp_members */
  datetime_getseters,                   /* tp_getset */
  0,                                 /* tp_base */
  0,                                 /* tp_dict */
  0,                                 /* tp_descr_get */
  0,                                 /* tp_descr_set */
  0,                                 /* tp_dictoffset */
  (initproc)datetime_init,              /* tp_init */
  0,                                 /* tp_alloc */
  datetime_new,                         /* tp_new */
};

// ------------------------------------------
// ----- implement forward declarations -----
// ------------------------------------------

static void new_datetimeType(datetime** an_angle) {
  *an_angle = PyObject_New(datetime, &datetimeType);
}

static int is_datetimeType(PyObject* an_angle) {
  //wrapper for type check
  return PyObject_TypeCheck(an_angle, &datetimeType);
}


// --------------------------
// ----- module methods -----
// --------------------------

// ----- Cartesian cross product -----
static PyObject* Cartesian_cross(PyObject* self, PyObject *args) {

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);

  if (!PyArg_ParseTuple(args, "OO", &arg0, &arg1))
    return NULL;

  if (!is_CartesianType(arg0) || !is_CartesianType(arg1)) {
    PyErr_SetString(sCoordsException, "dot product takes two Cartesian types");
    return NULL;
  }

  Cartesian* result_Cartesian(NULL);
  result_Cartesian = PyObject_New(Cartesian, &CartesianType); // alloc and inits
  if (result_Cartesian == NULL) {
    PyErr_SetString(sCoordsException, "cross product failed to create Cartesian.");
    return NULL;
  }


  Coords::Cartesian a_cross_product(Coords::cross(((Cartesian*)arg0)->m_Cartesian,
						  ((Cartesian*)arg1)->m_Cartesian));

  result_Cartesian->m_Cartesian = a_cross_product;

  return (PyObject*) result_Cartesian;

}

// ----- Cartesian dot product -----
static PyObject* Cartesian_dot(PyObject* self, PyObject *args) {

  PyObject* arg0(NULL);
  PyObject* arg1(NULL);

  if (!PyArg_ParseTuple(args, "OO", &arg0, &arg1))
    return NULL;

  if (!is_CartesianType(arg0) || !is_CartesianType(arg1)) {
    PyErr_SetString(sCoordsException, "dot product takes two Cartesian types");
    return NULL;
  }

  double a_dot_product(Coords::dot(((Cartesian*)arg0)->m_Cartesian, ((Cartesian*)arg1)->m_Cartesian));

  return (PyObject*) Py_BuildValue("d", a_dot_product);

}

// -----------------------
// ----- method list -----
// -----------------------
PyDoc_STRVAR(Cartesian_cross__doc__, "Returns the cross product of two Cartesian objects");
PyDoc_STRVAR(Cartesian_dot__doc__, "Returns the dot product of two Cartesian objects");

// TODO cross, dot as module methods, not instance methods

PyMethodDef coords_module_methods[] = {
  {"cross", (PyCFunction) Cartesian_cross, METH_VARARGS, Cartesian_cross__doc__},
  {"dot", (PyCFunction) Cartesian_dot, METH_VARARGS, Cartesian_dot__doc__},
  {NULL, NULL}  /* Sentinel */
};


// ----------------------------
// ----- module utilities -----
// ----------------------------

PyObject* Cartesian_create(const Coords::Cartesian& a_Cartesian) {
  // Creates a python Cartesian object from a Coords::Cartesian object.
  // Intended for use in the module initorbits function to generate
  // Cartesian constants with Coords::Cartesian::U[oxyz]

  // TODO borrowed reference?
  Cartesian* py_Cartesian(NULL);

  py_Cartesian = PyObject_New(Cartesian, &CartesianType); // alloc and inits

  // TODO exception handle this
  if (py_Cartesian == NULL){
    PyErr_SetString(sCoordsException, "failed to create coord.Cartesian.");
    return NULL;
  }

  py_Cartesian->m_Cartesian.x(a_Cartesian.x());
  py_Cartesian->m_Cartesian.y(a_Cartesian.y());
  py_Cartesian->m_Cartesian.z(a_Cartesian.z());

  return (PyObject*) py_Cartesian;
}

// ------------------------------
// ----- init coords module -----
// ------------------------------

MOD_INIT(coords) {

  PyObject* m;

  MOD_DEF(m, "coords", "python wrappers for coords objects.", coords_module_methods);

  // error
  char eMsgStr[] = "coords.Error";
  sCoordsException = PyErr_NewException(eMsgStr, NULL, NULL);
  Py_INCREF(sCoordsException);
  PyModule_AddObject(m, "Error", sCoordsException);


  // Angle
  AngleType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&AngleType) < 0)
    return MOD_ERROR_VAL;
  Py_INCREF(&AngleType);
  PyModule_AddObject(m, "angle", (PyObject *)&AngleType);

  // Latitude
  LatitudeType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&LatitudeType) < 0)
    return MOD_ERROR_VAL;
  Py_INCREF(&LatitudeType);
  PyModule_AddObject(m, "latitude", (PyObject *)&LatitudeType);

  // Declination
  DeclinationType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&DeclinationType) < 0)
    return MOD_ERROR_VAL;
  Py_INCREF(&DeclinationType);
  PyModule_AddObject(m, "declination", (PyObject *)&DeclinationType);


  // Cartesian
  CartesianType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&CartesianType) < 0)
    return MOD_ERROR_VAL;

  Py_INCREF(&CartesianType);
  PyModule_AddObject(m, "Cartesian", (PyObject *)&CartesianType);

  // rotator
  rotatorType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&rotatorType) < 0)
    return MOD_ERROR_VAL;

  Py_INCREF(&rotatorType);
  PyModule_AddObject(m, "rotator", (PyObject *)&rotatorType);

  // spherical
  sphericalType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&sphericalType) < 0)
    return MOD_ERROR_VAL;

  Py_INCREF(&sphericalType);
  PyModule_AddObject(m, "spherical", (PyObject *)&sphericalType);

  // datetime
  datetimeType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&datetimeType) < 0)
    return MOD_ERROR_VAL;
  Py_INCREF(&datetimeType);
  PyModule_AddObject(m, "datetime", (PyObject *)&datetimeType);

  // module unit vector constants

  PyObject* Cartesian_Uo(NULL);
  Cartesian_Uo = Cartesian_create(Coords::Cartesian::Uo);
  Py_INCREF(Cartesian_Uo);
  PyModule_AddObject(m, "Uo", (PyObject*)Cartesian_Uo);

  PyObject* Cartesian_Ux(NULL);
  Cartesian_Ux = Cartesian_create(Coords::Cartesian::Ux);
  Py_INCREF(Cartesian_Ux);
  PyModule_AddObject(m, "Ux", (PyObject*)Cartesian_Ux);

  PyObject* Cartesian_Uy(NULL);
  Cartesian_Uy = Cartesian_create(Coords::Cartesian::Uy);
  Py_INCREF(Cartesian_Uy);
  PyModule_AddObject(m, "Uy", (PyObject*)Cartesian_Uy);

  PyObject* Cartesian_Uz(NULL);
  Cartesian_Uz = Cartesian_create(Coords::Cartesian::Uz);
  Py_INCREF(Cartesian_Uz);
  PyModule_AddObject(m, "Uz", (PyObject*)Cartesian_Uz);

  return MOD_SUCCESS_VAL(m);
}
