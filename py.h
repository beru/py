#pragma once

#include <string>
#include <string_view>
#include <cstdint>

#ifdef PY_CPP
#define EXTERN
#else
#define EXTERN extern
#endif

using Py_ssize_t = intptr_t;
using Py_hash_t = Py_ssize_t;

struct PyTypeObject;

struct PyObject
{
	Py_ssize_t ob_refcnt;
	PyTypeObject* ob_type;
};

#define PyObject_HEAD                   PyObject ob_base;

using PyCFunction = PyObject* (*)(PyObject*, PyObject*);

struct PyMethodDef
{
	const char* ml_name;
	PyCFunction ml_meth;
	int ml_flags;
	const char* ml_doc;
};

struct PyModuleDef_Base
{
	PyObject_HEAD
	PyObject* (*m_init)(void);
	Py_ssize_t m_index;
	PyObject* m_copy;
};

using visitproc = int (*)(PyObject*, void*);
using traverseproc = int (*)(PyObject*, visitproc, void*);
using freefunc = void (*)(void*);
using inquiry = int (*)(PyObject*);

struct PyModuleDef
{
	PyModuleDef_Base m_base;
	const char* m_name;
	const char* m_doc;
	Py_ssize_t m_size;
	PyMethodDef* m_methods;
	struct PyModuleDef_Slot* m_slots;
	traverseproc m_traverse;
	inquiry m_clear;
	freefunc m_free;
};

struct PyVarObject
{
	PyObject ob_base;
	Py_ssize_t ob_size;
};

#define PyObject_VAR_HEAD      PyVarObject ob_base;

struct PyTupleObject
{
	PyObject_VAR_HEAD
	PyObject* ob_item[1];
};

#if 0
#define _PyObject_EXTRA_INIT 0, 0,
#else
#define _PyObject_EXTRA_INIT
#endif

#define PyObject_HEAD_INIT(type)        \
    { _PyObject_EXTRA_INIT              \
    1, type },

#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }

inline Py_ssize_t Py_REFCNT(const void* ob) {
	return reinterpret_cast<const PyObject*>(ob)->ob_refcnt;
}

inline PyTypeObject* Py_TYPE(const void* ob) {
	return reinterpret_cast<const PyObject*>(ob)->ob_type;
}

inline Py_ssize_t Py_SIZE(const void* ob) {
	return reinterpret_cast<const PyVarObject*>(ob)->ob_size;
}

inline Py_ssize_t PyTuple_GET_SIZE(const void* op) {
	return Py_SIZE(op);
}

EXTERN void (*Py_Initialize)();
EXTERN void (*Py_InitializeEx)(int);
EXTERN void (*Py_Finalize)(void);
EXTERN int (*Py_FinalizeEx)(void); // >= 3.6
EXTERN int (*Py_IsInitialized)(void);

EXTERN const char* (*Py_GetVersion)(void);
EXTERN const char* (*Py_GetCompiler)(void);
EXTERN wchar_t* (*Py_GetProgramFullPath)();

EXTERN void (*Py_IncRef)(PyObject*);
EXTERN void (*Py_DecRef)(PyObject*);

EXTERN void* (*PyCapsule_GetPointer)(PyObject* capsule, const char* name);
using PyCapsule_Destructor = void (*)(PyObject*);
EXTERN PyObject* (*PyCapsule_New)(void* pointer, const char* name, PyCapsule_Destructor destructor);

// Tuple Objects
EXTERN PyObject* (*PyTuple_New)(Py_ssize_t len);
EXTERN PyObject* (*PyTuple_Pack)(Py_ssize_t n, ...);
EXTERN Py_ssize_t (*PyTuple_Size)(PyObject* p);
EXTERN PyObject* (*PyTuple_GetItem)(PyObject*, Py_ssize_t);
EXTERN PyObject* (*PyTuple_GetSlice)(PyObject* p, Py_ssize_t low, Py_ssize_t high);
EXTERN int (*PyTuple_SetItem)(PyObject* p, Py_ssize_t pos, PyObject* o);

// Struct Sequence Objects
struct PyStructSequence_Desc;
EXTERN PyTypeObject* (*PyStructSequence_NewType)(PyStructSequence_Desc* desc);
EXTERN PyObject* (*PyStructSequence_New)(PyTypeObject* type);
EXTERN PyObject* (*PyStructSequence_GetItem)(PyObject* p, Py_ssize_t pos);
EXTERN void (*PyStructSequence_SetItem)(PyObject* p, Py_ssize_t pos, PyObject* o);

// List Objects
EXTERN PyObject* (*PyList_New)(Py_ssize_t len);
EXTERN Py_ssize_t (*PyList_Size)(PyObject* list);
EXTERN PyObject* (*PyList_GetItem)(PyObject* list, Py_ssize_t index);
EXTERN int (*PyList_SetItem)(PyObject* list, Py_ssize_t index, PyObject* item);
EXTERN int (*PyList_Insert)(PyObject* list, Py_ssize_t index, PyObject* item);
EXTERN int (*PyList_Append)(PyObject* list, PyObject* item);
EXTERN PyObject* (*PyList_GetSlice)(PyObject* list, Py_ssize_t low, Py_ssize_t high);
EXTERN int (*PyList_SetSlice)(PyObject* list, Py_ssize_t low, Py_ssize_t high, PyObject* itemlist);
EXTERN int (*PyList_Sort)(PyObject* list);
EXTERN int (*PyList_Reverse)(PyObject* list);
EXTERN PyObject* (*PyList_AsTuple)(PyObject* list);

// Dictionary Objects
EXTERN PyObject* (*PyDict_New)();
EXTERN void (*PyDict_Clear)(PyObject* p);
EXTERN int (*PyDict_Contains)(PyObject* p, PyObject* key);
EXTERN PyObject* (*PyDict_Copy)(PyObject* p);
EXTERN int (*PyDict_SetItem)(PyObject* p, PyObject* key, PyObject* val);
EXTERN int (*PyDict_SetItemString)(PyObject* p, const char* key, PyObject* val);
EXTERN int (*PyDict_DelItem)(PyObject* p, PyObject* key);
EXTERN int (*PyDict_DelItemString)(PyObject* p, const char* key);
EXTERN PyObject* (*PyDict_GetItem)(PyObject* p, PyObject* key);
EXTERN PyObject* (*PyDict_GetItemWithError)(PyObject* p, PyObject* key);
EXTERN PyObject* (*PyDict_GetItemString)(PyObject* p, const char* key);
EXTERN PyObject* (*PyDict_Items)(PyObject* p);
EXTERN PyObject* (*PyDict_Keys)(PyObject* p);
EXTERN PyObject* (*PyDict_Values)(PyObject* p);
EXTERN Py_ssize_t(*PyDict_Size)(PyObject* p);
EXTERN int (*PyDict_Next)(PyObject* p, Py_ssize_t* ppos, PyObject** pkey, PyObject** pvalue);
EXTERN int (*PyDict_Merge)(PyObject* a, PyObject* b, int override);
EXTERN int (*PyDict_Update)(PyObject* a, PyObject* b);
EXTERN int (*PyDict_MergeFromSeq2)(PyObject* a, PyObject* seq2, int override);

// Set Objects
EXTERN PyObject* (*PySet_New)(PyObject* iterable);
EXTERN PyObject* (*PyFrozenSet_New)(PyObject* iterable);
EXTERN Py_ssize_t (*PySet_Size)(PyObject* anyset);
EXTERN int (*PySet_Contains)(PyObject* anyset, PyObject* key);
EXTERN int (*PySet_Add)(PyObject* set, PyObject* key);
EXTERN int (*PySet_Discard)(PyObject* set, PyObject* key);
EXTERN PyObject* (*PySet_Pop)(PyObject* set);
EXTERN int (*PySet_Clear)(PyObject* set);

// File Objects
EXTERN PyObject* (*PyFile_FromFd)(int fd, const char* name, const char* mode, int buffering, const char* encoding, const char* errors, const char* newline, int closefd);
EXTERN int (*PyObject_AsFileDescriptor)(PyObject* p);
EXTERN PyObject* (*PyFile_GetLine)(PyObject* p, int n);
EXTERN int (*PyFile_WriteObject)(PyObject* obj, PyObject* p, int flags);
EXTERN int (*PyFile_WriteString)(const char* s, PyObject* p);

// Module Objects
EXTERN PyObject* (*PyModule_NewObject)(PyObject* name);
EXTERN PyObject* (*PyModule_New)(const char* name);
EXTERN PyObject* (*PyModule_GetDict)(PyObject* module);
EXTERN PyObject* (*PyModule_GetNameObject)(PyObject* module);
EXTERN const char* (*PyModule_GetName)(PyObject* module);
EXTERN void* (*PyModule_GetState)(PyObject* module);
EXTERN PyModuleDef* (*PyModule_GetDef)(PyObject* module);
EXTERN PyObject* (*PyModule_GetFilenameObject)(PyObject* module);
EXTERN const char* (*PyModule_GetFilename)(PyObject* module);
#define PYTHON_ABI_VERSION 3
EXTERN PyObject* (*PyModule_Create2)(struct PyModuleDef*, int apiver);
inline PyObject* PyModule_Create(struct PyModuleDef* def) {
	return PyModule_Create2(def, PYTHON_ABI_VERSION);
}
EXTERN PyObject* (*PyModuleDef_Init)(PyModuleDef* def);
EXTERN PyObject* (*PyModule_FromDefAndSpec2)(PyModuleDef* def, PyObject* spec, int module_api_version);
EXTERN int (*PyModule_ExecDef)(PyObject* module, PyModuleDef* def);
EXTERN int (*PyModule_SetDocString)(PyObject* module, const char* docstring);
EXTERN int (*PyModule_AddFunctions)(PyObject* module, PyMethodDef* functions);
EXTERN int (*PyModule_AddObjectRef)(PyObject* module, const char* name, PyObject* value);
EXTERN int (*PyModule_AddObject)(PyObject* module, const char* name, PyObject* value);
EXTERN int (*PyModule_AddIntConstant)(PyObject* module, const char* name, long value);
EXTERN int (*PyModule_AddStringConstant)(PyObject* module, const char* name, const char* value);
EXTERN int (*PyModule_AddType)(PyObject* module, PyTypeObject* type);

// Module lookup
EXTERN PyObject* (*PyState_FindModule)(PyModuleDef* def);
EXTERN int (*PyState_AddModule)(PyObject* module, PyModuleDef* def);
EXTERN int (*PyState_RemoveModule)(PyModuleDef* def);

EXTERN int (*PyErr_BadArgument)(void);
EXTERN void (*PyErr_Print)(void);

EXTERN long (*PyLong_AsLong)(PyObject*);
EXTERN PyObject* (*PyLong_FromLong)(long);

EXTERN PyObject* (*PyUnicode_AsUTF8String)(PyObject* unicode);
EXTERN const char* (*PyUnicode_AsUTF8AndSize)(PyObject* unicode, Py_ssize_t* size);
EXTERN const char* (*PyUnicode_AsUTF8)(PyObject* unicode);
EXTERN PyObject* (*PyUnicode_FromWideChar)(const wchar_t* w, Py_ssize_t size);

// Operating System Utilities
EXTERN PyObject* (*PyOS_FSPath)(PyObject* path);
EXTERN void (*PyOS_BeforeFork)();
EXTERN void (*PyOS_AfterFork_Parent)();
EXTERN void (*PyOS_AfterFork_Child)();
EXTERN void (*PyOS_AfterFork)();
EXTERN int (*PyOS_CheckStack)();
using PyOS_sighandler_t = void (*)(int);
EXTERN PyOS_sighandler_t (*PyOS_getsig)(int i);
EXTERN PyOS_sighandler_t (*PyOS_setsig)(int i, PyOS_sighandler_t h);
EXTERN wchar_t* (*Py_DecodeLocale)(const char* arg, size_t* size);
EXTERN char* (*Py_EncodeLocale)(const wchar_t* text, size_t* error_pos);

// System Functions
EXTERN PyObject* (*PySys_GetObject)(const char* name);
EXTERN int (*PySys_SetObject)(const char* name, PyObject* v);
EXTERN void (*PySys_ResetWarnOptions)();
EXTERN void (*PySys_AddWarnOption)(const wchar_t* s);
EXTERN void (*PySys_AddWarnOptionUnicode)(PyObject* unicode);
EXTERN void (*PySys_SetPath)(const wchar_t* path);
EXTERN void (*PySys_WriteStdout)(const char* format, ...);
EXTERN void (*PySys_WriteStderr)(const char* format, ...);
EXTERN void (*PySys_FormatStdout)(const char* format, ...);
EXTERN void (*PySys_FormatStderr)(const char* format, ...);
EXTERN void (*PySys_AddXOption)(const wchar_t* s);
EXTERN PyObject* (*PySys_GetXOptions)();

// Process Control
EXTERN void (*Py_FatalError)(const char* message);
EXTERN void (*Py_Exit)(int status);
EXTERN int (*Py_AtExit)(void (*func)());

// Importing Modules
EXTERN PyObject* (*PyImport_ImportModule)(const char* name);
EXTERN PyObject* (*PyImport_ImportModuleNoBlock)(const char* name);
EXTERN PyObject* (*PyImport_ImportModuleLevelObject)(PyObject* name, PyObject* globals, PyObject* locals, PyObject* fromlist, int level);
EXTERN PyObject* (*PyImport_ImportModuleLevel)(const char* name, PyObject* globals, PyObject* locals, PyObject* fromlist, int level);
EXTERN PyObject* (*PyImport_Import)(PyObject* name);
EXTERN PyObject* (*PyImport_ReloadModule)(PyObject* m);
EXTERN PyObject* (*PyImport_AddModuleObject)(PyObject* name);
EXTERN PyObject* (*PyImport_AddModule)(const char* name);
EXTERN PyObject* (*PyImport_ExecCodeModule)(const char* name, PyObject* co);
EXTERN PyObject* (*PyImport_ExecCodeModuleEx)(const char* name, PyObject* co, const char* pathname);
EXTERN PyObject* (*PyImport_ExecCodeModuleObject)(PyObject* name, PyObject* co, PyObject* pathname, PyObject* cpathname);
EXTERN PyObject* (*PyImport_ExecCodeModuleWithPathnames)(const char* name, PyObject* co, const char* pathname, const char* cpathname);
EXTERN long (*PyImport_GetMagicNumber)();
EXTERN const char* (*PyImport_GetMagicTag)();
EXTERN PyObject* (*PyImport_GetModuleDict)();
EXTERN PyObject* (*PyImport_GetModule)(PyObject* name);
EXTERN PyObject* (*PyImport_GetImporter)(PyObject* path);
EXTERN int (*PyImport_ImportFrozenModuleObject)(PyObject* name);
EXTERN int (*PyImport_ImportFrozenModule)(const char* name);
EXTERN int (*PyImport_AppendInittab)(const char* name, PyObject* (*initfunc)(void));

// API Functions
EXTERN int (*PyArg_ParseTuple)(PyObject* args, const char* format, ...);
EXTERN int (*PyArg_VaParse)(PyObject* args, const char* format, va_list vargs);
EXTERN int (*PyArg_ParseTupleAndKeywords)(PyObject* args, PyObject* kw, const char* format, char* keywords[], ...);
EXTERN int (*PyArg_VaParseTupleAndKeywords)(PyObject* args, PyObject* kw, const char* format, char* keywords[], va_list vargs);
EXTERN int (*PyArg_ValidateKeywordArguments)(PyObject*);
EXTERN int (*PyArg_Parse)(PyObject* args, const char* format, ...);
EXTERN int (*PyArg_UnpackTuple)(PyObject* args, const char* name, Py_ssize_t min, Py_ssize_t max, ...);

// Building values
EXTERN PyObject* (*Py_BuildValue)(const char* format, ...);
EXTERN PyObject* (*Py_VaBuildValue)(const char* format, va_list vargs);

// String conversion and formatting
EXTERN int (*PyOS_snprintf)(char* str, size_t size, const char* format, ...);
EXTERN int (*PyOS_vsnprintf)(char* str, size_t size, const char* format, va_list va);
EXTERN double (*PyOS_string_to_double)(const char* s, char** endptr, PyObject* overflow_exception);
EXTERN char* (*PyOS_double_to_string)(double val, char format_code, int precision, int flags, int* ptype);

// Reflection
EXTERN PyObject* (*PyEval_GetBuiltins)(void);
EXTERN PyObject* (*PyEval_GetLocals)(void);
EXTERN PyObject* (*PyEval_GetGlobals)(void);
struct PyFrameObject;
EXTERN PyFrameObject* (*PyEval_GetFrame)(void);
EXTERN PyFrameObject* (*PyFrame_GetBack)(PyFrameObject* frame);
struct PyCodeObject;
EXTERN PyCodeObject* (*PyFrame_GetCode)(PyFrameObject* frame);
EXTERN int (*PyFrame_GetLineNumber)(PyFrameObject* frame);
EXTERN const char* (*PyEval_GetFuncName)(PyObject* func);
EXTERN const char* (*PyEval_GetFuncDesc)(PyObject* func);

// Codec registry and support functions
EXTERN int (*PyCodec_Register)(PyObject* search_function);
EXTERN int (*PyCodec_Unregister)(PyObject* search_function);
EXTERN int (*PyCodec_KnownEncoding)(const char* encoding);
EXTERN PyObject* (*PyCodec_Encode)(PyObject* object, const char* encoding, const char* errors);
EXTERN PyObject* (*PyCodec_Decode)(PyObject* object, const char* encoding, const char* errors);

// Codec lookup API
EXTERN PyObject* (*PyCodec_Encoder)(const char* encoding);
EXTERN PyObject* (*PyCodec_Decoder)(const char* encoding);
EXTERN PyObject* (*PyCodec_IncrementalEncoder)(const char* encoding, const char* errors);
EXTERN PyObject* (*PyCodec_IncrementalDecoder)(const char* encoding, const char* errors);
EXTERN PyObject* (*PyCodec_StreamReader)(const char* encoding, PyObject* stream, const char* errors);
EXTERN PyObject* (*PyCodec_StreamWriter)(const char* encoding, PyObject* stream, const char* errors);

// Registry API for Unicode encoding error handlers
EXTERN int (*PyCodec_RegisterError)(const char* name, PyObject* error);
EXTERN PyObject* (*PyCodec_LookupError)(const char* name);
EXTERN PyObject* (*PyCodec_StrictErrors)(PyObject* exc);
EXTERN PyObject* (*PyCodec_IgnoreErrors)(PyObject* exc);
EXTERN PyObject* (*PyCodec_ReplaceErrors)(PyObject* exc);
EXTERN PyObject* (*PyCodec_XMLCharRefReplaceErrors)(PyObject* exc);
EXTERN PyObject* (*PyCodec_BackslashReplaceErrors)(PyObject* exc);
EXTERN PyObject* (*PyCodec_NameReplaceErrors)(PyObject* exc);

// Object Protocol
EXTERN int (*PyObject_HasAttr)(PyObject* o, PyObject* attr_name);
EXTERN int (*PyObject_HasAttrString)(PyObject* o, const char* attr_name);
EXTERN PyObject* (*PyObject_GetAttr)(PyObject* o, PyObject* attr_name);
EXTERN PyObject* (*PyObject_GetAttrString)(PyObject* o, const char* attr_name);
EXTERN PyObject* (*PyObject_GenericGetAttr)(PyObject* o, PyObject* name);
EXTERN int (*PyObject_SetAttr)(PyObject* o, PyObject* attr_name, PyObject* v);
EXTERN int (*PyObject_SetAttrString)(PyObject* o, const char* attr_name, PyObject* v);
EXTERN int (*PyObject_GenericSetAttr)(PyObject* o, PyObject* name, PyObject* value);
EXTERN PyObject* (*PyObject_GenericGetDict)(PyObject* o, void* context);
EXTERN int (*PyObject_GenericSetDict)(PyObject* o, PyObject* value, void* context);
EXTERN PyObject* (*PyObject_RichCompare)(PyObject* o1, PyObject* o2, int opid);
EXTERN int (*PyObject_RichCompareBool)(PyObject* o1, PyObject* o2, int opid);
EXTERN PyObject* (*PyObject_Repr)(PyObject* o);
EXTERN PyObject* (*PyObject_ASCII)(PyObject* o);
EXTERN PyObject* (*PyObject_Str)(PyObject* o);
EXTERN PyObject* (*PyObject_Bytes)(PyObject* o);
EXTERN int (*PyObject_IsSubclass)(PyObject* derived, PyObject* cls);
EXTERN int (*PyObject_IsInstance)(PyObject* inst, PyObject* cls);
EXTERN Py_hash_t(*PyObject_Hash)(PyObject* o);
EXTERN Py_hash_t(*PyObject_HashNotImplemented)(PyObject* o);
EXTERN int (*PyObject_IsTrue)(PyObject* o);
EXTERN int (*PyObject_Not)(PyObject* o);
EXTERN PyObject* (*PyObject_Type)(PyObject* o);
EXTERN Py_ssize_t(*PyObject_Size)(PyObject* o);
EXTERN Py_ssize_t(*PyObject_Length)(PyObject* o);
EXTERN PyObject* (*PyObject_GetItem)(PyObject* o, PyObject* key);
EXTERN int (*PyObject_SetItem)(PyObject* o, PyObject* key, PyObject* v);
EXTERN int (*PyObject_DelItem)(PyObject* o, PyObject* key);
EXTERN PyObject* (*PyObject_Dir)(PyObject* o);
EXTERN PyObject* (*PyObject_GetIter)(PyObject* o);
EXTERN PyObject* (*PyObject_GetAIter)(PyObject* o);

// Object Calling API
EXTERN PyObject* (*PyObject_Call)(PyObject* callable, PyObject* args, PyObject* kwargs);
EXTERN PyObject* (*PyObject_CallNoArgs)(PyObject* callable);
EXTERN PyObject* (*PyObject_CallObject)(PyObject* callable, PyObject* args);
EXTERN PyObject* (*PyObject_CallFunction)(PyObject* callable, const char* format, ...);
EXTERN PyObject* (*PyObject_CallMethod)(PyObject* obj, const char* name, const char* format, ...);
EXTERN PyObject* (*PyObject_CallFunctionObjArgs)(PyObject* callable, ...);
EXTERN PyObject* (*PyObject_CallMethodObjArgs)(PyObject* obj, PyObject* name, ...);

// Call Support API
EXTERN int (*PyCallable_Check)(PyObject* o);

#define PyCFunction_New(ML, SELF) PyCFunction_NewEx((ML), (SELF), NULL)
EXTERN PyObject* (*PyCFunction_NewEx)(PyMethodDef*, PyObject*, PyObject*);

EXTERN int (*Py_Main)(int argc, wchar_t** argv);
EXTERN int (*Py_BytesMain)(int argc, char** argv);
EXTERN int (**PyOS_InputHook)(void);
EXTERN PyObject* (*Py_CompileString)(const char* str, const char* filename, int start);
struct PyCompilerFlags
{
	int cf_flags;
	int cf_feature_version;
};
EXTERN int (*PyRun_SimpleStringFlags)(const char*, PyCompilerFlags*);
#define PyRun_SimpleString(s) PyRun_SimpleStringFlags(s, NULL)
EXTERN PyObject* (*PyEval_EvalCode)(PyObject* co, PyObject* globals, PyObject* locals);
EXTERN PyObject* (*PyEval_EvalCodeEx)(PyObject* co, PyObject* globals, PyObject* locals, PyObject* const* args, int argcount, PyObject* const* kws, int kwcount, PyObject* const* defs, int defcount, PyObject* kwdefs, PyObject* closure);
EXTERN PyObject* (*PyEval_EvalFrame)(PyFrameObject* f);
EXTERN PyObject* (*PyEval_EvalFrameEx)(PyFrameObject* f, int throwflag);

inline void Py_XINCREF(PyObject* op) {
	if (op != NULL) {
		Py_IncRef(op);
	}
}

inline void Py_XDECREF(PyObject* op) {
	if (op != NULL) {
		Py_DecRef(op);
	}
}

#define METH_VARARGS  0x0001

#define Py_single_input 256
#define Py_file_input 257
#define Py_eval_input 258
#define Py_func_type_input 345

bool pyLoadLib(const char* libPath, std::string* pError = nullptr);
bool pyUnloadLib(std::string* pError = nullptr);

