#include "RigUnit_GetIKSlot.h"

#include "Animations/Interfaces/AnimIKInterface.h"

FRigUnit_GetIKSlot_Execute()
{
	const FRigVMExecuteContext& Context = ExecuteContext;
	auto Component = ExecuteContext.GetMutableOwningComponent();
	if (!Component) return;
	USkeletalMeshComponent* SkelMesh = Cast<USkeletalMeshComponent>(Component);
	if (!SkelMesh) return;
	UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance();
	if (!AnimInstance) return;
	IAnimIKInterface* IKInterface = Cast<IAnimIKInterface>(AnimInstance);
	if (!IKInterface) return;
	Slot = IKInterface->GetIK(Tag);
}