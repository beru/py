
#include <cstdio>
#include <cassert>
#include <vector>

#include "py.h"

namespace {

PyMethodDef moduleMethods[] = {
	{NULL, NULL, 0, NULL}
};

PyModuleDef moduleDef = {
	PyModuleDef_HEAD_INIT,
	"test",		// name of module
	"",			// module documentation, may be NULL
	-1,			// size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
	moduleMethods,
};

PyObject* PyInit_test(void)
{
	auto module = PyModule_Create(&moduleDef);
	return module;
}

PyObject* handleFunction(PyObject* self, PyObject* args);

std::vector<PyMethodDef> functionDescs = {
	{"test1", (PyCFunction)handleFunction, METH_VARARGS, NULL},
	{"test2", (PyCFunction)handleFunction, METH_VARARGS, NULL},
};

PyObject* handleFunction(PyObject* self, PyObject* args)
{
	const PyMethodDef* pDesc = (const PyMethodDef*)PyCapsule_GetPointer(self, nullptr);
	ptrdiff_t idx = pDesc - &functionDescs[0];
	assert(idx >= 0);
	assert((size_t)idx < functionDescs.size());

	//VARIANT vtArgs[8];
	size_t nArgs = PyTuple_GET_SIZE(args);
	size_t i;
	for (i = 0; i < nArgs; ++i) {
		PyObject* arg = PyTuple_GetItem(args, i);
		if (!arg)
			break;
		//VARTYPE varType;
		if (i < 4) {
			//varType = info->m_varArguments[i];
		}
		else {
			//if (!info->m_pData || info->m_pData->m_nArgMaxSize >= i) {
			//	varType = VT_EMPTY;
			//}
			//else {
			//	varType = info->m_pData->m_pVarArgEx[i - 4];
			//}
		}
		//if (varType == VT_EMPTY) {
		//	PyErr_BadArgument();
		//	break;
		//}

		//::VariantInit(&vtArgs[i]);
		//if (varType == VT_BSTR) {
		//	const char* str = PyUnicode_AsUTF8(arg);
		//	SysString S(str, strlen(str));
		//	Wrap(&vtArgs[i])->Receive(S);
		//}
		//else if (varType == VT_I4) {
		//	vtArgs[i].vt = VT_I4;
		//	vtArgs[i].lVal = PyLong_AsLong(arg);
		//}
		//else {
		//	assert(false);
		//}
	}

	PyObject* retObj = NULL;
	if (i == nArgs) {
		//VARIANT vtResult;
		//::VariantInit(&vtResult);
		//bool ret = false;// CMacro::HandleFunction(g_pEditView, (EFunctionCode)info->m_nFuncID, vtArgs, nArgs, vtResult);
		//std::wstring str;
		//switch (vtResult.vt) {
		//case VT_I4:
		//	retObj = PyLong_FromLong(vtResult.lVal);
		//	break;
		//case VT_BSTR:
		//	Wrap(&vtResult.bstrVal)->GetW(&str);
		//	retObj = PyUnicode_FromWideChar(str.c_str(), str.size());
		//	break;
		//default:
		//	assert(false);
		//	break;
		//}
		//::VariantClear(&vtResult);
	}
	for (size_t j = 0; j < i; ++j) {
		//::VariantClear(&vtArgs[j]);
	}

	return retObj;
}



} // namespace {

int main(int argc, char* argv[])
{
	std::string err;
	if (!pyLoadLib("python3.dll", &err)) {
		printf("%s\n", err.c_str());
		return 0;
	}

	if (PyImport_AppendInittab("test", &PyInit_test) == -1) {
		fprintf(stderr, "Error: could not extend in-built modules test\n");
		exit(1);
	}

	Py_Initialize();

	assert(Py_IsInitialized());

	const char* version = Py_GetVersion();
	const char* compiler = Py_GetCompiler();
	PyObject* module = PyImport_ImportModule("test");
	if (!module) {
		PyErr_Print();
		fprintf(stderr, "Error: could not import module 'test'\n");
		assert(false);
	}

	for (auto& desc : functionDescs) {
		int ret = PyModule_AddObject(module, desc.ml_name, PyCFunction_New(&desc, PyCapsule_New(&desc, nullptr, nullptr)));
	}

	PyObject* pCode = Py_CompileString("from test import *\ntest1(1, 2, 3)", "", Py_file_input);
	PyObject* pMain = PyImport_AddModule("__main__");
	PyObject* pGlobals = PyModule_GetDict(pMain);
	PyObject* pLocals = PyDict_New();

	PyObject* pObj = PyEval_EvalCode(pCode, pGlobals, pLocals);

	Py_DecRef(pMain);
	Py_DecRef(pCode);
	Py_Finalize();

	pyUnloadLib();
	return 0;
}

