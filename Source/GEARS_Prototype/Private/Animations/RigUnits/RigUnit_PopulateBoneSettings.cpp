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
		FRotator MinRota = Control->Settings.MinimumValue.Get<FRigControlValue::FEulerTransform_Float>().GetRotator();
		FRotator MaxRota = Control->Settings.MaximumValue.Get<FRigControlValue::FEulerTransform_Float>().GetRotator();
		auto LimitsView = TArrayView<FRigControlLimitEnabled>(Limits).Mid(Limits.Num() == 9 ? 2 : 0, 3);
		for (const FRigElementKey& DrivenKey : Control->Settings.DrivenControls)
		{
			if (DrivenKey.Type != ERigElementType::Bone) continue;
			FPBIKBoneSetting Setting;
			Setting.Bone = DrivenKey.Name;
			Setting.PositionStiffness = 1;
			Setting.RotationStiffness = 0.2;
			Setting.X = GetLimitType(LimitsView[0], MinRota.Pitch, MaxRota.Pitch);
			Setting.MinX = MinRota.Pitch;
			Setting.MaxX = MaxRota.Pitch;
			Setting.Y = GetLimitType(LimitsView[1], MinRota.Yaw, MaxRota.Yaw);
			Setting.MinY = MinRota.Yaw;
			Setting.MaxY = MaxRota.Yaw;
			Setting.Z = GetLimitType(LimitsView[2], MinRota.Roll, MaxRota.Roll);
			Setting.MinZ = MinRota.Roll;
			Setting.MaxZ = MaxRota.Roll;
			Settings.Add(Setting);
		}
	}
}

EPBIKLimitType FRigUnit_PopulateBoneSettings::GetLimitType(const FRigControlLimitEnabled& Limit, const float Min, const float Max)
{
	if (Limit.IsOff()) return EPBIKLimitType::Free;
	if (!Limit.bMaximum || !Limit.bMinimum) return EPBIKLimitType::Limited;
	if (Min != Max) return EPBIKLimitType::Limited;
	return EPBIKLimitType::Locked;
}
