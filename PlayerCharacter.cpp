#include "CrashBandicootTest/PlayerCharacter.h"
#include "CrashBandicootTest/C_Box.h"
#include "CrashBandicootTest/C_GS_CBTest.h"
#include "GameFramework/Character.h" 
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/Rotator.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter() :
	bIsAttacking(false),
	bIsJumping(false),
	bIsDoubleJumping(false),
	fJumpValue(420.f)
{
	PrimaryActorTick.bCanEverTick = true;
	XYMovement.Init(0, 2);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.AddUnique("PlayerCharacter");

	m_MoveC = GetCharacterMovement();
	m_MeshC = (UStaticMeshComponent*)GetComponentByClass(UStaticMeshComponent::StaticClass());
	m_MeshC->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
	m_GameState = (AC_GS_CBTest*)UGameplayStatics::GetGameState(GetWorld());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	if (bIsJumping) 
	{
		APlayerCharacter::CheckGrounded();
	}

	if (bIsAttacking && AttackCurve) 
	{
		if (AttackTimeline.IsPlaying())
		{
			AttackTimeline.TickTimeline(DeltaTime);
		}
		else
		{
			bIsAttacking = false;
		}
	}


	if (DoubleJumpCurve && DoubleJumpTimeline.IsPlaying())
	{
		DoubleJumpTimeline.TickTimeline(DeltaTime);
	}
}

void APlayerCharacter::CheckGrounded()
{
	if (GetVelocity().Z == 0.f) 
	{
		bIsJumping = false;
		bIsDoubleJumping = false;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveY);
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveX);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
}

void APlayerCharacter::MoveY(float fValue)
{
	this->AddMovementInput(FVector(0, fValue * fSpeedValue, 0));
	if (fValue == -1.00f)
	{
		XYMovement[1] = -1;
		SetYawValue();
	}
	else if (fValue == 1.00f)
	{
		XYMovement[1] = 1;
		SetYawValue();
	}
	else
	{
		XYMovement[1] = 0;
	}
}

void APlayerCharacter::MoveX(float fValue)
{
	this->AddMovementInput(FVector(fValue*fSpeedValue, 0, 0));
	if (fValue == -1.00f)
	{
		XYMovement[0] = -1;
		SetYawValue();
	}
	else if (fValue == 1.00f)
	{
		XYMovement[0] = 1;
		SetYawValue();
	}
	else
	{
		XYMovement[0] = 0;
	}
}

void APlayerCharacter::SetYawValue()
{
	//Placeholder Solution!
	if (XYMovement[0] == -1 && XYMovement[1] == 0)
		SetRotation(0.f);
	else if (XYMovement[0] == -1 && XYMovement[1] == -1)
		SetRotation(45.f);
	else if (XYMovement[0] == 0 && XYMovement[1] == -1)
		SetRotation(90.f);
	else if (XYMovement[0] == 1 && XYMovement[1] == -1)
		SetRotation(135.f);
	else if (XYMovement[0] == 1 && XYMovement[1] == 0)
		SetRotation(180.f);
	else if (XYMovement[0] == 1 && XYMovement[1] == 1)
		SetRotation(225.f);
	else if (XYMovement[0] == 0 && XYMovement[1] == 1)
		SetRotation(270.f);
	else if (XYMovement[0] == -1 && XYMovement[1] == 1)
		SetRotation(315.f);
}

void APlayerCharacter::SetRotation(float Yaw)
{
	m_MeshC->SetRelativeRotation(FRotator(m_MeshC->GetRelativeRotation().Pitch, Yaw, m_MeshC->GetRelativeRotation().Roll));
}


void APlayerCharacter::Attack()
{
	bIsAttacking = true;

	if (AttackCurve)
	{
		FOnTimelineFloat AttackTimelineBind;
		AttackTimelineBind.BindUFunction(this, FName("CheckAttackTimeline"));
		AttackTimeline.AddInterpFloat(AttackCurve, AttackTimelineBind);
		AttackTimeline.SetLooping(false);

		AttackTimeline.PlayFromStart();
	}
}

void APlayerCharacter::Jump()
{
	if (!bIsJumping) 
	{
		LaunchCharacter(FVector(0, 0, fJumpValue), false, true);
		bIsJumping = true;
	}
	else if (!bIsDoubleJumping)
	{
		APlayerCharacter::DoubleJump();
	}
}

void APlayerCharacter::DoubleJump()
{
	LaunchCharacter(FVector(0, 0, fJumpValue), false, true);
	bIsDoubleJumping = true;

	if (DoubleJumpCurve)
	{
		currentMeshRotation = m_MeshC->GetRelativeRotation();
		FOnTimelineFloat DoubleJumpTimelineBind;
		DoubleJumpTimelineBind.BindUFunction(this, FName("CheckDoubleJumpTimeline"));
		DoubleJumpTimeline.AddInterpFloat(DoubleJumpCurve, DoubleJumpTimelineBind);
		DoubleJumpTimeline.SetLooping(false);

		DoubleJumpTimeline.PlayFromStart();
	}
}

void APlayerCharacter::CheckAttackTimeline(float deltaTime)
{
	m_MeshC->SetRelativeRotation(FRotator(currentMeshRotation.Pitch, deltaTime, currentMeshRotation.Roll));
}

void APlayerCharacter::CheckDoubleJumpTimeline(float deltaTime)
{
	m_MeshC->SetRelativeRotation(FRotator(deltaTime, currentMeshRotation.Yaw, currentMeshRotation.Roll));
}

void APlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	FString strActorName = OtherActor->GetName();
	if (strActorName.StartsWith(TEXT("BP_BOX")) && bIsAttacking)
	{
		if (((AC_Box*)OtherActor)->bReward)
		{
			m_GameState->intPlayerScore++;
		}
		OtherActor->Destroy();
	}

	if (strActorName.StartsWith(TEXT("BP_Death")))
	{
		m_GameState->bGameOver = true;
		Destroy();
	}
}