#ifndef GOODBREWINTERNALSTUFF
#define GOODBREWINTERNALSTUFF
#define SETTERFUNC(functionName,varType,varName) \
	void functionName (varType _passed){		 \
	varName=_passed; \
	}
#define GETTERFUNC(functionName,varType,varName) \
	varType functionName (){		 \
		return varName;				 \
	}
#endif
