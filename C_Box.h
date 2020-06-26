#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "C_Box.generated.h"

class UCurveFloat;

UCLASS()
class CRASHBANDICOOTTEST_API AC_Box : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_Box();

	virtual void Tick(float DeltaTime) override;
	void OnDestroy();

	UFUNCTION()
		void CheckDestructionTimeline(float DeltaTime);

public:
	bool bDestroyTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
		bool bReward;

	FTimeline DestructionTimeline;
	UPROPERTY(EditAnywhere, Category = "Destruction")
		UCurveFloat* DestructionCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destruction")
		FVector OriginalScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
		FVector DestroyScale;
};
