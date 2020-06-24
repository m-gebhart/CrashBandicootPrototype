// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "C_GS_CBTest.generated.h"

UCLASS()
class CRASHBANDICOOTTEST_API AC_GS_CBTest : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	AC_GS_CBTest();

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		int32 intPlayerScore = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
		bool bGameOver;
};
