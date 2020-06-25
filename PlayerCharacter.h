#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

class UCharacterMovementComponent;
class UStaticMeshComponent;
class UCurveFloat;
class AC_GS_CBTest;


UCLASS()
class CRASHBANDICOOTTEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveY(float fValue);
	void MoveX(float fValue);
	void SetYawValue();
	void Rotate(float Yaw);
	void Attack();
	virtual void Jump() override;
	void DoubleJump();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

private:
	virtual void BeginPlay() override;

	void CheckGrounded();

	void CheckTimelineAnimations(float DeltaTime);
	void PlayTimelineOnce(FTimeline* Timeline, FName Function, UCurveFloat* Curve);

	//Attack + Double Jump Animation
	UFUNCTION()
		void CheckAttackTimeline(float DeltaTime);
	
	UFUNCTION()
		void CheckStretchTimeline(float DeltaTime);

	UFUNCTION()
		void CheckDoubleJumpTimeline(float DeltaTime);

	UFUNCTION()
		void CheckSquashTimeline(float DeltaTime);

private:
	UCharacterMovementComponent* m_MoveC = nullptr;
	UStaticMeshComponent* m_MeshC = nullptr;
	AC_GS_CBTest* m_GameState = nullptr;
	TArray<float> RotationDirections;
	FRotator MeshRotation;
	FVector MeshScale;

public:
	bool bIsAttacking;
	bool bIsJumping;
	bool bIsDoubleJumping;
	TArray<int32> XYMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fJumpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fSpeedValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fAirGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fCoyoteTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fRememberedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		FVector StretchScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		FVector SquashScale;


	FTimeline AttackTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* AttackCurve;

	FTimeline StretchTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* StretchCurve;

	FTimeline DoubleJumpTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* DoubleJumpCurve;

	FTimeline SquashTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* SquashCurve;
};
