#define PY_CPP
#include "py.h"

#include <Windows.h>

#include "map.h"

namespace {

HMODULE hModule;

void GetErrorMessage(std::string* pError)
{
	if (!pError)
		return;
	DWORD err = ::GetLastError();
	LPTSTR errorText = NULL;
	// https://stackoverflow.com/a/455533
	DWORD nChars = ::FormatMessageA(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 
	if (nChars && errorText) {
		*pError = errorText;
		::LocalFree(errorText);
	}else {
		*pError = "(unknown)";
	}
}

} // namespace {

#define LOAD_SYMBOL(name) name = reinterpret_cast<decltype(name)>(::GetProcAddress(hModule, #name));

bool pyLoad(const char* libPath, std::string* pError)
{
	hModule = ::LoadLibraryA(libPath);
	if (hModule == NULL) {
		GetErrorMessage(pError);
		return false;
	}
	MAP(LOAD_SYMBOL,
		Py_Initialize,
		Py_InitializeEx,
		Py_Finalize,
		Py_FinalizeEx,
		Py_IsInitialized,

		Py_GetVersion,
		Py_GetCompiler,
		Py_GetProgramFullPath,

		Py_IncRef,
		Py_DecRef,

		PyCapsule_GetPointer,
		PyCapsule_New,

		PyTuple_New,
		PyTuple_Pack,
		PyTuple_Size,
		PyTuple_GetItem,
		PyTuple_GetSlice,
		PyTuple_SetItem,

		PyStructSequence_NewType,
		PyStructSequence_New,
		PyStructSequence_GetItem,
		PyStructSequence_SetItem,

		PyList_New,
		PyList_Size,
		PyList_GetItem,
		PyList_SetItem,
		PyList_Insert,
		PyList_Append,
		PyList_GetSlice,
		PyList_SetSlice,
		PyList_Sort,
		PyList_Reverse,
		PyList_AsTuple,

		PyDict_New,
		PyDict_Clear,
		PyDict_Contains,
		PyDict_Copy,
		PyDict_SetItem,
		PyDict_SetItemString,
		PyDict_DelItem,
		PyDict_DelItemString,
		PyDict_GetItem,
		PyDict_GetItemWithError,
		PyDict_GetItemString,
		PyDict_Items,
		PyDict_Keys,
		PyDict_Values,
		PyDict_Size,
		PyDict_Next,
		PyDict_Merge,
		PyDict_Update,
		PyDict_MergeFromSeq2,

		PySet_New,
		PyFrozenSet_New,
		PySet_Size,
		PySet_Contains,
		PySet_Add,
		PySet_Discard,
		PySet_Pop,
		PySet_Clear,

		PyFile_FromFd,
		PyObject_AsFileDescriptor,
		PyFile_GetLine,
		PyFile_WriteObject,
		PyFile_WriteString,

		PyModule_NewObject,
		PyModule_New,
		PyModule_GetDict,
		PyModule_GetNameObject,
		PyModule_GetName,
		PyModule_GetState,
		PyModule_GetDef,
		PyModule_GetFilenameObject,
		PyModule_GetFilename,
		PyModule_Create2,
		PyModuleDef_Init,
		PyModule_FromDefAndSpec2,
		PyModule_ExecDef,
		PyModule_SetDocString,
		PyModule_AddFunctions,
		PyModule_AddObjectRef,
		PyModule_AddObject,
		PyModule_AddIntConstant,
		PyModule_AddStringConstant,
		PyModule_AddType,

		PyState_FindModule,
		PyState_AddModule,
		PyState_RemoveModule,

		PyErr_BadArgument,
		PyErr_Print,

		PyLong_AsLong,
		PyLong_FromLong,

		PyUnicode_AsUTF8String,
		PyUnicode_AsUTF8AndSize,
		PyUnicode_AsUTF8,
		PyUnicode_FromWideChar,

		PyOS_FSPath,
		PyOS_BeforeFork,
		PyOS_AfterFork_Parent,
		PyOS_AfterFork_Child,
		PyOS_AfterFork,
		PyOS_CheckStack,

		Py_DecodeLocale,
		Py_EncodeLocale,

		PySys_GetObject,
		PySys_SetObject,
		PySys_ResetWarnOptions,
		PySys_AddWarnOption,
		PySys_AddWarnOptionUnicode,
		PySys_SetPath,
		PySys_WriteStdout,
		PySys_WriteStderr,
		PySys_FormatStdout,
		PySys_FormatStderr,
		PySys_AddXOption,
		PySys_GetXOptions,

		Py_FatalError,
		Py_Exit,
		Py_AtExit,

		PyImport_ImportModule,
		PyImport_ImportModuleNoBlock,
		PyImport_ImportModuleLevelObject,
		PyImport_ImportModuleLevel,
		PyImport_Import,
		PyImport_ReloadModule,
		PyImport_AddModuleObject,
		PyImport_AddModule,
		PyImport_ExecCodeModule,
		PyImport_ExecCodeModuleEx,
		PyImport_ExecCodeModuleObject,
		PyImport_ExecCodeModuleWithPathnames,
		PyImport_GetMagicNumber,
		PyImport_GetMagicTag,
		PyImport_GetModuleDict,
		PyImport_GetModule,
		PyImport_GetImporter,
		PyImport_ImportFrozenModuleObject,
		PyImport_ImportFrozenModule,
		PyImport_AppendInittab,

		PyArg_ParseTuple,
		PyArg_VaParse,
		PyArg_ParseTupleAndKeywords,
		PyArg_VaParseTupleAndKeywords,
		PyArg_ValidateKeywordArguments,
		PyArg_Parse,
		PyArg_UnpackTuple,

		Py_BuildValue,
		Py_VaBuildValue,

		PyOS_snprintf,
		PyOS_vsnprintf,
		PyOS_string_to_double,
		PyOS_double_to_string,

		PyEval_GetBuiltins,
		PyEval_GetLocals,
		PyEval_GetGlobals,
		PyEval_GetFrame,
		PyFrame_GetBack,
		PyFrame_GetCode,
		PyFrame_GetLineNumber,
		PyEval_GetFuncName,
		PyEval_GetFuncDesc,

		PyCodec_Register,
		PyCodec_Unregister,
		PyCodec_KnownEncoding,
		PyCodec_Encode,
		PyCodec_Decode,

		PyCodec_Encoder,
		PyCodec_Decoder,
		PyCodec_IncrementalEncoder,
		PyCodec_IncrementalDecoder,
		PyCodec_StreamReader,
		PyCodec_StreamWriter,

		PyCodec_RegisterError,
		PyCodec_LookupError,
		PyCodec_StrictErrors,
		PyCodec_IgnoreErrors,
		PyCodec_ReplaceErrors,
		PyCodec_XMLCharRefReplaceErrors,
		PyCodec_BackslashReplaceErrors,
		PyCodec_NameReplaceErrors,

		PyObject_HasAttr,
		PyObject_HasAttrString,
		PyObject_GetAttr,
		PyObject_GetAttrString,
		PyObject_GenericGetAttr,
		PyObject_SetAttr,
		PyObject_SetAttrString,
		PyObject_GenericSetAttr,
		PyObject_GenericGetDict,
		PyObject_GenericSetDict,
		PyObject_RichCompare,
		PyObject_RichCompareBool,
		PyObject_Repr,
		PyObject_ASCII,
		PyObject_Str,
		PyObject_Bytes,
		PyObject_IsSubclass,
		PyObject_IsInstance,
		PyObject_Hash,
		PyObject_HashNotImplemented,
		PyObject_IsTrue,
		PyObject_Not,
		PyObject_Type,
		PyObject_Size,
		PyObject_Length,
		PyObject_GetItem,
		PyObject_SetItem,
		PyObject_DelItem,
		PyObject_Dir,
		PyObject_GetIter,
		PyObject_GetAIter,

		PyObject_Call,
		PyObject_CallNoArgs,
		PyObject_CallObject,
		PyObject_CallFunction,
		PyObject_CallMethod,
		PyObject_CallFunctionObjArgs,
		PyObject_CallMethodObjArgs,

		PyCallable_Check,
		
		PyCFunction_NewEx,

		Py_Main,
		Py_BytesMain,
		PyOS_InputHook,
		Py_CompileString,
		PyRun_SimpleStringFlags,
		PyEval_EvalCode,
		PyEval_EvalCodeEx,
		PyEval_EvalFrame,
		PyEval_EvalFrameEx,

		Py_Initialize
	);


	return true;
}

#undef LOAD_SYMBOL

bool pyUnload(std::string* pError)
{
	if (!hModule) {
		return false;
	}
	if (!::FreeLibrary(hModule)) {
		GetErrorMessage(pError);
		return false;
	}
	hModule = NULL;
	return true;
}
