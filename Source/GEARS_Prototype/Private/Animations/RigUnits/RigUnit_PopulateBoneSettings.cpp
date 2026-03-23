#include "RigUnit_PopulateBoneSettings.h"

FRigUnit_PopulateBoneSettings_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (!Hierarchy) return;
	
	TArray<FRigControlElement*> Controls = Hierarchy->GetControls();
	
	Settings.Reset();
	Settings.Reserve(Controls.Num());
	for (FRigControlElement* Control : Controls)
	{
		TArray<FRigControlLimitEnabled>& Limits = Control->Settings.LimitEnabled;
		if (!(Limits.Num() == 3 || Limits.Num() == 9))
		{
			UE_LOG(LogTemp, Error, TEXT("Mismatch Number of Limits for %s ! (expect: 3 or 9, get: %d)"), *Control->GetName(), Limits.Num());
			continue;
		}
		auto LimitsView = TArrayView<FRigControlLimitEnabled>(Limits).Mid(Limits.Num() == 9 ? 2 : 0, 3);
		for (const FRigElementKey& DrivenKey : Control->Settings.DrivenControls)
		{
			if (DrivenKey.Type != ERigElementType::Bone) continue;
			FPBIKBoneSetting Setting;
			Setting.Bone = DrivenKey.Name;
			Setting.PositionStiffness = 1;
			Setting.RotationStiffness = 0.2;
			Setting.X = GetLimitType(LimitsView[0]);
			Setting.MinX = LimitsView[0].GetForValueType(ERigControlValueType::Minimum);
			Setting.MaxX = LimitsView[0].GetForValueType(ERigControlValueType::Maximum);
			Setting.Y = GetLimitType(LimitsView[1]);
			Setting.MinY = LimitsView[1].GetForValueType(ERigControlValueType::Minimum);
			Setting.MaxY = LimitsView[1].GetForValueType(ERigControlValueType::Maximum);
			Setting.Z = GetLimitType(LimitsView[2]);
			Setting.MinZ = LimitsView[2].GetForValueType(ERigControlValueType::Minimum);
			Setting.MaxZ = LimitsView[2].GetForValueType(ERigControlValueType::Maximum);
			Settings.Add(Setting);
		}
	}
}

EPBIKLimitType FRigUnit_PopulateBoneSettings::GetLimitType(const FRigControlLimitEnabled& Limit)
{
	if (Limit.IsOff()) return EPBIKLimitType::Free;
	if (!Limit.bMaximum || !Limit.bMinimum) return EPBIKLimitType::Limited;
	if (Limit.GetForValueType(ERigControlValueType::Minimum) != Limit.GetForValueType(ERigControlValueType::Maximum)) return EPBIKLimitType::Limited;
	return EPBIKLimitType::Locked;
}
