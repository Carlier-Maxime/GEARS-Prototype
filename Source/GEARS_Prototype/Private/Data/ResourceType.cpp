#include "ResourceType.h"

void UResourceType::PostInitProperties()
{
	Super::PostInitProperties();
	# if WITH_EDITORONLY_DATA
		Sampling.UpdatePreview();
	# endif
}

#if WITH_EDITORONLY_DATA
void UResourceType::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Sampling.UpdatePreview();
}
#endif