#pragma once

#define ensureSoftPtr(SoftPtrVar) \
ensureMsgf(!(SoftPtrVar).IsNull(), TEXT("Missing [%s] in %s !"), \
TEXT(#SoftPtrVar), *GetName())

#define ensureSoftPtrOrRet(SoftPtrVar, Ret) \
if (!ensureSoftPtr(SoftPtrVar)) return Ret;	