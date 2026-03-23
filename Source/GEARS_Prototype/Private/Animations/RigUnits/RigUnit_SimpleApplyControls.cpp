#include "RigUnit_SimpleApplyControls.h"

FRigUnit_SimpleApplyControls_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (!Hierarchy) return;
	
	for (FRigControlElement* Controller : Hierarchy->GetControls())
	{
		auto GlobalTransform = Controller->GetTransform().Current.Global.Get();
		for (const FRigElementKey& DrivenKey : Controller->Settings.DrivenControls)
		{
			Hierarchy->SetGlobalTransform(DrivenKey, GlobalTransform);
		}
	}
}