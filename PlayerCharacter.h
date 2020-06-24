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
	void SetRotation(float Yaw);
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

	//Attack + Double Jump Animation
	UFUNCTION()
		void CheckAttackTimeline(float deltaTime);
	
	UFUNCTION()
		void CheckDoubleJumpTimeline(float deltaTime);

private:
	UCharacterMovementComponent* m_MoveC = nullptr;
	UStaticMeshComponent* m_MeshC = nullptr;
	AC_GS_CBTest* m_GameState = nullptr;

public:
	bool bIsAttacking;
	bool bIsJumping;
	bool bIsDoubleJumping;
	TArray<int32> XYMovement;
	FRotator currentMeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fJumpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Behaviour")
		float fSpeedValue;

	FTimeline AttackTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* AttackCurve;

	FTimeline DoubleJumpTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* DoubleJumpCurve;
};
