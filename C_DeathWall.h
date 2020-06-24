// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_DeathWall.generated.h"

UCLASS()
class CRASHBANDICOOTTEST_API AC_DeathWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_DeathWall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void InitializeStop();

private:
	bool bIsMoving;
	bool bReachedStop;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fWallSpeed;
	UPROPERTY()
		FVector currentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fXStopLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fStopTime;
};
