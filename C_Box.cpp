#include "CrashBandicootTest/C_Box.h"
#include "CrashBandicootTest/PlayerCharacter.h"
#include "GameFramework/Actor.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"


AC_Box::AC_Box() :
	bDestroyTriggered(false)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	OriginalScale = GetActorScale3D();
}

void AC_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AC_Box::CheckDestructionTimeline(DeltaTime);
	if (bDestroyTriggered && !DestructionTimeline.IsPlaying())
	{
		Destroy();
	}
}

void AC_Box::OnDestroy()
{
	if (DestructionCurve) {
		FOnTimelineFloat TimelineBind;
		TimelineBind.BindUFunction(this, FName("CheckDestructionTimeline"));
		DestructionTimeline.AddInterpFloat(DestructionCurve, TimelineBind);
		DestructionTimeline.SetLooping(false);
		DestructionTimeline.PlayFromStart();
		SetActorTickEnabled(true);
	}
	else
	{
		this->Destroy();
	}
	bDestroyTriggered = true;
}

void AC_Box::CheckDestructionTimeline(float DeltaTime)
{
	((UStaticMeshComponent*)GetComponentByClass(UStaticMeshComponent::StaticClass()))->SetWorldScale3D(FMath::Lerp(OriginalScale, DestroyScale, DeltaTime));
}