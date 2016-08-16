
#define PY_SSIZE_T_CLEAN
#include "Python.h"
#ifndef Py_PYTHON_H
    #error Python headers needed to compile C extensions, please install development version of Python.
#elif PY_VERSION_HEX < 0x02040000
    #error Cython requires Python 2.4+.
#else
#include <stddef.h> 
#ifndef offsetof
#define offsetof(type, member) ( (size_t) & ((type*)0) -> member )
#endif
#if !defined(WIN32) && !defined(MS_WINDOWS)
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
  #ifndef __fastcall
    #define __fastcall
  #endif
#endif
#ifndef DL_IMPORT
  #define DL_IMPORT(t) t
#endif
#ifndef DL_EXPORT
  #define DL_EXPORT(t) t
#endif
#ifndef PY_LONG_LONG
  #define PY_LONG_LONG LONG_LONG
#endif
#ifndef Py_HUGE_VAL
  #define Py_HUGE_VAL HUGE_VAL
#endif
#ifdef PYPY_VERSION
#define CYTHON_COMPILING_IN_PYPY 1
#define CYTHON_COMPILING_IN_CPYTHON 0
#else
#define CYTHON_COMPILING_IN_PYPY 0
#define CYTHON_COMPILING_IN_CPYTHON 1
#endif
#if PY_VERSION_HEX < 0x02050000
  typedef int Py_ssize_t;
  #define PY_SSIZE_T_MAX INT_MAX
  #define PY_SSIZE_T_MIN INT_MIN
  #define PY_FORMAT_SIZE_T ""
  #define CYTHON_FORMAT_SSIZE_T ""
  #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
  #define PyInt_AsSsize_t(o)   __Pyx_PyInt_AsInt(o)
  #define PyNumber_Index(o)    ((PyNumber_Check(o) && !PyFloat_Check(o)) ? PyNumber_Int(o) : \
                                (PyErr_Format(PyExc_TypeError, \
                                              "expected index value, got %.200s", Py_TYPE(o)->tp_name), \
                                 (PyObject*)0))
  #define __Pyx_PyIndex_Check(o) (PyNumber_Check(o) && !PyFloat_Check(o) && \
                                  !PyComplex_Check(o))
  #define PyIndex_Check __Pyx_PyIndex_Check
  #define PyErr_WarnEx(category, message, stacklevel) PyErr_Warn(category, message)
  #define __PYX_BUILD_PY_SSIZE_T "i"
#else
  #define __PYX_BUILD_PY_SSIZE_T "n"
  #define CYTHON_FORMAT_SSIZE_T "z"
  #define __Pyx_PyIndex_Check PyIndex_Check
#endif
#if PY_VERSION_HEX < 0x02060000
  #define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
  #define Py_TYPE(ob)   (((PyObject*)(ob))->ob_type)
  #define Py_SIZE(ob)   (((PyVarObject*)(ob))->ob_size)
  #define PyVarObject_HEAD_INIT(type, size) \
          PyObject_HEAD_INIT(type) size,
  #define PyType_Modified(t)
  typedef struct {
     void *buf;
     PyObject *obj;
     Py_ssize_t len;
     Py_ssize_t itemsize;
     int readonly;
     int ndim;
     char *format;
     Py_ssize_t *shape;
     Py_ssize_t *strides;
     Py_ssize_t *suboffsets;
     void *internal;
  } Py_buffer;
  #define PyBUF_SIMPLE 0
  #define PyBUF_WRITABLE 0x0001
  #define PyBUF_FORMAT 0x0004
  #define PyBUF_ND 0x0008
  #define PyBUF_STRIDES (0x0010 | PyBUF_ND)
  #define PyBUF_C_CONTIGUOUS (0x0020 | PyBUF_STRIDES)
  #define PyBUF_F_CONTIGUOUS (0x0040 | PyBUF_STRIDES)
  #define PyBUF_ANY_CONTIGUOUS (0x0080 | PyBUF_STRIDES)
  #define PyBUF_INDIRECT (0x0100 | PyBUF_STRIDES)
  #define PyBUF_RECORDS (PyBUF_STRIDES | PyBUF_FORMAT | PyBUF_WRITABLE)
  #define PyBUF_FULL (PyBUF_INDIRECT | PyBUF_FORMAT | PyBUF_WRITABLE)
  typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
  typedef void (*releasebufferproc)(PyObject *, Py_buffer *);
#endif
#if PY_MAJOR_VERSION < 3
  #define __Pyx_BUILTIN_MODULE_NAME "__builtin__"
  #define __Pyx_PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos) \
          PyCode_New(a, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos)
#else
  #define __Pyx_BUILTIN_MODULE_NAME "builtins"
  #define __Pyx_PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos) \
          PyCode_New(a, k, l, s, f, code, c, n, v, fv, cell, fn, name, fline, lnos)
#endif
#if PY_MAJOR_VERSION < 3 && PY_MINOR_VERSION < 6
  #define PyUnicode_FromString(s) PyUnicode_Decode(s, strlen(s), "UTF-8", "strict")
#endif
#if PY_MAJOR_VERSION >= 3
  #define Py_TPFLAGS_CHECKTYPES 0
  #define Py_TPFLAGS_HAVE_INDEX 0
#endif
#if (PY_VERSION_HEX < 0x02060000) || (PY_MAJOR_VERSION >= 3)
  #define Py_TPFLAGS_HAVE_NEWBUFFER 0
#endif
#if PY_VERSION_HEX > 0x03030000 && defined(PyUnicode_KIND)
  #define CYTHON_PEP393_ENABLED 1
  #define __Pyx_PyUnicode_READY(op)       (likely(PyUnicode_IS_READY(op)) ? \
                                              0 : _PyUnicode_Ready((PyObject *)(op)))
  #define __Pyx_PyUnicode_GET_LENGTH(u)   PyUnicode_GET_LENGTH(u)
  #define __Pyx_PyUnicode_READ_CHAR(u, i) PyUnicode_READ_CHAR(u, i)
  #define __Pyx_PyUnicode_READ(k, d, i)   PyUnicode_READ(k, d, i)
#else
  #define CYTHON_PEP393_ENABLED 0
  #define __Pyx_PyUnicode_READY(op)       (0)
  #define __Pyx_PyUnicode_GET_LENGTH(u)   PyUnicode_GET_SIZE(u)
  #define __Pyx_PyUnicode_READ_CHAR(u, i) ((Py_UCS4)(PyUnicode_AS_UNICODE(u)[i]))
  #define __Pyx_PyUnicode_READ(k, d, i)   ((k=k), (Py_UCS4)(((Py_UNICODE*)d)[i]))
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyBaseString_Type            PyUnicode_Type
  #define PyStringObject               PyUnicodeObject
  #define PyString_Type                PyUnicode_Type
  #define PyString_Check               PyUnicode_Check
  #define PyString_CheckExact          PyUnicode_CheckExact
#endif
#if PY_VERSION_HEX < 0x02060000
  #define PyBytesObject                PyStringObject
  #define PyBytes_Type                 PyString_Type
  #define PyBytes_Check                PyString_Check
  #define PyBytes_CheckExact           PyString_CheckExact
  #define PyBytes_FromString           PyString_FromString
  #define PyBytes_FromStringAndSize    PyString_FromStringAndSize
  #define PyBytes_FromFormat           PyString_FromFormat
  #define PyBytes_DecodeEscape         PyString_DecodeEscape
  #define PyBytes_AsString             PyString_AsString
  #define PyBytes_AsStringAndSize      PyString_AsStringAndSize
  #define PyBytes_Size                 PyString_Size
  #define PyBytes_AS_STRING            PyString_AS_STRING
  #define PyBytes_GET_SIZE             PyString_GET_SIZE
  #define PyBytes_Repr                 PyString_Repr
  #define PyBytes_Concat               PyString_Concat
  #define PyBytes_ConcatAndDel         PyString_ConcatAndDel
#endif
#if PY_VERSION_HEX < 0x02060000
  #define PySet_Check(obj)             PyObject_TypeCheck(obj, &PySet_Type)
  #define PyFrozenSet_Check(obj)       PyObject_TypeCheck(obj, &PyFrozenSet_Type)
#endif
#ifndef PySet_CheckExact
  #define PySet_CheckExact(obj)        (Py_TYPE(obj) == &PySet_Type)
#endif
#define __Pyx_TypeCheck(obj, type) PyObject_TypeCheck(obj, (PyTypeObject *)type)
#if PY_MAJOR_VERSION >= 3
  #define PyIntObject                  PyLongObject
  #define PyInt_Type                   PyLong_Type
  #define PyInt_Check(op)              PyLong_Check(op)
  #define PyInt_CheckExact(op)         PyLong_CheckExact(op)
  #define PyInt_FromString             PyLong_FromString
  #define PyInt_FromUnicode            PyLong_FromUnicode
  #define PyInt_FromLong               PyLong_FromLong
  #define PyInt_FromSize_t             PyLong_FromSize_t
  #define PyInt_FromSsize_t            PyLong_FromSsize_t
  #define PyInt_AsLong                 PyLong_AsLong
  #define PyInt_AS_LONG                PyLong_AS_LONG
  #define PyInt_AsSsize_t              PyLong_AsSsize_t
  #define PyInt_AsUnsignedLongMask     PyLong_AsUnsignedLongMask
  #define PyInt_AsUnsignedLongLongMask PyLong_AsUnsignedLongLongMask
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyBoolObject                 PyLongObject
#endif
#if PY_VERSION_HEX < 0x03020000
  typedef long Py_hash_t;
  #define __Pyx_PyInt_FromHash_t PyInt_FromLong
  #define __Pyx_PyInt_AsHash_t   PyInt_AsLong
#else
  #define __Pyx_PyInt_FromHash_t PyInt_FromSsize_t
  #define __Pyx_PyInt_AsHash_t   PyInt_AsSsize_t
#endif
#if (PY_MAJOR_VERSION < 3) || (PY_VERSION_HEX >= 0x03010300)
  #define __Pyx_PySequence_GetSlice(obj, a, b) PySequence_GetSlice(obj, a, b)
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) PySequence_SetSlice(obj, a, b, value)
  #define __Pyx_PySequence_DelSlice(obj, a, b) PySequence_DelSlice(obj, a, b)
#else
  #define __Pyx_PySequence_GetSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), (PyObject*)0) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_GetSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object is unsliceable", (obj)->ob_type->tp_name), (PyObject*)0)))
  #define __Pyx_PySequence_SetSlice(obj, a, b, value) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_SetSlice(obj, a, b, value)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice assignment", (obj)->ob_type->tp_name), -1)))
  #define __Pyx_PySequence_DelSlice(obj, a, b) (unlikely(!(obj)) ? \
        (PyErr_SetString(PyExc_SystemError, "null argument to internal routine"), -1) : \
        (likely((obj)->ob_type->tp_as_mapping) ? (PySequence_DelSlice(obj, a, b)) : \
            (PyErr_Format(PyExc_TypeError, "'%.200s' object doesn't support slice deletion", (obj)->ob_type->tp_name), -1)))
#endif
#if PY_MAJOR_VERSION >= 3
  #define PyMethod_New(func, self, klass) ((self) ? PyMethod_New(func, self) : PyInstanceMethod_New(func))
#endif
#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),((char *)(n)))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),((char *)(n)),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),((char *)(n)))
#else
  #define __Pyx_GetAttrString(o,n)   PyObject_GetAttrString((o),(n))
  #define __Pyx_SetAttrString(o,n,a) PyObject_SetAttrString((o),(n),(a))
  #define __Pyx_DelAttrString(o,n)   PyObject_DelAttrString((o),(n))
#endif
#if PY_VERSION_HEX < 0x02050000
  #define __Pyx_NAMESTR(n) ((char *)(n))
  #define __Pyx_DOCSTR(n)  ((char *)(n))
#else
  #define __Pyx_NAMESTR(n) (n)
  #define __Pyx_DOCSTR(n)  (n)
#endif


#if PY_MAJOR_VERSION >= 3
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_TrueDivide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceTrueDivide(x,y)
#else
  #define __Pyx_PyNumber_Divide(x,y)         PyNumber_Divide(x,y)
  #define __Pyx_PyNumber_InPlaceDivide(x,y)  PyNumber_InPlaceDivide(x,y)
#endif

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#if defined(WIN32) || defined(MS_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#define __PYX_HAVE__bintrees__qavltree
#define __PYX_HAVE_API__bintrees__qavltree
#include "ctrees.h"
#include "stack.h"
#ifdef _OPENMP
#include <omp.h>
#endif 

#ifdef PYREX_WITHOUT_ASSERTIONS
#define CYTHON_WITHOUT_ASSERTIONS
#endif


#ifndef CYTHON_INLINE
  #if defined(__GNUC__)
    #define CYTHON_INLINE __inline__
  #elif defined(_MSC_VER)
    #define CYTHON_INLINE __inline
  #elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define CYTHON_INLINE inline
  #else
    #define CYTHON_INLINE
  #endif
#endif

#ifndef CYTHON_UNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define CYTHON_UNUSED __attribute__ ((__unused__))
#   else
#     define CYTHON_UNUSED
#   endif
# elif defined(__ICC) || (defined(__INTEL_COMPILER) && !defined(_MSC_VER))
#   define CYTHON_UNUSED __attribute__ ((__unused__))
# else
#   define CYTHON_UNUSED
# endif
#endif

typedef struct {PyObject **p; char *s; const long n; const char* encoding; const char is_unicode; const char is_str; const char intern; } __Pyx_StringTabEntry; 



#define __Pyx_PyBytes_FromUString(s) PyBytes_FromString((char*)s)
#define __Pyx_PyBytes_AsUString(s)   ((unsigned char*) PyBytes_AsString(s))

#define __Pyx_Owned_Py_None(b) (Py_INCREF(Py_None), Py_None)
#define __Pyx_PyBool_FromLong(b) ((b) ? (Py_INCREF(Py_True), Py_True) : (Py_INCREF(Py_False), Py_False))
static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject*);
static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x);

static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject*);
static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t);
static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject*);

#if CYTHON_COMPILING_IN_CPYTHON
#define __pyx_PyFloat_AsDouble(x) (PyFloat_CheckExact(x) ? PyFloat_AS_DOUBLE(x) : PyFloat_AsDouble(x))
#else
#define __pyx_PyFloat_AsDouble(x) PyFloat_AsDouble(x)
#endif
#define __pyx_PyFloat_AsFloat(x) ((float) __pyx_PyFloat_AsDouble(x))

#ifdef __GNUC__
  
  #if __GNUC__ > 2 || (__GNUC__ == 2 && (__GNUC_MINOR__ > 95))
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
  #else 
    #define likely(x)   (x)
    #define unlikely(x) (x)
  #endif 
#else 
  #define likely(x)   (x)
  #define unlikely(x) (x)
#endif 
    
static PyObject *__pyx_m;
static PyObject *__pyx_b;
static PyObject *__pyx_empty_tuple;
static PyObject *__pyx_empty_bytes;
static int __pyx_lineno;
static int __pyx_clineno = 0;
static const char * __pyx_cfilenm= __FILE__;
static const char *__pyx_filename;


static const char *__pyx_f[] = {
  "qavltree.pyx",
  "cwalker.pxd",
};

struct __pyx_obj_8bintrees_7cwalker_cWalker;
struct __pyx_obj_8bintrees_8qavltree_cAVLTree;

struct __pyx_obj_8bintrees_7cwalker_cWalker {
  PyObject_HEAD
  struct __pyx_vtabstruct_8bintrees_7cwalker_cWalker *__pyx_vtab;
  node_t *node;
  node_t *root;
  node_stack_t *stack;
};


struct __pyx_obj_8bintrees_8qavltree_cAVLTree {
  PyObject_HEAD
  node_t *_root;
  int _count;
};




struct __pyx_vtabstruct_8bintrees_7cwalker_cWalker {
  void (*set_tree)(struct __pyx_obj_8bintrees_7cwalker_cWalker *, node_t *);
  PyObject *(*reset)(struct __pyx_obj_8bintrees_7cwalker_cWalker *, int __pyx_skip_dispatch);
  PyObject *(*push)(struct __pyx_obj_8bintrees_7cwalker_cWalker *, int __pyx_skip_dispatch);
  PyObject *(*pop)(struct __pyx_obj_8bintrees_7cwalker_cWalker *, int __pyx_skip_dispatch);
};
static struct __pyx_vtabstruct_8bintrees_7cwalker_cWalker *__pyx_vtabptr_8bintrees_7cwalker_cWalker;
#ifndef CYTHON_REFNANNY
  #define CYTHON_REFNANNY 0
#endif
#if CYTHON_REFNANNY
  typedef struct {
    void (*INCREF)(void*, PyObject*, int);
    void (*DECREF)(void*, PyObject*, int);
    void (*GOTREF)(void*, PyObject*, int);
    void (*GIVEREF)(void*, PyObject*, int);
    void* (*SetupContext)(const char*, int, const char*);
    void (*FinishContext)(void**);
  } __Pyx_RefNannyAPIStruct;
  static __Pyx_RefNannyAPIStruct *__Pyx_RefNanny = NULL;
  static __Pyx_RefNannyAPIStruct *__Pyx_RefNannyImportAPI(const char *modname); 
  #define __Pyx_RefNannyDeclarations void *__pyx_refnanny = NULL;
#ifdef WITH_THREAD
  #define __Pyx_RefNannySetupContext(name, acquire_gil) \
          if (acquire_gil) { \
              PyGILState_STATE __pyx_gilstate_save = PyGILState_Ensure(); \
              __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__); \
              PyGILState_Release(__pyx_gilstate_save); \
          } else { \
              __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__); \
          }
#else
  #define __Pyx_RefNannySetupContext(name, acquire_gil) \
          __pyx_refnanny = __Pyx_RefNanny->SetupContext((name), __LINE__, __FILE__)
#endif
  #define __Pyx_RefNannyFinishContext() \
          __Pyx_RefNanny->FinishContext(&__pyx_refnanny)
  #define __Pyx_INCREF(r)  __Pyx_RefNanny->INCREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_DECREF(r)  __Pyx_RefNanny->DECREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GOTREF(r)  __Pyx_RefNanny->GOTREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_GIVEREF(r) __Pyx_RefNanny->GIVEREF(__pyx_refnanny, (PyObject *)(r), __LINE__)
  #define __Pyx_XINCREF(r)  do { if((r) != NULL) {__Pyx_INCREF(r); }} while(0)
  #define __Pyx_XDECREF(r)  do { if((r) != NULL) {__Pyx_DECREF(r); }} while(0)
  #define __Pyx_XGOTREF(r)  do { if((r) != NULL) {__Pyx_GOTREF(r); }} while(0)
  #define __Pyx_XGIVEREF(r) do { if((r) != NULL) {__Pyx_GIVEREF(r);}} while(0)
#else
  #define __Pyx_RefNannyDeclarations
  #define __Pyx_RefNannySetupContext(name, acquire_gil)
  #define __Pyx_RefNannyFinishContext()
  #define __Pyx_INCREF(r) Py_INCREF(r)
  #define __Pyx_DECREF(r) Py_DECREF(r)
  #define __Pyx_GOTREF(r)
  #define __Pyx_GIVEREF(r)
  #define __Pyx_XINCREF(r) Py_XINCREF(r)
  #define __Pyx_XDECREF(r) Py_XDECREF(r)
  #define __Pyx_XGOTREF(r)
  #define __Pyx_XGIVEREF(r)
#endif 
#define __Pyx_CLEAR(r)    do { PyObject* tmp = ((PyObject*)(r)); r = NULL; __Pyx_DECREF(tmp);} while(0)
#define __Pyx_XCLEAR(r)   do { if((r) != NULL) {PyObject* tmp = ((PyObject*)(r)); r = NULL; __Pyx_DECREF(tmp);}} while(0)

static PyObject *__Pyx_GetName(PyObject *dict, PyObject *name); 

static void __Pyx_RaiseDoubleKeywordsError(const char* func_name, PyObject* kw_name); 

static int __Pyx_ParseOptionalKeywords(PyObject *kwds, PyObject **argnames[], \
    PyObject *kwds2, PyObject *values[], Py_ssize_t num_pos_args, \
    const char* function_name); 

static void __Pyx_RaiseArgtupleInvalid(const char* func_name, int exact,
    Py_ssize_t num_min, Py_ssize_t num_max, Py_ssize_t num_found); 

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb); 
static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb); 

static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb, PyObject *cause); 

static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Generic(PyObject *o, PyObject* j) {
    PyObject *r;
    if (!j) return NULL;
    r = PyObject_GetItem(o, j);
    Py_DECREF(j);
    return r;
}
#define __Pyx_GetItemInt_List(o, i, size, to_py_func) (((size) <= sizeof(Py_ssize_t)) ? \
                                                    __Pyx_GetItemInt_List_Fast(o, i) : \
                                                    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_List_Fast(PyObject *o, Py_ssize_t i) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (likely((0 <= i) & (i < PyList_GET_SIZE(o)))) {
        PyObject *r = PyList_GET_ITEM(o, i);
        Py_INCREF(r);
        return r;
    }
    else if ((-PyList_GET_SIZE(o) <= i) & (i < 0)) {
        PyObject *r = PyList_GET_ITEM(o, PyList_GET_SIZE(o) + i);
        Py_INCREF(r);
        return r;
    }
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
#else
    return PySequence_GetItem(o, i);
#endif
}
#define __Pyx_GetItemInt_Tuple(o, i, size, to_py_func) (((size) <= sizeof(Py_ssize_t)) ? \
                                                    __Pyx_GetItemInt_Tuple_Fast(o, i) : \
                                                    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Tuple_Fast(PyObject *o, Py_ssize_t i) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (likely((0 <= i) & (i < PyTuple_GET_SIZE(o)))) {
        PyObject *r = PyTuple_GET_ITEM(o, i);
        Py_INCREF(r);
        return r;
    }
    else if ((-PyTuple_GET_SIZE(o) <= i) & (i < 0)) {
        PyObject *r = PyTuple_GET_ITEM(o, PyTuple_GET_SIZE(o) + i);
        Py_INCREF(r);
        return r;
    }
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
#else
    return PySequence_GetItem(o, i);
#endif
}
#define __Pyx_GetItemInt(o, i, size, to_py_func) (((size) <= sizeof(Py_ssize_t)) ? \
                                                    __Pyx_GetItemInt_Fast(o, i) : \
                                                    __Pyx_GetItemInt_Generic(o, to_py_func(i)))
static CYTHON_INLINE PyObject *__Pyx_GetItemInt_Fast(PyObject *o, Py_ssize_t i) {
#if CYTHON_COMPILING_IN_CPYTHON
    if (PyList_CheckExact(o)) {
        Py_ssize_t n = (likely(i >= 0)) ? i : i + PyList_GET_SIZE(o);
        if (likely((n >= 0) & (n < PyList_GET_SIZE(o)))) {
            PyObject *r = PyList_GET_ITEM(o, n);
            Py_INCREF(r);
            return r;
        }
    }
    else if (PyTuple_CheckExact(o)) {
        Py_ssize_t n = (likely(i >= 0)) ? i : i + PyTuple_GET_SIZE(o);
        if (likely((n >= 0) & (n < PyTuple_GET_SIZE(o)))) {
            PyObject *r = PyTuple_GET_ITEM(o, n);
            Py_INCREF(r);
            return r;
        }
    } else {  
        PySequenceMethods *m = Py_TYPE(o)->tp_as_sequence;
        if (likely(m && m->sq_item)) {
            if (unlikely(i < 0) && likely(m->sq_length)) {
                Py_ssize_t l = m->sq_length(o);
                if (unlikely(l < 0)) return NULL;
                i += l;
            }
            return m->sq_item(o, i);
        }
    }
#else
    if (PySequence_Check(o)) {
        return PySequence_GetItem(o, i);
    }
#endif
    return __Pyx_GetItemInt_Generic(o, PyInt_FromSsize_t(i));
}

static PyObject *__Pyx_Import(PyObject *name, PyObject *from_list, long level); 

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject *);

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject *);

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject *);

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject *);

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject *);

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject *);

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject *);

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject *);

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject *);

static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject *);

static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject *);

static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject *);

static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject *);

static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject *);

static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject *);

static int __Pyx_check_binary_version(void);

#if !defined(__Pyx_PyIdentifier_FromString)
#if PY_MAJOR_VERSION < 3
  #define __Pyx_PyIdentifier_FromString(s) PyString_FromString(s)
#else
  #define __Pyx_PyIdentifier_FromString(s) PyUnicode_FromString(s)
#endif
#endif

static PyObject *__Pyx_ImportModule(const char *name); 

static PyTypeObject *__Pyx_ImportType(const char *module_name, const char *class_name, size_t size, int strict);  

static void* __Pyx_GetVtable(PyObject *dict); 

typedef struct {
    int code_line;
    PyCodeObject* code_object;
} __Pyx_CodeObjectCacheEntry;
struct __Pyx_CodeObjectCache {
    int count;
    int max_count;
    __Pyx_CodeObjectCacheEntry* entries;
};
static struct __Pyx_CodeObjectCache __pyx_code_cache = {0,0,NULL};
static int __pyx_bisect_code_objects(__Pyx_CodeObjectCacheEntry* entries, int count, int code_line);
static PyCodeObject *__pyx_find_code_object(int code_line);
static void __pyx_insert_code_object(int code_line, PyCodeObject* code_object);

static void __Pyx_AddTraceback(const char *funcname, int c_line,
                               int py_line, const char *filename); 

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t); 




static PyTypeObject *__pyx_ptype_8bintrees_7cwalker_cWalker = 0;

static PyTypeObject *__pyx_ptype_8bintrees_8qavltree_cAVLTree = 0;
#define __Pyx_MODULE_NAME "bintrees.qavltree"
int __pyx_module_is_main_bintrees__qavltree = 0;

static PyObject *__pyx_builtin_property;
static PyObject *__pyx_builtin_KeyError;
static PyObject *__pyx_builtin_MemoryError;
static PyObject *__pyx_builtin_ValueError;
static int __pyx_pf_8bintrees_8qavltree_8cAVLTree___cinit__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_items); 
static void __pyx_pf_8bintrees_8qavltree_8cAVLTree_2__dealloc__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_4count(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_6__getstate__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_8__setstate__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_state); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_10clear(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_12get_value(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_14get_walker(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_16insert(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key, PyObject *__pyx_v_value); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_18remove(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_20max_item(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_22min_item(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self); 
static char __pyx_k_1[] = "Can not allocate memory for node structure.";
static char __pyx_k_3[] = "Tree is empty";
static char __pyx_k__key[] = "key";
static char __pyx_k__count[] = "count";
static char __pyx_k__items[] = "items";
static char __pyx_k__value[] = "value";
static char __pyx_k__update[] = "update";
static char __pyx_k____all__[] = "__all__";
static char __pyx_k__cWalker[] = "cWalker";
static char __pyx_k__cwalker[] = "cwalker";
static char __pyx_k__KeyError[] = "KeyError";
static char __pyx_k____main__[] = "__main__";
static char __pyx_k____test__[] = "__test__";
static char __pyx_k__cAVLTree[] = "cAVLTree";
static char __pyx_k__property[] = "property";
static char __pyx_k__ValueError[] = "ValueError";
static char __pyx_k__MemoryError[] = "MemoryError";
static PyObject *__pyx_kp_s_1;
static PyObject *__pyx_kp_s_3;
static PyObject *__pyx_n_s__KeyError;
static PyObject *__pyx_n_s__MemoryError;
static PyObject *__pyx_n_s__ValueError;
static PyObject *__pyx_n_s____all__;
static PyObject *__pyx_n_s____main__;
static PyObject *__pyx_n_s____test__;
static PyObject *__pyx_n_s__cAVLTree;
static PyObject *__pyx_n_s__cWalker;
static PyObject *__pyx_n_s__count;
static PyObject *__pyx_n_s__cwalker;
static PyObject *__pyx_n_s__items;
static PyObject *__pyx_n_s__key;
static PyObject *__pyx_n_s__property;
static PyObject *__pyx_n_s__update;
static PyObject *__pyx_n_s__value;
static PyObject *__pyx_int_0;
static PyObject *__pyx_k_tuple_2;
static PyObject *__pyx_k_tuple_4;
static PyObject *__pyx_k_tuple_5;

static int __pyx_pw_8bintrees_8qavltree_8cAVLTree_1__cinit__(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); 
static int __pyx_pw_8bintrees_8qavltree_8cAVLTree_1__cinit__(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_items = 0;
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__cinit__ (wrapper)", 0);
  {
    static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__items,0};
    PyObject* values[1] = {0};

    values[0] = ((PyObject *)Py_None);
    if (unlikely(__pyx_kwds)) {
      Py_ssize_t kw_args;
      const Py_ssize_t pos_args = PyTuple_GET_SIZE(__pyx_args);
      switch (pos_args) {
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
      kw_args = PyDict_Size(__pyx_kwds);
      switch (pos_args) {
        case  0:
        if (kw_args > 0) {
          PyObject* value = PyDict_GetItem(__pyx_kwds, __pyx_n_s__items);
          if (value) { values[0] = value; kw_args--; }
        }
      }
      if (unlikely(kw_args > 0)) {
        if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, pos_args, "__cinit__") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 20; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    } else {
      switch (PyTuple_GET_SIZE(__pyx_args)) {
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
    }
    __pyx_v_items = values[0];
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("__cinit__", 0, 0, 1, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 20; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.__cinit__", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __Pyx_RefNannyFinishContext();
  return -1;
  __pyx_L4_argument_unpacking_done:;
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree___cinit__(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self), __pyx_v_items);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static int __pyx_pf_8bintrees_8qavltree_8cAVLTree___cinit__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_items) {
  int __pyx_r;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("__cinit__", 0);

  __pyx_v_self->_root = NULL;

  __pyx_v_self->_count = 0;

  __pyx_t_1 = __Pyx_PyObject_IsTrue(__pyx_v_items); if (unlikely(__pyx_t_1 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 23; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (__pyx_t_1) {

    __pyx_t_2 = PyObject_GetAttr(((PyObject *)__pyx_v_self), __pyx_n_s__update); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 24; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __pyx_t_3 = PyTuple_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 24; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_INCREF(__pyx_v_items);
    PyTuple_SET_ITEM(__pyx_t_3, 0, __pyx_v_items);
    __Pyx_GIVEREF(__pyx_v_items);
    __pyx_t_4 = PyObject_Call(__pyx_t_2, ((PyObject *)__pyx_t_3), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 24; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
    __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
    goto __pyx_L3;
  }
  __pyx_L3:;

  __pyx_r = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.__cinit__", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = -1;
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static void __pyx_pw_8bintrees_8qavltree_8cAVLTree_3__dealloc__(PyObject *__pyx_v_self); 
static void __pyx_pw_8bintrees_8qavltree_8cAVLTree_3__dealloc__(PyObject *__pyx_v_self) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__dealloc__ (wrapper)", 0);
  __pyx_pf_8bintrees_8qavltree_8cAVLTree_2__dealloc__(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
}


static void __pyx_pf_8bintrees_8qavltree_8cAVLTree_2__dealloc__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__dealloc__", 0);

  ct_delete_tree(__pyx_v_self->_root);

  __Pyx_RefNannyFinishContext();
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_5count(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_5count(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("count (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_4count(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_4count(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("count", 0);

  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyInt_FromLong(__pyx_v_self->_count); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 31; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_r = __pyx_t_1;
  __pyx_t_1 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.count", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_7__getstate__(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_7__getstate__(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__getstate__ (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_6__getstate__(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_6__getstate__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("__getstate__", 0);

  __Pyx_XDECREF(__pyx_r);
  __pyx_t_1 = PyObject_GetAttr(((PyObject *)__pyx_v_self), __pyx_n_s__items); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 34; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = PyObject_Call(__pyx_t_1, ((PyObject *)__pyx_empty_tuple), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 34; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 34; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  PyTuple_SET_ITEM(__pyx_t_1, 0, __pyx_t_2);
  __Pyx_GIVEREF(__pyx_t_2);
  __pyx_t_2 = 0;
  __pyx_t_2 = PyObject_Call(((PyObject *)((PyObject*)(&PyDict_Type))), ((PyObject *)__pyx_t_1), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 34; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __pyx_r = __pyx_t_2;
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.__getstate__", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_9__setstate__(PyObject *__pyx_v_self, PyObject *__pyx_v_state); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_9__setstate__(PyObject *__pyx_v_self, PyObject *__pyx_v_state) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__setstate__ (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_8__setstate__(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self), ((PyObject *)__pyx_v_state));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_8__setstate__(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_state) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("__setstate__", 0);

  __pyx_t_1 = PyObject_GetAttr(((PyObject *)__pyx_v_self), __pyx_n_s__update); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 37; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_t_2 = PyTuple_New(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 37; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_INCREF(__pyx_v_state);
  PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_v_state);
  __Pyx_GIVEREF(__pyx_v_state);
  __pyx_t_3 = PyObject_Call(__pyx_t_1, ((PyObject *)__pyx_t_2), NULL); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 37; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_3);
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.__setstate__", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_11clear(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_11clear(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("clear (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_10clear(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_10clear(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("clear", 0);

  ct_delete_tree(__pyx_v_self->_root);

  __pyx_v_self->_count = 0;

  __pyx_v_self->_root = NULL;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_13get_value(PyObject *__pyx_v_self, PyObject *__pyx_v_key); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_13get_value(PyObject *__pyx_v_self, PyObject *__pyx_v_key) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("get_value (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_12get_value(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self), ((PyObject *)__pyx_v_key));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_12get_value(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key) {
  PyObject *__pyx_v_result = NULL;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1;
  int __pyx_t_2;
  PyObject *__pyx_t_3 = NULL;
  PyObject *__pyx_t_4 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("get_value", 0);

  __pyx_t_1 = ct_get_item(__pyx_v_self->_root, __pyx_v_key);
  __Pyx_INCREF(((PyObject *)__pyx_t_1));
  __pyx_v_result = ((PyObject *)__pyx_t_1);

  __pyx_t_2 = (__pyx_v_result == Py_None);
  if (__pyx_t_2) {

    __pyx_t_3 = PyTuple_New(1); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 47; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_INCREF(__pyx_v_key);
    PyTuple_SET_ITEM(__pyx_t_3, 0, __pyx_v_key);
    __Pyx_GIVEREF(__pyx_v_key);
    __pyx_t_4 = PyObject_Call(__pyx_builtin_KeyError, ((PyObject *)__pyx_t_3), NULL); if (unlikely(!__pyx_t_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 47; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __Pyx_DECREF(((PyObject *)__pyx_t_3)); __pyx_t_3 = 0;
    __Pyx_Raise(__pyx_t_4, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_4); __pyx_t_4 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 47; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
   {

    __Pyx_XDECREF(__pyx_r);
    __pyx_t_4 = __Pyx_GetItemInt(__pyx_v_result, 1, sizeof(long), PyInt_FromLong); if (!__pyx_t_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 49; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_4);
    __pyx_r = __pyx_t_4;
    __pyx_t_4 = 0;
    goto __pyx_L0;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_XDECREF(__pyx_t_4);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.get_value", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XDECREF(__pyx_v_result);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_15get_walker(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_15get_walker(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("get_walker (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_14get_walker(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_14get_walker(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  struct __pyx_obj_8bintrees_7cwalker_cWalker *__pyx_v_walker = 0;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("get_walker", 0);

  __pyx_t_1 = PyObject_Call(((PyObject *)((PyObject*)__pyx_ptype_8bintrees_7cwalker_cWalker)), ((PyObject *)__pyx_empty_tuple), NULL); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 53; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_v_walker = ((struct __pyx_obj_8bintrees_7cwalker_cWalker *)__pyx_t_1);
  __pyx_t_1 = 0;

  ((struct __pyx_vtabstruct_8bintrees_7cwalker_cWalker *)__pyx_v_walker->__pyx_vtab)->set_tree(__pyx_v_walker, __pyx_v_self->_root);

  __Pyx_XDECREF(__pyx_r);
  __Pyx_INCREF(((PyObject *)__pyx_v_walker));
  __pyx_r = ((PyObject *)__pyx_v_walker);
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.get_walker", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XDECREF((PyObject *)__pyx_v_walker);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_17insert(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_17insert(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_key = 0;
  PyObject *__pyx_v_value = 0;
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("insert (wrapper)", 0);
  {
    static PyObject **__pyx_pyargnames[] = {&__pyx_n_s__key,&__pyx_n_s__value,0};
    PyObject* values[2] = {0,0};
    if (unlikely(__pyx_kwds)) {
      Py_ssize_t kw_args;
      const Py_ssize_t pos_args = PyTuple_GET_SIZE(__pyx_args);
      switch (pos_args) {
        case  2: values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
        case  1: values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
        case  0: break;
        default: goto __pyx_L5_argtuple_error;
      }
      kw_args = PyDict_Size(__pyx_kwds);
      switch (pos_args) {
        case  0:
        if (likely((values[0] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__key)) != 0)) kw_args--;
        else goto __pyx_L5_argtuple_error;
        case  1:
        if (likely((values[1] = PyDict_GetItem(__pyx_kwds, __pyx_n_s__value)) != 0)) kw_args--;
        else {
          __Pyx_RaiseArgtupleInvalid("insert", 1, 2, 2, 1); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 57; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
        }
      }
      if (unlikely(kw_args > 0)) {
        if (unlikely(__Pyx_ParseOptionalKeywords(__pyx_kwds, __pyx_pyargnames, 0, values, pos_args, "insert") < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 57; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
      }
    } else if (PyTuple_GET_SIZE(__pyx_args) != 2) {
      goto __pyx_L5_argtuple_error;
    } else {
      values[0] = PyTuple_GET_ITEM(__pyx_args, 0);
      values[1] = PyTuple_GET_ITEM(__pyx_args, 1);
    }
    __pyx_v_key = values[0];
    __pyx_v_value = values[1];
  }
  goto __pyx_L4_argument_unpacking_done;
  __pyx_L5_argtuple_error:;
  __Pyx_RaiseArgtupleInvalid("insert", 1, 2, 2, PyTuple_GET_SIZE(__pyx_args)); {__pyx_filename = __pyx_f[0]; __pyx_lineno = 57; __pyx_clineno = __LINE__; goto __pyx_L3_error;}
  __pyx_L3_error:;
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.insert", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __Pyx_RefNannyFinishContext();
  return NULL;
  __pyx_L4_argument_unpacking_done:;
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_16insert(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self), __pyx_v_key, __pyx_v_value);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_16insert(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key, PyObject *__pyx_v_value) {
  PyObject *__pyx_v_res = NULL;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  PyObject *__pyx_t_1 = NULL;
  int __pyx_t_2;
  PyObject *__pyx_t_3 = NULL;
  int __pyx_t_4;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("insert", 0);

  __pyx_t_1 = PyInt_FromLong(avl_insert((&__pyx_v_self->_root), __pyx_v_key, __pyx_v_value)); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 58; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __pyx_v_res = __pyx_t_1;
  __pyx_t_1 = 0;

  __pyx_t_1 = PyObject_RichCompare(__pyx_v_res, __pyx_int_0, Py_LT); __Pyx_XGOTREF(__pyx_t_1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 59; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_t_2 = __Pyx_PyObject_IsTrue(__pyx_t_1); if (unlikely(__pyx_t_2 < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 59; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
  if (__pyx_t_2) {

    __pyx_t_1 = PyObject_Call(__pyx_builtin_MemoryError, ((PyObject *)__pyx_k_tuple_2), NULL); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 60; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_1);
    __Pyx_Raise(__pyx_t_1, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 60; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
   {

    __pyx_t_1 = PyInt_FromLong(__pyx_v_self->_count); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 62; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_1);
    __pyx_t_3 = PyNumber_InPlaceAdd(__pyx_t_1, __pyx_v_res); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 62; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    __pyx_t_4 = __Pyx_PyInt_AsInt(__pyx_t_3); if (unlikely((__pyx_t_4 == (int)-1) && PyErr_Occurred())) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 62; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    __pyx_v_self->_count = __pyx_t_4;
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.insert", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XDECREF(__pyx_v_res);
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_19remove(PyObject *__pyx_v_self, PyObject *__pyx_v_key); 
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_19remove(PyObject *__pyx_v_self, PyObject *__pyx_v_key) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("remove (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_18remove(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self), ((PyObject *)__pyx_v_key));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_18remove(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self, PyObject *__pyx_v_key) {
  int __pyx_v_result;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  PyObject *__pyx_t_3 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("remove", 0);

  __pyx_v_result = avl_remove((&__pyx_v_self->_root), __pyx_v_key);

  __pyx_t_1 = (__pyx_v_result == 0);
  if (__pyx_t_1) {

    __pyx_t_2 = PyTuple_New(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 68; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_INCREF(__pyx_v_key);
    PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_v_key);
    __Pyx_GIVEREF(__pyx_v_key);
    __pyx_t_3 = PyObject_Call(((PyObject *)((PyObject*)(&PyString_Type))), ((PyObject *)__pyx_t_2), NULL); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 68; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
    __pyx_t_2 = PyTuple_New(1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 68; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    PyTuple_SET_ITEM(__pyx_t_2, 0, __pyx_t_3);
    __Pyx_GIVEREF(__pyx_t_3);
    __pyx_t_3 = 0;
    __pyx_t_3 = PyObject_Call(__pyx_builtin_KeyError, ((PyObject *)__pyx_t_2), NULL); if (unlikely(!__pyx_t_3)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 68; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_3);
    __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
    __Pyx_Raise(__pyx_t_3, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_3); __pyx_t_3 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 68; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
   {

    __pyx_v_self->_count = (__pyx_v_self->_count - 1);
  }
  __pyx_L3:;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_XDECREF(__pyx_t_3);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.remove", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_21max_item(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static char __pyx_doc_8bintrees_8qavltree_8cAVLTree_20max_item[] = " Get item with max key of tree, raises ValueError if tree is empty. ";
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_21max_item(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("max_item (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_20max_item(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_20max_item(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  node_t *__pyx_v_node;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("max_item", 0);

  __pyx_v_node = ct_max_node(__pyx_v_self->_root);

  __pyx_t_1 = (__pyx_v_node == NULL);
  if (__pyx_t_1) {

    __pyx_t_2 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_4), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_Raise(__pyx_t_2, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
  __pyx_L3:;

  __Pyx_XDECREF(__pyx_r);
  __pyx_t_2 = PyTuple_New(2); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 78; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_INCREF(((PyObject *)__pyx_v_node->key));
  PyTuple_SET_ITEM(__pyx_t_2, 0, ((PyObject *)__pyx_v_node->key));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_node->key));
  __Pyx_INCREF(((PyObject *)__pyx_v_node->value));
  PyTuple_SET_ITEM(__pyx_t_2, 1, ((PyObject *)__pyx_v_node->value));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_node->value));
  __pyx_r = ((PyObject *)__pyx_t_2);
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.max_item", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_23min_item(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused); 
static char __pyx_doc_8bintrees_8qavltree_8cAVLTree_22min_item[] = " Get item with min key of tree, raises ValueError if tree is empty. ";
static PyObject *__pyx_pw_8bintrees_8qavltree_8cAVLTree_23min_item(PyObject *__pyx_v_self, CYTHON_UNUSED PyObject *unused) {
  PyObject *__pyx_r = 0;
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("min_item (wrapper)", 0);
  __pyx_r = __pyx_pf_8bintrees_8qavltree_8cAVLTree_22min_item(((struct __pyx_obj_8bintrees_8qavltree_cAVLTree *)__pyx_v_self));
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}


static PyObject *__pyx_pf_8bintrees_8qavltree_8cAVLTree_22min_item(struct __pyx_obj_8bintrees_8qavltree_cAVLTree *__pyx_v_self) {
  node_t *__pyx_v_node;
  PyObject *__pyx_r = NULL;
  __Pyx_RefNannyDeclarations
  int __pyx_t_1;
  PyObject *__pyx_t_2 = NULL;
  int __pyx_lineno = 0;
  const char *__pyx_filename = NULL;
  int __pyx_clineno = 0;
  __Pyx_RefNannySetupContext("min_item", 0);

  __pyx_v_node = ct_min_node(__pyx_v_self->_root);

  __pyx_t_1 = (__pyx_v_node == NULL);
  if (__pyx_t_1) {

    __pyx_t_2 = PyObject_Call(__pyx_builtin_ValueError, ((PyObject *)__pyx_k_tuple_5), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 85; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    __Pyx_GOTREF(__pyx_t_2);
    __Pyx_Raise(__pyx_t_2, 0, 0, 0);
    __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
    {__pyx_filename = __pyx_f[0]; __pyx_lineno = 85; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    goto __pyx_L3;
  }
  __pyx_L3:;

  __Pyx_XDECREF(__pyx_r);
  __pyx_t_2 = PyTuple_New(2); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 86; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_INCREF(((PyObject *)__pyx_v_node->key));
  PyTuple_SET_ITEM(__pyx_t_2, 0, ((PyObject *)__pyx_v_node->key));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_node->key));
  __Pyx_INCREF(((PyObject *)__pyx_v_node->value));
  PyTuple_SET_ITEM(__pyx_t_2, 1, ((PyObject *)__pyx_v_node->value));
  __Pyx_GIVEREF(((PyObject *)__pyx_v_node->value));
  __pyx_r = ((PyObject *)__pyx_t_2);
  __pyx_t_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; __Pyx_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_2);
  __Pyx_AddTraceback("bintrees.qavltree.cAVLTree.min_item", __pyx_clineno, __pyx_lineno, __pyx_filename);
  __pyx_r = NULL;
  __pyx_L0:;
  __Pyx_XGIVEREF(__pyx_r);
  __Pyx_RefNannyFinishContext();
  return __pyx_r;
}

static PyObject *__pyx_tp_new_8bintrees_8qavltree_cAVLTree(PyTypeObject *t, PyObject *a, PyObject *k) {
  PyObject *o = (*t->tp_alloc)(t, 0);
  if (!o) return 0;
  if (__pyx_pw_8bintrees_8qavltree_8cAVLTree_1__cinit__(o, a, k) < 0) {
    Py_DECREF(o); o = 0;
  }
  return o;
}

static void __pyx_tp_dealloc_8bintrees_8qavltree_cAVLTree(PyObject *o) {
  {
    PyObject *etype, *eval, *etb;
    PyErr_Fetch(&etype, &eval, &etb);
    ++Py_REFCNT(o);
    __pyx_pw_8bintrees_8qavltree_8cAVLTree_3__dealloc__(o);
    if (PyErr_Occurred()) PyErr_WriteUnraisable(o);
    --Py_REFCNT(o);
    PyErr_Restore(etype, eval, etb);
  }
  (*Py_TYPE(o)->tp_free)(o);
}

static PyMethodDef __pyx_methods_8bintrees_8qavltree_cAVLTree[] = {
  {__Pyx_NAMESTR("count"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_5count, METH_NOARGS, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("__getstate__"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_7__getstate__, METH_NOARGS, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("__setstate__"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_9__setstate__, METH_O, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("clear"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_11clear, METH_NOARGS, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("get_value"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_13get_value, METH_O, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("get_walker"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_15get_walker, METH_NOARGS, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("insert"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_17insert, METH_VARARGS|METH_KEYWORDS, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("remove"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_19remove, METH_O, __Pyx_DOCSTR(0)},
  {__Pyx_NAMESTR("max_item"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_21max_item, METH_NOARGS, __Pyx_DOCSTR(__pyx_doc_8bintrees_8qavltree_8cAVLTree_20max_item)},
  {__Pyx_NAMESTR("min_item"), (PyCFunction)__pyx_pw_8bintrees_8qavltree_8cAVLTree_23min_item, METH_NOARGS, __Pyx_DOCSTR(__pyx_doc_8bintrees_8qavltree_8cAVLTree_22min_item)},
  {0, 0, 0, 0}
};

static PyNumberMethods __pyx_tp_as_number_cAVLTree = {
  0, 
  0, 
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #else
  0, 
  #endif
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  0, 
  0, 
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  #if PY_VERSION_HEX >= 0x02050000
  0, 
  #endif
};

static PySequenceMethods __pyx_tp_as_sequence_cAVLTree = {
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
};

static PyMappingMethods __pyx_tp_as_mapping_cAVLTree = {
  0, 
  0, 
  0, 
};

static PyBufferProcs __pyx_tp_as_buffer_cAVLTree = {
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  #if PY_MAJOR_VERSION < 3
  0, 
  #endif
  #if PY_VERSION_HEX >= 0x02060000
  0, 
  #endif
  #if PY_VERSION_HEX >= 0x02060000
  0, 
  #endif
};

static PyTypeObject __pyx_type_8bintrees_8qavltree_cAVLTree = {
  PyVarObject_HEAD_INIT(0, 0)
  __Pyx_NAMESTR("bintrees.qavltree.cAVLTree"), 
  sizeof(struct __pyx_obj_8bintrees_8qavltree_cAVLTree), 
  0, 
  __pyx_tp_dealloc_8bintrees_8qavltree_cAVLTree, 
  0, 
  0, 
  0, 
  #if PY_MAJOR_VERSION < 3
  0, 
  #else
  0, 
  #endif
  0, 
  &__pyx_tp_as_number_cAVLTree, 
  &__pyx_tp_as_sequence_cAVLTree, 
  &__pyx_tp_as_mapping_cAVLTree, 
  0, 
  0, 
  0, 
  0, 
  0, 
  &__pyx_tp_as_buffer_cAVLTree, 
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_NEWBUFFER|Py_TPFLAGS_BASETYPE, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  __pyx_methods_8bintrees_8qavltree_cAVLTree, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  __pyx_tp_new_8bintrees_8qavltree_cAVLTree, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  #if PY_VERSION_HEX >= 0x02060000
  0, 
  #endif
};

static PyMethodDef __pyx_methods[] = {
  {0, 0, 0, 0}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef __pyx_moduledef = {
    PyModuleDef_HEAD_INIT,
    __Pyx_NAMESTR("qavltree"),
    0, 
    -1, 
    __pyx_methods ,
    NULL, 
    NULL, 
    NULL, 
    NULL 
};
#endif

static __Pyx_StringTabEntry __pyx_string_tab[] = {
  {&__pyx_kp_s_1, __pyx_k_1, sizeof(__pyx_k_1), 0, 0, 1, 0},
  {&__pyx_kp_s_3, __pyx_k_3, sizeof(__pyx_k_3), 0, 0, 1, 0},
  {&__pyx_n_s__KeyError, __pyx_k__KeyError, sizeof(__pyx_k__KeyError), 0, 0, 1, 1},
  {&__pyx_n_s__MemoryError, __pyx_k__MemoryError, sizeof(__pyx_k__MemoryError), 0, 0, 1, 1},
  {&__pyx_n_s__ValueError, __pyx_k__ValueError, sizeof(__pyx_k__ValueError), 0, 0, 1, 1},
  {&__pyx_n_s____all__, __pyx_k____all__, sizeof(__pyx_k____all__), 0, 0, 1, 1},
  {&__pyx_n_s____main__, __pyx_k____main__, sizeof(__pyx_k____main__), 0, 0, 1, 1},
  {&__pyx_n_s____test__, __pyx_k____test__, sizeof(__pyx_k____test__), 0, 0, 1, 1},
  {&__pyx_n_s__cAVLTree, __pyx_k__cAVLTree, sizeof(__pyx_k__cAVLTree), 0, 0, 1, 1},
  {&__pyx_n_s__cWalker, __pyx_k__cWalker, sizeof(__pyx_k__cWalker), 0, 0, 1, 1},
  {&__pyx_n_s__count, __pyx_k__count, sizeof(__pyx_k__count), 0, 0, 1, 1},
  {&__pyx_n_s__cwalker, __pyx_k__cwalker, sizeof(__pyx_k__cwalker), 0, 0, 1, 1},
  {&__pyx_n_s__items, __pyx_k__items, sizeof(__pyx_k__items), 0, 0, 1, 1},
  {&__pyx_n_s__key, __pyx_k__key, sizeof(__pyx_k__key), 0, 0, 1, 1},
  {&__pyx_n_s__property, __pyx_k__property, sizeof(__pyx_k__property), 0, 0, 1, 1},
  {&__pyx_n_s__update, __pyx_k__update, sizeof(__pyx_k__update), 0, 0, 1, 1},
  {&__pyx_n_s__value, __pyx_k__value, sizeof(__pyx_k__value), 0, 0, 1, 1},
  {0, 0, 0, 0, 0, 0, 0}
};
static int __Pyx_InitCachedBuiltins(void) {
  __pyx_builtin_property = __Pyx_GetName(__pyx_b, __pyx_n_s__property); if (!__pyx_builtin_property) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 29; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_KeyError = __Pyx_GetName(__pyx_b, __pyx_n_s__KeyError); if (!__pyx_builtin_KeyError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 47; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_MemoryError = __Pyx_GetName(__pyx_b, __pyx_n_s__MemoryError); if (!__pyx_builtin_MemoryError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 60; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_builtin_ValueError = __Pyx_GetName(__pyx_b, __pyx_n_s__ValueError); if (!__pyx_builtin_ValueError) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  return 0;
  __pyx_L1_error:;
  return -1;
}

static int __Pyx_InitCachedConstants(void) {
  __Pyx_RefNannyDeclarations
  __Pyx_RefNannySetupContext("__Pyx_InitCachedConstants", 0);

  __pyx_k_tuple_2 = PyTuple_New(1); if (unlikely(!__pyx_k_tuple_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 60; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_2);
  __Pyx_INCREF(((PyObject *)__pyx_kp_s_1));
  PyTuple_SET_ITEM(__pyx_k_tuple_2, 0, ((PyObject *)__pyx_kp_s_1));
  __Pyx_GIVEREF(((PyObject *)__pyx_kp_s_1));
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_2));

  __pyx_k_tuple_4 = PyTuple_New(1); if (unlikely(!__pyx_k_tuple_4)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 77; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_4);
  __Pyx_INCREF(((PyObject *)__pyx_kp_s_3));
  PyTuple_SET_ITEM(__pyx_k_tuple_4, 0, ((PyObject *)__pyx_kp_s_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_kp_s_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_4));

  __pyx_k_tuple_5 = PyTuple_New(1); if (unlikely(!__pyx_k_tuple_5)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 85; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_k_tuple_5);
  __Pyx_INCREF(((PyObject *)__pyx_kp_s_3));
  PyTuple_SET_ITEM(__pyx_k_tuple_5, 0, ((PyObject *)__pyx_kp_s_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_kp_s_3));
  __Pyx_GIVEREF(((PyObject *)__pyx_k_tuple_5));
  __Pyx_RefNannyFinishContext();
  return 0;
  __pyx_L1_error:;
  __Pyx_RefNannyFinishContext();
  return -1;
}

static int __Pyx_InitGlobals(void) {
  if (__Pyx_InitStrings(__pyx_string_tab) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  __pyx_int_0 = PyInt_FromLong(0); if (unlikely(!__pyx_int_0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  return 0;
  __pyx_L1_error:;
  return -1;
}

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initqavltree(void); 
PyMODINIT_FUNC initqavltree(void)
#else
PyMODINIT_FUNC PyInit_qavltree(void); 
PyMODINIT_FUNC PyInit_qavltree(void)
#endif
{
  PyObject *__pyx_t_1 = NULL;
  PyObject *__pyx_t_2 = NULL;
  __Pyx_RefNannyDeclarations
  #if CYTHON_REFNANNY
  __Pyx_RefNanny = __Pyx_RefNannyImportAPI("refnanny");
  if (!__Pyx_RefNanny) {
      PyErr_Clear();
      __Pyx_RefNanny = __Pyx_RefNannyImportAPI("Cython.Runtime.refnanny");
      if (!__Pyx_RefNanny)
          Py_FatalError("failed to import 'refnanny' module");
  }
  #endif
  __Pyx_RefNannySetupContext("PyMODINIT_FUNC PyInit_qavltree(void)", 0);
  if ( __Pyx_check_binary_version() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_empty_tuple = PyTuple_New(0); if (unlikely(!__pyx_empty_tuple)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_empty_bytes = PyBytes_FromStringAndSize("", 0); if (unlikely(!__pyx_empty_bytes)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #ifdef __Pyx_CyFunction_USED
  if (__Pyx_CyFunction_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  #ifdef __Pyx_FusedFunction_USED
  if (__pyx_FusedFunction_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  #ifdef __Pyx_Generator_USED
  if (__pyx_Generator_init() < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #endif
  
  
  #if defined(__PYX_FORCE_INIT_THREADS) && __PYX_FORCE_INIT_THREADS
  #ifdef WITH_THREAD 
  PyEval_InitThreads();
  #endif
  #endif
  
  #if PY_MAJOR_VERSION < 3
  __pyx_m = Py_InitModule4(__Pyx_NAMESTR("qavltree"), __pyx_methods, 0, 0, PYTHON_API_VERSION); Py_XINCREF(__pyx_m);
  #else
  __pyx_m = PyModule_Create(&__pyx_moduledef);
  #endif
  if (unlikely(!__pyx_m)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #if PY_MAJOR_VERSION >= 3
  {
    PyObject *modules = PyImport_GetModuleDict(); if (unlikely(!modules)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    if (!PyDict_GetItemString(modules, "bintrees.qavltree")) {
      if (unlikely(PyDict_SetItemString(modules, "bintrees.qavltree", __pyx_m) < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
    }
  }
  #endif
  __pyx_b = PyImport_AddModule(__Pyx_NAMESTR(__Pyx_BUILTIN_MODULE_NAME)); if (unlikely(!__pyx_b)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  #if CYTHON_COMPILING_IN_PYPY
  Py_INCREF(__pyx_b);
  #endif
  if (__Pyx_SetAttrString(__pyx_m, "__builtins__", __pyx_b) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  
  if (unlikely(__Pyx_InitGlobals() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (__pyx_module_is_main_bintrees__qavltree) {
    if (__Pyx_SetAttrString(__pyx_m, "__name__", __pyx_n_s____main__) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;};
  }
  
  if (unlikely(__Pyx_InitCachedBuiltins() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  
  if (unlikely(__Pyx_InitCachedConstants() < 0)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  
  
  
  
  if (PyType_Ready(&__pyx_type_8bintrees_8qavltree_cAVLTree) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 16; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  if (__Pyx_SetAttrString(__pyx_m, "cAVLTree", (PyObject *)&__pyx_type_8bintrees_8qavltree_cAVLTree) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 16; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_ptype_8bintrees_8qavltree_cAVLTree = &__pyx_type_8bintrees_8qavltree_cAVLTree;
  
  __pyx_ptype_8bintrees_7cwalker_cWalker = __Pyx_ImportType("bintrees.cwalker", "cWalker", sizeof(struct __pyx_obj_8bintrees_7cwalker_cWalker), 1); if (unlikely(!__pyx_ptype_8bintrees_7cwalker_cWalker)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 11; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __pyx_vtabptr_8bintrees_7cwalker_cWalker = (struct __pyx_vtabstruct_8bintrees_7cwalker_cWalker*)__Pyx_GetVtable(__pyx_ptype_8bintrees_7cwalker_cWalker->tp_dict); if (unlikely(!__pyx_vtabptr_8bintrees_7cwalker_cWalker)) {__pyx_filename = __pyx_f[1]; __pyx_lineno = 11; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  
  
  

  /* "bintrees\qavltree.pyx":9
 * # License: MIT License
 * 
 * __all__ = ['cAVLTree']             # <<<<<<<<<<<<<<
 * 
 * from cwalker import cWalker
 */
  __pyx_t_1 = PyList_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 9; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_INCREF(((PyObject *)__pyx_n_s__cAVLTree));
  PyList_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_n_s__cAVLTree));
  __Pyx_GIVEREF(((PyObject *)__pyx_n_s__cAVLTree));
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s____all__, ((PyObject *)__pyx_t_1)) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 9; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;

  __pyx_t_1 = PyList_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 11; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  __Pyx_INCREF(((PyObject *)__pyx_n_s__cWalker));
  PyList_SET_ITEM(__pyx_t_1, 0, ((PyObject *)__pyx_n_s__cWalker));
  __Pyx_GIVEREF(((PyObject *)__pyx_n_s__cWalker));
  __pyx_t_2 = __Pyx_Import(((PyObject *)__pyx_n_s__cwalker), ((PyObject *)__pyx_t_1), -1); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 11; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;

  __pyx_t_2 = __Pyx_GetName((PyObject *)__pyx_ptype_8bintrees_8qavltree_cAVLTree, __pyx_n_s__count); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 30; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __pyx_t_1 = PyTuple_New(1); if (unlikely(!__pyx_t_1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 29; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_1);
  PyTuple_SET_ITEM(__pyx_t_1, 0, __pyx_t_2);
  __Pyx_GIVEREF(__pyx_t_2);
  __pyx_t_2 = 0;
  __pyx_t_2 = PyObject_Call(__pyx_builtin_property, ((PyObject *)__pyx_t_1), NULL); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 29; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(__pyx_t_2);
  __Pyx_DECREF(((PyObject *)__pyx_t_1)); __pyx_t_1 = 0;
  if (PyDict_SetItem((PyObject *)__pyx_ptype_8bintrees_8qavltree_cAVLTree->tp_dict, __pyx_n_s__count, __pyx_t_2) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 30; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(__pyx_t_2); __pyx_t_2 = 0;
  PyType_Modified(__pyx_ptype_8bintrees_8qavltree_cAVLTree);

  __pyx_t_2 = PyDict_New(); if (unlikely(!__pyx_t_2)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_GOTREF(((PyObject *)__pyx_t_2));
  if (PyObject_SetAttr(__pyx_m, __pyx_n_s____test__, ((PyObject *)__pyx_t_2)) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 1; __pyx_clineno = __LINE__; goto __pyx_L1_error;}
  __Pyx_DECREF(((PyObject *)__pyx_t_2)); __pyx_t_2 = 0;
  goto __pyx_L0;
  __pyx_L1_error:;
  __Pyx_XDECREF(__pyx_t_1);
  __Pyx_XDECREF(__pyx_t_2);
  if (__pyx_m) {
    __Pyx_AddTraceback("init bintrees.qavltree", __pyx_clineno, __pyx_lineno, __pyx_filename);
    Py_DECREF(__pyx_m); __pyx_m = 0;
  } else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_ImportError, "init bintrees.qavltree");
  }
  __pyx_L0:;
  __Pyx_RefNannyFinishContext();
  #if PY_MAJOR_VERSION < 3
  return;
  #else
  return __pyx_m;
  #endif
}

#if CYTHON_REFNANNY
static __Pyx_RefNannyAPIStruct *__Pyx_RefNannyImportAPI(const char *modname) {
    PyObject *m = NULL, *p = NULL;
    void *r = NULL;
    m = PyImport_ImportModule((char *)modname);
    if (!m) goto end;
    p = PyObject_GetAttrString(m, (char *)"RefNannyAPI");
    if (!p) goto end;
    r = PyLong_AsVoidPtr(p);
end:
    Py_XDECREF(p);
    Py_XDECREF(m);
    return (__Pyx_RefNannyAPIStruct *)r;
}
#endif 

static PyObject *__Pyx_GetName(PyObject *dict, PyObject *name) {
    PyObject *result;
    result = PyObject_GetAttr(dict, name);
    if (!result) {
        if (dict != __pyx_b) {
            PyErr_Clear();
            result = PyObject_GetAttr(__pyx_b, name);
        }
        if (!result) {
            PyErr_SetObject(PyExc_NameError, name);
        }
    }
    return result;
}

static void __Pyx_RaiseDoubleKeywordsError(
    const char* func_name,
    PyObject* kw_name)
{
    PyErr_Format(PyExc_TypeError,
        #if PY_MAJOR_VERSION >= 3
        "%s() got multiple values for keyword argument '%U'", func_name, kw_name);
        #else
        "%s() got multiple values for keyword argument '%s'", func_name,
        PyString_AsString(kw_name));
        #endif
}

static int __Pyx_ParseOptionalKeywords(
    PyObject *kwds,
    PyObject **argnames[],
    PyObject *kwds2,
    PyObject *values[],
    Py_ssize_t num_pos_args,
    const char* function_name)
{
    PyObject *key = 0, *value = 0;
    Py_ssize_t pos = 0;
    PyObject*** name;
    PyObject*** first_kw_arg = argnames + num_pos_args;
    while (PyDict_Next(kwds, &pos, &key, &value)) {
        name = first_kw_arg;
        while (*name && (**name != key)) name++;
        if (*name) {
            values[name-argnames] = value;
            continue;
        }
        name = first_kw_arg;
        #if PY_MAJOR_VERSION < 3
        if (likely(PyString_CheckExact(key)) || likely(PyString_Check(key))) {
            while (*name) {
                if ((CYTHON_COMPILING_IN_PYPY || PyString_GET_SIZE(**name) == PyString_GET_SIZE(key))
                        && _PyString_Eq(**name, key)) {
                    values[name-argnames] = value;
                    break;
                }
                name++;
            }
            if (*name) continue;
            else {
                PyObject*** argname = argnames;
                while (argname != first_kw_arg) {
                    if ((**argname == key) || (
                            (CYTHON_COMPILING_IN_PYPY || PyString_GET_SIZE(**argname) == PyString_GET_SIZE(key))
                             && _PyString_Eq(**argname, key))) {
                        goto arg_passed_twice;
                    }
                    argname++;
                }
            }
        } else
        #endif
        if (likely(PyUnicode_Check(key))) {
            while (*name) {
                int cmp = (**name == key) ? 0 :
                #if !CYTHON_COMPILING_IN_PYPY && PY_MAJOR_VERSION >= 3
                    (PyUnicode_GET_SIZE(**name) != PyUnicode_GET_SIZE(key)) ? 1 :
                #endif
                    PyUnicode_Compare(**name, key);
                if (cmp < 0 && unlikely(PyErr_Occurred())) goto bad;
                if (cmp == 0) {
                    values[name-argnames] = value;
                    break;
                }
                name++;
            }
            if (*name) continue;
            else {
                PyObject*** argname = argnames;
                while (argname != first_kw_arg) {
                    int cmp = (**argname == key) ? 0 :
                    #if !CYTHON_COMPILING_IN_PYPY && PY_MAJOR_VERSION >= 3
                        (PyUnicode_GET_SIZE(**argname) != PyUnicode_GET_SIZE(key)) ? 1 :
                    #endif
                        PyUnicode_Compare(**argname, key);
                    if (cmp < 0 && unlikely(PyErr_Occurred())) goto bad;
                    if (cmp == 0) goto arg_passed_twice;
                    argname++;
                }
            }
        } else
            goto invalid_keyword_type;
        if (kwds2) {
            if (unlikely(PyDict_SetItem(kwds2, key, value))) goto bad;
        } else {
            goto invalid_keyword;
        }
    }
    return 0;
arg_passed_twice:
    __Pyx_RaiseDoubleKeywordsError(function_name, key);
    goto bad;
invalid_keyword_type:
    PyErr_Format(PyExc_TypeError,
        "%s() keywords must be strings", function_name);
    goto bad;
invalid_keyword:
    PyErr_Format(PyExc_TypeError,
    #if PY_MAJOR_VERSION < 3
        "%s() got an unexpected keyword argument '%s'",
        function_name, PyString_AsString(key));
    #else
        "%s() got an unexpected keyword argument '%U'",
        function_name, key);
    #endif
bad:
    return -1;
}

static void __Pyx_RaiseArgtupleInvalid(
    const char* func_name,
    int exact,
    Py_ssize_t num_min,
    Py_ssize_t num_max,
    Py_ssize_t num_found)
{
    Py_ssize_t num_expected;
    const char *more_or_less;
    if (num_found < num_min) {
        num_expected = num_min;
        more_or_less = "at least";
    } else {
        num_expected = num_max;
        more_or_less = "at most";
    }
    if (exact) {
        more_or_less = "exactly";
    }
    PyErr_Format(PyExc_TypeError,
                 "%s() takes %s %" CYTHON_FORMAT_SSIZE_T "d positional argument%s (%" CYTHON_FORMAT_SSIZE_T "d given)",
                 func_name, more_or_less, num_expected,
                 (num_expected == 1) ? "" : "s", num_found);
}

static CYTHON_INLINE void __Pyx_ErrRestore(PyObject *type, PyObject *value, PyObject *tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyObject *tmp_type, *tmp_value, *tmp_tb;
    PyThreadState *tstate = PyThreadState_GET();
    tmp_type = tstate->curexc_type;
    tmp_value = tstate->curexc_value;
    tmp_tb = tstate->curexc_traceback;
    tstate->curexc_type = type;
    tstate->curexc_value = value;
    tstate->curexc_traceback = tb;
    Py_XDECREF(tmp_type);
    Py_XDECREF(tmp_value);
    Py_XDECREF(tmp_tb);
#else
    PyErr_Restore(type, value, tb);
#endif
}
static CYTHON_INLINE void __Pyx_ErrFetch(PyObject **type, PyObject **value, PyObject **tb) {
#if CYTHON_COMPILING_IN_CPYTHON
    PyThreadState *tstate = PyThreadState_GET();
    *type = tstate->curexc_type;
    *value = tstate->curexc_value;
    *tb = tstate->curexc_traceback;
    tstate->curexc_type = 0;
    tstate->curexc_value = 0;
    tstate->curexc_traceback = 0;
#else
    PyErr_Fetch(type, value, tb);
#endif
}

#if PY_MAJOR_VERSION < 3
static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb,
                        CYTHON_UNUSED PyObject *cause) {
    Py_XINCREF(type);
    if (!value || value == Py_None)
        value = NULL;
    else
        Py_INCREF(value);
    if (!tb || tb == Py_None)
        tb = NULL;
    else {
        Py_INCREF(tb);
        if (!PyTraceBack_Check(tb)) {
            PyErr_SetString(PyExc_TypeError,
                "raise: arg 3 must be a traceback or None");
            goto raise_error;
        }
    }
    #if PY_VERSION_HEX < 0x02050000
    if (PyClass_Check(type)) {
    #else
    if (PyType_Check(type)) {
    #endif
#if CYTHON_COMPILING_IN_PYPY
        if (!value) {
            Py_INCREF(Py_None);
            value = Py_None;
        }
#endif
        PyErr_NormalizeException(&type, &value, &tb);
    } else {
        if (value) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto raise_error;
        }
        value = type;
        #if PY_VERSION_HEX < 0x02050000
            if (PyInstance_Check(type)) {
                type = (PyObject*) ((PyInstanceObject*)type)->in_class;
                Py_INCREF(type);
            }
            else {
                type = 0;
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception must be an old-style class or instance");
                goto raise_error;
            }
        #else
            type = (PyObject*) Py_TYPE(type);
            Py_INCREF(type);
            if (!PyType_IsSubtype((PyTypeObject *)type, (PyTypeObject *)PyExc_BaseException)) {
                PyErr_SetString(PyExc_TypeError,
                    "raise: exception class must be a subclass of BaseException");
                goto raise_error;
            }
        #endif
    }
    __Pyx_ErrRestore(type, value, tb);
    return;
raise_error:
    Py_XDECREF(value);
    Py_XDECREF(type);
    Py_XDECREF(tb);
    return;
}
#else 
static void __Pyx_Raise(PyObject *type, PyObject *value, PyObject *tb, PyObject *cause) {
    PyObject* owned_instance = NULL;
    if (tb == Py_None) {
        tb = 0;
    } else if (tb && !PyTraceBack_Check(tb)) {
        PyErr_SetString(PyExc_TypeError,
            "raise: arg 3 must be a traceback or None");
        goto bad;
    }
    if (value == Py_None)
        value = 0;
    if (PyExceptionInstance_Check(type)) {
        if (value) {
            PyErr_SetString(PyExc_TypeError,
                "instance exception may not have a separate value");
            goto bad;
        }
        value = type;
        type = (PyObject*) Py_TYPE(value);
    } else if (PyExceptionClass_Check(type)) {
        PyObject *args;
        if (!value)
            args = PyTuple_New(0);
        else if (PyTuple_Check(value)) {
            Py_INCREF(value);
            args = value;
        }
        else
            args = PyTuple_Pack(1, value);
        if (!args)
            goto bad;
        owned_instance = PyEval_CallObject(type, args);
        Py_DECREF(args);
        if (!owned_instance)
            goto bad;
        value = owned_instance;
        if (!PyExceptionInstance_Check(value)) {
            PyErr_Format(PyExc_TypeError,
                         "calling %R should have returned an instance of "
                         "BaseException, not %R",
                         type, Py_TYPE(value));
            goto bad;
        }
    } else {
        PyErr_SetString(PyExc_TypeError,
            "raise: exception class must be a subclass of BaseException");
        goto bad;
    }
    if (cause && cause != Py_None) {
        PyObject *fixed_cause;
        if (PyExceptionClass_Check(cause)) {
            fixed_cause = PyObject_CallObject(cause, NULL);
            if (fixed_cause == NULL)
                goto bad;
        }
        else if (PyExceptionInstance_Check(cause)) {
            fixed_cause = cause;
            Py_INCREF(fixed_cause);
        }
        else {
            PyErr_SetString(PyExc_TypeError,
                            "exception causes must derive from "
                            "BaseException");
            goto bad;
        }
        PyException_SetCause(value, fixed_cause);
    }
    PyErr_SetObject(type, value);
    if (tb) {
        PyThreadState *tstate = PyThreadState_GET();
        PyObject* tmp_tb = tstate->curexc_traceback;
        if (tb != tmp_tb) {
            Py_INCREF(tb);
            tstate->curexc_traceback = tb;
            Py_XDECREF(tmp_tb);
        }
    }
bad:
    Py_XDECREF(owned_instance);
    return;
}
#endif

static PyObject *__Pyx_Import(PyObject *name, PyObject *from_list, long level) {
    PyObject *py_import = 0;
    PyObject *empty_list = 0;
    PyObject *module = 0;
    PyObject *global_dict = 0;
    PyObject *empty_dict = 0;
    PyObject *list;
    py_import = __Pyx_GetAttrString(__pyx_b, "__import__");
    if (!py_import)
        goto bad;
    if (from_list)
        list = from_list;
    else {
        empty_list = PyList_New(0);
        if (!empty_list)
            goto bad;
        list = empty_list;
    }
    global_dict = PyModule_GetDict(__pyx_m);
    if (!global_dict)
        goto bad;
    empty_dict = PyDict_New();
    if (!empty_dict)
        goto bad;
    #if PY_VERSION_HEX >= 0x02050000
    {
        #if PY_MAJOR_VERSION >= 3
        if (level == -1) {
            if (strchr(__Pyx_MODULE_NAME, '.')) {
                
                PyObject *py_level = PyInt_FromLong(1);
                if (!py_level)
                    goto bad;
                module = PyObject_CallFunctionObjArgs(py_import,
                    name, global_dict, empty_dict, list, py_level, NULL);
                Py_DECREF(py_level);
                if (!module) {
                    if (!PyErr_ExceptionMatches(PyExc_ImportError))
                        goto bad;
                    PyErr_Clear();
                }
            }
            level = 0; 
        }
        #endif
        if (!module) {
            PyObject *py_level = PyInt_FromLong(level);
            if (!py_level)
                goto bad;
            module = PyObject_CallFunctionObjArgs(py_import,
                name, global_dict, empty_dict, list, py_level, NULL);
            Py_DECREF(py_level);
        }
    }
    #else
    if (level>0) {
        PyErr_SetString(PyExc_RuntimeError, "Relative import is not supported for Python <=2.4.");
        goto bad;
    }
    module = PyObject_CallFunctionObjArgs(py_import,
        name, global_dict, empty_dict, list, NULL);
    #endif
bad:
    Py_XDECREF(empty_list);
    Py_XDECREF(py_import);
    Py_XDECREF(empty_dict);
    return module;
}

static CYTHON_INLINE unsigned char __Pyx_PyInt_AsUnsignedChar(PyObject* x) {
    const unsigned char neg_one = (unsigned char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned char" :
                    "value too large to convert to unsigned char");
            }
            return (unsigned char)-1;
        }
        return (unsigned char)val;
    }
    return (unsigned char)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned short __Pyx_PyInt_AsUnsignedShort(PyObject* x) {
    const unsigned short neg_one = (unsigned short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned short" :
                    "value too large to convert to unsigned short");
            }
            return (unsigned short)-1;
        }
        return (unsigned short)val;
    }
    return (unsigned short)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE unsigned int __Pyx_PyInt_AsUnsignedInt(PyObject* x) {
    const unsigned int neg_one = (unsigned int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(unsigned int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(unsigned int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to unsigned int" :
                    "value too large to convert to unsigned int");
            }
            return (unsigned int)-1;
        }
        return (unsigned int)val;
    }
    return (unsigned int)__Pyx_PyInt_AsUnsignedLong(x);
}

static CYTHON_INLINE char __Pyx_PyInt_AsChar(PyObject* x) {
    const char neg_one = (char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to char" :
                    "value too large to convert to char");
            }
            return (char)-1;
        }
        return (char)val;
    }
    return (char)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE short __Pyx_PyInt_AsShort(PyObject* x) {
    const short neg_one = (short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to short" :
                    "value too large to convert to short");
            }
            return (short)-1;
        }
        return (short)val;
    }
    return (short)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsInt(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE signed char __Pyx_PyInt_AsSignedChar(PyObject* x) {
    const signed char neg_one = (signed char)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed char) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed char)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed char" :
                    "value too large to convert to signed char");
            }
            return (signed char)-1;
        }
        return (signed char)val;
    }
    return (signed char)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed short __Pyx_PyInt_AsSignedShort(PyObject* x) {
    const signed short neg_one = (signed short)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed short) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed short)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed short" :
                    "value too large to convert to signed short");
            }
            return (signed short)-1;
        }
        return (signed short)val;
    }
    return (signed short)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE signed int __Pyx_PyInt_AsSignedInt(PyObject* x) {
    const signed int neg_one = (signed int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(signed int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(signed int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to signed int" :
                    "value too large to convert to signed int");
            }
            return (signed int)-1;
        }
        return (signed int)val;
    }
    return (signed int)__Pyx_PyInt_AsSignedLong(x);
}

static CYTHON_INLINE int __Pyx_PyInt_AsLongDouble(PyObject* x) {
    const int neg_one = (int)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
    if (sizeof(int) < sizeof(long)) {
        long val = __Pyx_PyInt_AsLong(x);
        if (unlikely(val != (long)(int)val)) {
            if (!unlikely(val == -1 && PyErr_Occurred())) {
                PyErr_SetString(PyExc_OverflowError,
                    (is_unsigned && unlikely(val < 0)) ?
                    "can't convert negative value to int" :
                    "value too large to convert to int");
            }
            return (int)-1;
        }
        return (int)val;
    }
    return (int)__Pyx_PyInt_AsLong(x);
}

static CYTHON_INLINE unsigned long __Pyx_PyInt_AsUnsignedLong(PyObject* x) {
    const unsigned long neg_one = (unsigned long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned long");
            return (unsigned long)-1;
        }
        return (unsigned long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned long");
                return (unsigned long)-1;
            }
            return (unsigned long)PyLong_AsUnsignedLong(x);
        } else {
            return (unsigned long)PyLong_AsLong(x);
        }
    } else {
        unsigned long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned long)-1;
        val = __Pyx_PyInt_AsUnsignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE unsigned PY_LONG_LONG __Pyx_PyInt_AsUnsignedLongLong(PyObject* x) {
    const unsigned PY_LONG_LONG neg_one = (unsigned PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to unsigned PY_LONG_LONG");
            return (unsigned PY_LONG_LONG)-1;
        }
        return (unsigned PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to unsigned PY_LONG_LONG");
                return (unsigned PY_LONG_LONG)-1;
            }
            return (unsigned PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
            return (unsigned PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        unsigned PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (unsigned PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsUnsignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE long __Pyx_PyInt_AsLong(PyObject* x) {
    const long neg_one = (long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to long");
            return (long)-1;
        }
        return (long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to long");
                return (long)-1;
            }
            return (long)PyLong_AsUnsignedLong(x);
        } else {
            return (long)PyLong_AsLong(x);
        }
    } else {
        long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (long)-1;
        val = __Pyx_PyInt_AsLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE PY_LONG_LONG __Pyx_PyInt_AsLongLong(PyObject* x) {
    const PY_LONG_LONG neg_one = (PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to PY_LONG_LONG");
            return (PY_LONG_LONG)-1;
        }
        return (PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to PY_LONG_LONG");
                return (PY_LONG_LONG)-1;
            }
            return (PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
            return (PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE signed long __Pyx_PyInt_AsSignedLong(PyObject* x) {
    const signed long neg_one = (signed long)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed long");
            return (signed long)-1;
        }
        return (signed long)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed long");
                return (signed long)-1;
            }
            return (signed long)PyLong_AsUnsignedLong(x);
        } else {
            return (signed long)PyLong_AsLong(x);
        }
    } else {
        signed long val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed long)-1;
        val = __Pyx_PyInt_AsSignedLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static CYTHON_INLINE signed PY_LONG_LONG __Pyx_PyInt_AsSignedLongLong(PyObject* x) {
    const signed PY_LONG_LONG neg_one = (signed PY_LONG_LONG)-1, const_zero = 0;
    const int is_unsigned = neg_one > const_zero;
#if PY_VERSION_HEX < 0x03000000
    if (likely(PyInt_Check(x))) {
        long val = PyInt_AS_LONG(x);
        if (is_unsigned && unlikely(val < 0)) {
            PyErr_SetString(PyExc_OverflowError,
                            "can't convert negative value to signed PY_LONG_LONG");
            return (signed PY_LONG_LONG)-1;
        }
        return (signed PY_LONG_LONG)val;
    } else
#endif
    if (likely(PyLong_Check(x))) {
        if (is_unsigned) {
            if (unlikely(Py_SIZE(x) < 0)) {
                PyErr_SetString(PyExc_OverflowError,
                                "can't convert negative value to signed PY_LONG_LONG");
                return (signed PY_LONG_LONG)-1;
            }
            return (signed PY_LONG_LONG)PyLong_AsUnsignedLongLong(x);
        } else {
            return (signed PY_LONG_LONG)PyLong_AsLongLong(x);
        }
    } else {
        signed PY_LONG_LONG val;
        PyObject *tmp = __Pyx_PyNumber_Int(x);
        if (!tmp) return (signed PY_LONG_LONG)-1;
        val = __Pyx_PyInt_AsSignedLongLong(tmp);
        Py_DECREF(tmp);
        return val;
    }
}

static int __Pyx_check_binary_version(void) {
    char ctversion[4], rtversion[4];
    PyOS_snprintf(ctversion, 4, "%d.%d", PY_MAJOR_VERSION, PY_MINOR_VERSION);
    PyOS_snprintf(rtversion, 4, "%s", Py_GetVersion());
    if (ctversion[0] != rtversion[0] || ctversion[2] != rtversion[2]) {
        char message[200];
        PyOS_snprintf(message, sizeof(message),
                      "compiletime version %s of module '%.100s' "
                      "does not match runtime version %s",
                      ctversion, __Pyx_MODULE_NAME, rtversion);
        #if PY_VERSION_HEX < 0x02050000
        return PyErr_Warn(NULL, message);
        #else
        return PyErr_WarnEx(NULL, message, 1);
        #endif
    }
    return 0;
}

#ifndef __PYX_HAVE_RT_ImportModule
#define __PYX_HAVE_RT_ImportModule
static PyObject *__Pyx_ImportModule(const char *name) {
    PyObject *py_name = 0;
    PyObject *py_module = 0;
    py_name = __Pyx_PyIdentifier_FromString(name);
    if (!py_name)
        goto bad;
    py_module = PyImport_Import(py_name);
    Py_DECREF(py_name);
    return py_module;
bad:
    Py_XDECREF(py_name);
    return 0;
}
#endif

#ifndef __PYX_HAVE_RT_ImportType
#define __PYX_HAVE_RT_ImportType
static PyTypeObject *__Pyx_ImportType(const char *module_name, const char *class_name,
    size_t size, int strict)
{
    PyObject *py_module = 0;
    PyObject *result = 0;
    PyObject *py_name = 0;
    char warning[200];
    py_module = __Pyx_ImportModule(module_name);
    if (!py_module)
        goto bad;
    py_name = __Pyx_PyIdentifier_FromString(class_name);
    if (!py_name)
        goto bad;
    result = PyObject_GetAttr(py_module, py_name);
    Py_DECREF(py_name);
    py_name = 0;
    Py_DECREF(py_module);
    py_module = 0;
    if (!result)
        goto bad;
    if (!PyType_Check(result)) {
        PyErr_Format(PyExc_TypeError,
            "%s.%s is not a type object",
            module_name, class_name);
        goto bad;
    }
    if (!strict && (size_t)((PyTypeObject *)result)->tp_basicsize > size) {
        PyOS_snprintf(warning, sizeof(warning),
            "%s.%s size changed, may indicate binary incompatibility",
            module_name, class_name);
        #if PY_VERSION_HEX < 0x02050000
        if (PyErr_Warn(NULL, warning) < 0) goto bad;
        #else
        if (PyErr_WarnEx(NULL, warning, 0) < 0) goto bad;
        #endif
    }
    else if ((size_t)((PyTypeObject *)result)->tp_basicsize != size) {
        PyErr_Format(PyExc_ValueError,
            "%s.%s has the wrong size, try recompiling",
            module_name, class_name);
        goto bad;
    }
    return (PyTypeObject *)result;
bad:
    Py_XDECREF(py_module);
    Py_XDECREF(result);
    return NULL;
}
#endif

static void* __Pyx_GetVtable(PyObject *dict) {
    void* ptr;
    PyObject *ob = PyMapping_GetItemString(dict, (char *)"__pyx_vtable__");
    if (!ob)
        goto bad;
#if PY_VERSION_HEX >= 0x02070000 && !(PY_MAJOR_VERSION==3&&PY_MINOR_VERSION==0)
    ptr = PyCapsule_GetPointer(ob, 0);
#else
    ptr = PyCObject_AsVoidPtr(ob);
#endif
    if (!ptr && !PyErr_Occurred())
        PyErr_SetString(PyExc_RuntimeError, "invalid vtable found for imported type");
    Py_DECREF(ob);
    return ptr;
bad:
    Py_XDECREF(ob);
    return NULL;
}

static int __pyx_bisect_code_objects(__Pyx_CodeObjectCacheEntry* entries, int count, int code_line) {
    int start = 0, mid = 0, end = count - 1;
    if (end >= 0 && code_line > entries[end].code_line) {
        return count;
    }
    while (start < end) {
        mid = (start + end) / 2;
        if (code_line < entries[mid].code_line) {
            end = mid;
        } else if (code_line > entries[mid].code_line) {
             start = mid + 1;
        } else {
            return mid;
        }
    }
    if (code_line <= entries[mid].code_line) {
        return mid;
    } else {
        return mid + 1;
    }
}
static PyCodeObject *__pyx_find_code_object(int code_line) {
    PyCodeObject* code_object;
    int pos;
    if (unlikely(!code_line) || unlikely(!__pyx_code_cache.entries)) {
        return NULL;
    }
    pos = __pyx_bisect_code_objects(__pyx_code_cache.entries, __pyx_code_cache.count, code_line);
    if (unlikely(pos >= __pyx_code_cache.count) || unlikely(__pyx_code_cache.entries[pos].code_line != code_line)) {
        return NULL;
    }
    code_object = __pyx_code_cache.entries[pos].code_object;
    Py_INCREF(code_object);
    return code_object;
}
static void __pyx_insert_code_object(int code_line, PyCodeObject* code_object) {
    int pos, i;
    __Pyx_CodeObjectCacheEntry* entries = __pyx_code_cache.entries;
    if (unlikely(!code_line)) {
        return;
    }
    if (unlikely(!entries)) {
        entries = (__Pyx_CodeObjectCacheEntry*)PyMem_Malloc(64*sizeof(__Pyx_CodeObjectCacheEntry));
        if (likely(entries)) {
            __pyx_code_cache.entries = entries;
            __pyx_code_cache.max_count = 64;
            __pyx_code_cache.count = 1;
            entries[0].code_line = code_line;
            entries[0].code_object = code_object;
            Py_INCREF(code_object);
        }
        return;
    }
    pos = __pyx_bisect_code_objects(__pyx_code_cache.entries, __pyx_code_cache.count, code_line);
    if ((pos < __pyx_code_cache.count) && unlikely(__pyx_code_cache.entries[pos].code_line == code_line)) {
        PyCodeObject* tmp = entries[pos].code_object;
        entries[pos].code_object = code_object;
        Py_DECREF(tmp);
        return;
    }
    if (__pyx_code_cache.count == __pyx_code_cache.max_count) {
        int new_max = __pyx_code_cache.max_count + 64;
        entries = (__Pyx_CodeObjectCacheEntry*)PyMem_Realloc(
            __pyx_code_cache.entries, new_max*sizeof(__Pyx_CodeObjectCacheEntry));
        if (unlikely(!entries)) {
            return;
        }
        __pyx_code_cache.entries = entries;
        __pyx_code_cache.max_count = new_max;
    }
    for (i=__pyx_code_cache.count; i>pos; i--) {
        entries[i] = entries[i-1];
    }
    entries[pos].code_line = code_line;
    entries[pos].code_object = code_object;
    __pyx_code_cache.count++;
    Py_INCREF(code_object);
}

#include "compile.h"
#include "frameobject.h"
#include "traceback.h"
static PyCodeObject* __Pyx_CreateCodeObjectForTraceback(
            const char *funcname, int c_line,
            int py_line, const char *filename) {
    PyCodeObject *py_code = 0;
    PyObject *py_srcfile = 0;
    PyObject *py_funcname = 0;
    #if PY_MAJOR_VERSION < 3
    py_srcfile = PyString_FromString(filename);
    #else
    py_srcfile = PyUnicode_FromString(filename);
    #endif
    if (!py_srcfile) goto bad;
    if (c_line) {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, c_line);
        #else
        py_funcname = PyUnicode_FromFormat( "%s (%s:%d)", funcname, __pyx_cfilenm, c_line);
        #endif
    }
    else {
        #if PY_MAJOR_VERSION < 3
        py_funcname = PyString_FromString(funcname);
        #else
        py_funcname = PyUnicode_FromString(funcname);
        #endif
    }
    if (!py_funcname) goto bad;
    py_code = __Pyx_PyCode_New(
        0,            
        0,            
        0,            
        0,            
        0,            
        __pyx_empty_bytes, 
        __pyx_empty_tuple, 
        __pyx_empty_tuple, 
        __pyx_empty_tuple, 
        __pyx_empty_tuple, 
        __pyx_empty_tuple, 
        py_srcfile,   
        py_funcname,  
        py_line,      
        __pyx_empty_bytes  
    );
    Py_DECREF(py_srcfile);
    Py_DECREF(py_funcname);
    return py_code;
bad:
    Py_XDECREF(py_srcfile);
    Py_XDECREF(py_funcname);
    return NULL;
}
static void __Pyx_AddTraceback(const char *funcname, int c_line,
                               int py_line, const char *filename) {
    PyCodeObject *py_code = 0;
    PyObject *py_globals = 0;
    PyFrameObject *py_frame = 0;
    py_code = __pyx_find_code_object(c_line ? c_line : py_line);
    if (!py_code) {
        py_code = __Pyx_CreateCodeObjectForTraceback(
            funcname, c_line, py_line, filename);
        if (!py_code) goto bad;
        __pyx_insert_code_object(c_line ? c_line : py_line, py_code);
    }
    py_globals = PyModule_GetDict(__pyx_m);
    if (!py_globals) goto bad;
    py_frame = PyFrame_New(
        PyThreadState_GET(), 
        py_code,             
        py_globals,          
        0                    
    );
    if (!py_frame) goto bad;
    py_frame->f_lineno = py_line;
    PyTraceBack_Here(py_frame);
bad:
    Py_XDECREF(py_code);
    Py_XDECREF(py_frame);
}

static int __Pyx_InitStrings(__Pyx_StringTabEntry *t) {
    while (t->p) {
        #if PY_MAJOR_VERSION < 3
        if (t->is_unicode) {
            *t->p = PyUnicode_DecodeUTF8(t->s, t->n - 1, NULL);
        } else if (t->intern) {
            *t->p = PyString_InternFromString(t->s);
        } else {
            *t->p = PyString_FromStringAndSize(t->s, t->n - 1);
        }
        #else  
        if (t->is_unicode | t->is_str) {
            if (t->intern) {
                *t->p = PyUnicode_InternFromString(t->s);
            } else if (t->encoding) {
                *t->p = PyUnicode_Decode(t->s, t->n - 1, t->encoding, NULL);
            } else {
                *t->p = PyUnicode_FromStringAndSize(t->s, t->n - 1);
            }
        } else {
            *t->p = PyBytes_FromStringAndSize(t->s, t->n - 1);
        }
        #endif
        if (!*t->p)
            return -1;
        ++t;
    }
    return 0;
}



static CYTHON_INLINE int __Pyx_PyObject_IsTrue(PyObject* x) {
   int is_true = x == Py_True;
   if (is_true | (x == Py_False) | (x == Py_None)) return is_true;
   else return PyObject_IsTrue(x);
}

static CYTHON_INLINE PyObject* __Pyx_PyNumber_Int(PyObject* x) {
  PyNumberMethods *m;
  const char *name = NULL;
  PyObject *res = NULL;
#if PY_VERSION_HEX < 0x03000000
  if (PyInt_Check(x) || PyLong_Check(x))
#else
  if (PyLong_Check(x))
#endif
    return Py_INCREF(x), x;
  m = Py_TYPE(x)->tp_as_number;
#if PY_VERSION_HEX < 0x03000000
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Int(x);
  }
  else if (m && m->nb_long) {
    name = "long";
    res = PyNumber_Long(x);
  }
#else
  if (m && m->nb_int) {
    name = "int";
    res = PyNumber_Long(x);
  }
#endif
  if (res) {
#if PY_VERSION_HEX < 0x03000000
    if (!PyInt_Check(res) && !PyLong_Check(res)) {
#else
    if (!PyLong_Check(res)) {
#endif
      PyErr_Format(PyExc_TypeError,
                   "__%s__ returned non-%s (type %.200s)",
                   name, name, Py_TYPE(res)->tp_name);
      Py_DECREF(res);
      return NULL;
    }
  }
  else if (!PyErr_Occurred()) {
    PyErr_SetString(PyExc_TypeError,
                    "an integer is required");
  }
  return res;
}

static CYTHON_INLINE Py_ssize_t __Pyx_PyIndex_AsSsize_t(PyObject* b) {
  Py_ssize_t ival;
  PyObject* x = PyNumber_Index(b);
  if (!x) return -1;
  ival = PyInt_AsSsize_t(x);
  Py_DECREF(x);
  return ival;
}

static CYTHON_INLINE PyObject * __Pyx_PyInt_FromSize_t(size_t ival) {
#if PY_VERSION_HEX < 0x02050000
   if (ival <= LONG_MAX)
       return PyInt_FromLong((long)ival);
   else {
       unsigned char *bytes = (unsigned char *) &ival;
       int one = 1; int little = (int)*(unsigned char*)&one;
       return _PyLong_FromByteArray(bytes, sizeof(size_t), little, 0);
   }
#else
   return PyInt_FromSize_t(ival);
#endif
}

static CYTHON_INLINE size_t __Pyx_PyInt_AsSize_t(PyObject* x) {
   unsigned PY_LONG_LONG val = __Pyx_PyInt_AsUnsignedLongLong(x);
   if (unlikely(val == (unsigned PY_LONG_LONG)-1 && PyErr_Occurred())) {
       return (size_t)-1;
   } else if (unlikely(val != (unsigned PY_LONG_LONG)(size_t)val)) {
       PyErr_SetString(PyExc_OverflowError,
                       "value too large to convert to size_t");
       return (size_t)-1;
   }
   return (size_t)val;
}


#endif 