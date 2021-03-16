/* Generated by Cython 0.23.4 */

#ifndef __PYX_HAVE_API__PyCafe
#define __PYX_HAVE_API__PyCafe
#include "Python.h"

static void (*__pyx_api_f_6PyCafe_py_cb_wrapper)(PVDataHolder, unsigned int, std::string) = 0;
#define py_cb_wrapper __pyx_api_f_6PyCafe_py_cb_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_ctrl_wrapper)(PVCtrlHolder, unsigned int, std::string) = 0;
#define py_cb_ctrl_wrapper __pyx_api_f_6PyCafe_py_cb_ctrl_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_wrapper)(unsigned int) = 0;
#define py_cb_handle_wrapper __pyx_api_f_6PyCafe_py_cb_handle_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_monid_wrapper)(unsigned int, unsigned long) = 0;
#define py_cb_handle_monid_wrapper __pyx_api_f_6PyCafe_py_cb_handle_monid_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_get_wrapper)(unsigned int) = 0;
#define py_cb_handle_get_wrapper __pyx_api_f_6PyCafe_py_cb_handle_get_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_put_wrapper)(unsigned int) = 0;
#define py_cb_handle_put_wrapper __pyx_api_f_6PyCafe_py_cb_handle_put_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_open_wrapper)(unsigned int, int) = 0;
#define py_cb_handle_open_wrapper __pyx_api_f_6PyCafe_py_cb_handle_open_wrapper
static void (*__pyx_api_f_6PyCafe_py_cb_handle_connect_wrapper)(unsigned int, std::string, int) = 0;
#define py_cb_handle_connect_wrapper __pyx_api_f_6PyCafe_py_cb_handle_connect_wrapper
#if !defined(__Pyx_PyIdentifier_FromString)
#if PY_MAJOR_VERSION < 3
#define __Pyx_PyIdentifier_FromString(s) PyString_FromString(s)
#else
#define __Pyx_PyIdentifier_FromString(s) PyUnicode_FromString(s)
#endif
#endif

#ifndef __PYX_HAVE_RT_ImportModule
#define __PYX_HAVE_RT_ImportModule
static PyObject *__Pyx_ImportModule(const char *name)
{
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

#ifndef __PYX_HAVE_RT_ImportFunction
#define __PYX_HAVE_RT_ImportFunction
static int __Pyx_ImportFunction(PyObject *module, const char *funcname, void (**f)(void), const char *sig)
{
    PyObject *d = 0;
    PyObject *cobj = 0;
    union
    {
        void (*fp)(void);
        void *p;
    } tmp;
    d = PyObject_GetAttrString(module, (char *)"__pyx_capi__");
    if (!d)
        goto bad;
    cobj = PyDict_GetItemString(d, funcname);
    if (!cobj)
    {
        PyErr_Format(PyExc_ImportError,
                     "%.200s does not export expected C function %.200s",
                     PyModule_GetName(module), funcname);
        goto bad;
    }
#if PY_VERSION_HEX >= 0x02070000
    if (!PyCapsule_IsValid(cobj, sig))
    {
        PyErr_Format(PyExc_TypeError,
                     "C function %.200s.%.200s has wrong signature (expected %.500s, got %.500s)",
                     PyModule_GetName(module), funcname, sig, PyCapsule_GetName(cobj));
        goto bad;
    }
    tmp.p = PyCapsule_GetPointer(cobj, sig);
#else
    {
        const char *desc, *s1, *s2;
        desc = (const char *)PyCObject_GetDesc(cobj);
        if (!desc)
            goto bad;
        s1 = desc;
        s2 = sig;
        while (*s1 != '\0' && *s1 == *s2)
        {
            s1++;
            s2++;
        }
        if (*s1 != *s2)
        {
            PyErr_Format(PyExc_TypeError,
                         "C function %.200s.%.200s has wrong signature (expected %.500s, got %.500s)",
                         PyModule_GetName(module), funcname, sig, desc);
            goto bad;
        }
        tmp.p = PyCObject_AsVoidPtr(cobj);
    }
#endif
    *f = tmp.fp;
    if (!(*f))
        goto bad;
    Py_DECREF(d);
    return 0;
bad:
    Py_XDECREF(d);
    return -1;
}
#endif


static int import_PyCafe(void)
{
    PyObject *module = 0;
    module = __Pyx_ImportModule("PyCafe");
    if (!module) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_wrapper, "void (PVDataHolder, unsigned int, std::string)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_ctrl_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_ctrl_wrapper, "void (PVCtrlHolder, unsigned int, std::string)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_wrapper, "void (unsigned int)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_monid_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_monid_wrapper, "void (unsigned int, unsigned long)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_get_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_get_wrapper, "void (unsigned int)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_put_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_put_wrapper, "void (unsigned int)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_open_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_open_wrapper, "void (unsigned int, int)") < 0) goto bad;
    if (__Pyx_ImportFunction(module, "py_cb_handle_connect_wrapper", (void (**)(void))&__pyx_api_f_6PyCafe_py_cb_handle_connect_wrapper, "void (unsigned int, std::string, int)") < 0) goto bad;
    Py_DECREF(module);
    module = 0;
    return 0;
bad:
    Py_XDECREF(module);
    return -1;
}

#endif /* !__PYX_HAVE_API__PyCafe */
