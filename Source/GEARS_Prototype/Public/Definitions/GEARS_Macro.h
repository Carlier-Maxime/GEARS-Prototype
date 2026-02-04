#pragma once

#define ensureSoftPtr(SoftPtrVar) \
([&]() mutable -> bool { \
	const bool bIsValid = !(SoftPtrVar).IsNull(); \
	if (!bIsValid) { \
		FString Context = FString::Printf(TEXT("%S"), __FUNCTION__); \
		ensureMsgf(false, TEXT("Missing [%s] in %s!"), TEXT(#SoftPtrVar), *Context); \
	} \
	return bIsValid; \
}())

#define ensureSoftPtrOrRet(SoftPtrVar, Ret) \
if (!ensureSoftPtr(SoftPtrVar)) return Ret;