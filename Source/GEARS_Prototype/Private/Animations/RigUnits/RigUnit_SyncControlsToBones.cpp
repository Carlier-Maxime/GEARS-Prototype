#include "RigUnit_SyncControlsToBones.h"

FRigUnit_SyncControlsToBones_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (!Hierarchy) return;
	
	TArray<FTransform> Transforms;
	for (FRigControlElement* Controller : Hierarchy->GetControls(true))
	{
		Transforms.Reset();
		Transforms.Reserve(Controller->Settings.DrivenControls.Num());
		for (const FRigElementKey& DrivenKey : Controller->Settings.DrivenControls)
		{
			Transforms.Add(Hierarchy->GetGlobalTransform(DrivenKey));
		}
		Hierarchy->SetGlobalTransform(Controller->GetKey(), AverageTransforms(Transforms), false, false);
	}
}

FTransform FRigUnit_SyncControlsToBones::AverageTransforms(const TArray<FTransform>& Transforms)
{
	if (Transforms.Num() == 0) return FTransform::Identity;
	if (Transforms.Num() == 1) return Transforms[0];

	auto AvgLocation = FVector::ZeroVector;
	auto AvgScale = FVector::ZeroVector;
	FQuat AvgRotation(0.f, 0.f, 0.f, 0.f);

	auto Weight = 1.0f / static_cast<float>(Transforms.Num());

	for (const FTransform& T : Transforms)
	{
		AvgLocation += T.GetLocation() * Weight;
		AvgScale += T.GetScale3D() * Weight;
		FQuat TargetQ = T.GetRotation();
		if ((AvgRotation | TargetQ) < 0.f) TargetQ *= -1.f;
		AvgRotation += TargetQ * Weight;
	}

	AvgRotation.Normalize();
	return FTransform(AvgRotation, AvgLocation, AvgScale);
}