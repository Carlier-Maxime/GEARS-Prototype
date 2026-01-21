#pragma once

#define ensureSoftPtr(SoftPtrVar) \
ensureMsgf(!(SoftPtrVar).IsNull(), TEXT("Missing [%s] in %s !"), \
*GET_MEMBER_NAME_CHECKED(ThisClass, SoftPtrVar).ToString(), *GetName())

#define ensureSoftPtrOrRet(SoftPtrVar, Ret) \
if (!ensureSoftPtr(SoftPtrVar)) return Ret;	