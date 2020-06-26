#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

class UCharacterMovementComponent;
class UStaticMeshComponent;
class UCurveFloat;
class UCameraShake;
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
	void PlayTimeline(FTimeline* Timeline, FName Function, UCurveFloat* Curve, bool bPlayOnce);

	//Attack + Double Jump Animation
	UFUNCTION()
		void CheckWalkTimeline(float DeltaTime);	
	
	UFUNCTION()
		void CheckAttackTimeline(float DeltaTime);

	UFUNCTION()
		void CheckStretchTimeline(float DeltaTime);

	UFUNCTION()
		void CheckDoubleJumpTimeline(float DeltaTime);

	UFUNCTION()
		void CheckSquashTimeline(float DeltaTime);

public:
	bool bIsAttacking;
	bool bIsJumping;
	bool bIsDoubleJumping;
	FVector2D XYMovement;

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

	UPROPERTY(BlueprintReadOnly)
		FVector CameraFocusPos;


	FTimeline WalkTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* WalkCurve;	

	UPROPERTY(EditAnywhere, Category = "Timeline")
		FVector WalkScale;
	
	FTimeline AttackTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* AttackCurve;

	FTimeline StretchTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* StretchCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		FVector StretchScale;

	FTimeline DoubleJumpTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* DoubleJumpCurve;

	FTimeline SquashTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* SquashCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		FVector SquashScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Shake")
		float fIntensity;
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
		float fDuration;
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake|Oscillation")
		FVector2D BlendInOut;
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake|Oscillation")
		FVector2D PitchAmplitudeRange;		
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake|Oscillation")
		FVector2D PitchFrequencyRange;	
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake|Oscillation")
		FVector2D YawAmplitudeRange;	
	
	UPROPERTY(EditAnywhere, Category = "Camera Shake|Oscillation")
		FVector2D YawFrequencyRange;

private:
	UCharacterMovementComponent* m_MoveC = nullptr;
	UStaticMeshComponent* m_MeshC = nullptr;
	AC_GS_CBTest* m_GameState = nullptr;
	TArray<float> RotationDirections;
	FRotator MeshRotation;
	FVector MeshScale;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
		TSubclassOf<UCameraShake> CamShake;
};