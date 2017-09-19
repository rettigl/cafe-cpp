#ifndef __PYX_HAVE__PyCafe
#define __PYX_HAVE__PyCafe


#ifndef __PYX_HAVE_API__PyCafe

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(_T) _T
#endif

__PYX_EXTERN_C DL_IMPORT(void) py_cb_wrapper(PVDataHolder, unsigned int, std::string);
__PYX_EXTERN_C DL_IMPORT(void) py_cb_ctrl_wrapper(PVCtrlHolder, unsigned int, std::string);
__PYX_EXTERN_C DL_IMPORT(void) py_cb_handle_wrapper(unsigned int);
__PYX_EXTERN_C DL_IMPORT(void) py_cb_handle_monid_wrapper(unsigned int, unsigned long);

#endif /* !__PYX_HAVE_API__PyCafe */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initPyCafe(void);
#else
PyMODINIT_FUNC PyInit_PyCafe(void);
#endif

#endif /* !__PYX_HAVE__PyCafe */
