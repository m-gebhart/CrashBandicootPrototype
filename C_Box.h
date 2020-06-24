#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Box.generated.h"

UCLASS()
class CRASHBANDICOOTTEST_API AC_Box : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_Box();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bReward;
};
