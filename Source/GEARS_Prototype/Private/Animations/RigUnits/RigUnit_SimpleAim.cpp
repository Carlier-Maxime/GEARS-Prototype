#include "RigUnit_SimpleAim.h"
#include "RigVMCore/RigVMExecuteContext.h"

FRigUnit_SimpleAim_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (!Hierarchy || !Hierarchy->Contains(Base) || !Hierarchy->Contains(Target)) return;
	auto BaseTransform = Hierarchy->GetGlobalTransform(Base);
	auto TargetTransform = Hierarchy->GetGlobalTransform(Target);
	
	Aim(*Hierarchy, Base, BaseTransform, TargetTransform);
	if (!bMutualAim) return;
	Aim(*Hierarchy, Target, TargetTransform, BaseTransform);
}

FVector FRigUnit_SimpleAim::CalcBoneAxis(const URigHierarchy& Hierarchy, const FRigElementKey& Bone)
{
	TArray<int32> Children = Hierarchy.GetChildren(Hierarchy.GetIndex(Bone));
	if (Children.Num() == 0) return FVector::ZeroVector;
	const auto ChildLoc = Hierarchy.GetGlobalTransform(Children[0]).GetLocation();
	return (ChildLoc - Hierarchy.GetGlobalTransform(Bone).GetLocation()).GetSafeNormal();
}

void FRigUnit_SimpleAim::Aim(URigHierarchy& Hierarchy, const FRigElementKey& Base, const FTransform& BaseT, const FTransform& TargetT)
{
	auto BoneAxis = CalcBoneAxis(Hierarchy, Base);
	if (!BoneAxis.IsNormalized()) return;
	auto TargetDir = (TargetT.GetLocation() - BaseT.GetLocation()).GetSafeNormal();
	FQuat Delta = FQuat::FindBetweenNormals(BoneAxis, TargetDir);
	FTransform NewBase = BaseT;
	NewBase.SetRotation(Delta * BaseT.GetRotation());
	Hierarchy.SetGlobalTransform(Base, NewBase);
}
