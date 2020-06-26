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
#include "Editor/EditorEngine.h" 
#include "Math/Rotator.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter() :
	bIsAttacking(false),
	bIsJumping(false),
	bIsDoubleJumping(false)
{
	PrimaryActorTick.bCanEverTick = true;
	RotationDirections.Init(0,0);
	
	for (int direction = 0; direction < 8; direction++)
	{
		RotationDirections.Add(-135.f + direction*45.f);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.AddUnique("PlayerCharacter");

	m_MoveC = GetCharacterMovement();
	m_MeshC = (UStaticMeshComponent*)GetComponentByClass(UStaticMeshComponent::StaticClass());
	m_MeshC->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
	MeshScale = m_MeshC->GetRelativeScale3D();
	CameraFocusPos = GetActorLocation(),
	m_GameState = (AC_GS_CBTest*)UGameplayStatics::GetGameState(GetWorld());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsJumping)
	{
		APlayerCharacter::CheckGrounded();
	}

	APlayerCharacter::CheckTimelineAnimations(DeltaTime);

	if (!bIsJumping)
	{
		CameraFocusPos = GetActorLocation();
	}
	else
	{
		CameraFocusPos = FVector(GetActorLocation().X, GetActorLocation().Y, CameraFocusPos.Z);
	}
}


void APlayerCharacter::CheckTimelineAnimations(float DeltaTime)
{
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

	if (StretchCurve)
	{
		StretchTimeline.TickTimeline(DeltaTime);
	}


	if (DoubleJumpCurve)
	{
		DoubleJumpTimeline.TickTimeline(DeltaTime);
	}

	if (SquashCurve)
	{
		SquashTimeline.TickTimeline(DeltaTime);
	}

	if (WalkCurve) 
	{
		if (GetVelocity().Z == 0.0f && (XYMovement.X != 0 || XYMovement.Y != 0))
		{
			if (!WalkTimeline.IsPlaying() && !SquashTimeline.IsPlaying())
			{
				APlayerCharacter::PlayTimeline(&WalkTimeline, FName("CheckWalkTimeline"), WalkCurve, false);
			}
			WalkTimeline.TickTimeline(DeltaTime);
		}
		else if (WalkTimeline.IsPlaying())
		{
			m_MeshC->SetWorldScale3D(MeshScale);
			WalkTimeline.Stop();
		}
	}
}

void APlayerCharacter::CheckGrounded()
{
	if (ACharacter::GetVelocity().Z == 0.f) 
	{
		bIsJumping = false;
		bIsDoubleJumping = false;
		m_MoveC->GravityScale = 1.0f;

		if (SquashCurve)
		{
			APlayerCharacter::PlayTimeline(&SquashTimeline, FName("CheckSquashTimeline"), SquashCurve, true);
		}
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
		XYMovement.Y = -1;
		APlayerCharacter::SetYawValue();
	}
	else if (fValue == 1.00f)
	{
		XYMovement.Y = 1;
		APlayerCharacter::SetYawValue();
	}
	else
	{
		XYMovement.Y = 0;
	}
}

void APlayerCharacter::MoveX(float fValue)
{
	this->AddMovementInput(FVector(fValue*fSpeedValue, 0, 0));	
	if (fValue == -1.00f)
	{
		XYMovement.X = -1;
		APlayerCharacter::SetYawValue();
	}
	else if (fValue == 1.00f)
	{
		XYMovement.X = 1;
		APlayerCharacter::SetYawValue();
	}
	else
	{
		XYMovement.X = 0;
	}
}

void APlayerCharacter::SetYawValue()
{
	//Workaround with rotating Mesh only due to this->SetActorRotation() not working for whatever reason
	if (XYMovement.X == -1 && XYMovement.Y == -1)
		APlayerCharacter::Rotate(RotationDirections[0]);
	else if (XYMovement.X == 0 && XYMovement.Y == -1)
		APlayerCharacter::Rotate(RotationDirections[1]);
	else if (XYMovement.X == 1 && XYMovement.Y == -1)
		APlayerCharacter::Rotate(RotationDirections[2]);
	else if (XYMovement.X == 1 && XYMovement.Y == 0)
		APlayerCharacter::Rotate(RotationDirections[3]);
	else if (XYMovement.X == 1 && XYMovement.Y == 1)
		APlayerCharacter::Rotate(RotationDirections[4]);
	else if (XYMovement.X == 0 && XYMovement.Y == 1)
		APlayerCharacter::Rotate(RotationDirections[5]);
	else if (XYMovement.X == -1 && XYMovement.Y == 1)
		APlayerCharacter::Rotate(RotationDirections[6]);
	else if (XYMovement.X == -1 && XYMovement.Y == 0)
		APlayerCharacter::Rotate(RotationDirections[7]);
}

void APlayerCharacter::Rotate(float Goal)
{
	m_MeshC->SetWorldRotation(FRotator(0, Goal, 0));
	MeshRotation = m_MeshC->GetRelativeRotation();
}


void APlayerCharacter::Attack()
{
	bIsAttacking = true;
	if (AttackCurve)
	{
		APlayerCharacter::PlayTimeline(&AttackTimeline, FName("CheckAttackTimeline"), AttackCurve, true);
	}
}

void APlayerCharacter::Jump()
{
	if (!bIsJumping) 
	{
		ACharacter::LaunchCharacter(FVector(0, 0, fJumpValue), false, true);
		bIsJumping = true;
		m_MoveC->GravityScale += fAirGravity / 2.f;
		if (StretchCurve)
		{
			APlayerCharacter::PlayTimeline(&StretchTimeline, FName("CheckStretchTimeline"), StretchCurve, true);
		}
	}
	else if (!bIsDoubleJumping)
	{
		APlayerCharacter::DoubleJump();
	}
}

void APlayerCharacter::DoubleJump()
{
	ACharacter::LaunchCharacter(FVector(0, 0, fJumpValue), false, true);
	bIsDoubleJumping = true;
	m_MoveC->GravityScale = fAirGravity;

	if (DoubleJumpCurve)
	{
		APlayerCharacter::PlayTimeline(&DoubleJumpTimeline, FName("CheckDoubleJumpTimeline"), DoubleJumpCurve, true);
	}
}

void APlayerCharacter::PlayTimeline(FTimeline* Timeline, FName Function, UCurveFloat* Curve, bool PlayOnce)
{
	FOnTimelineFloat TimelineBind;
	TimelineBind.BindUFunction(this, Function);
	Timeline->AddInterpFloat(Curve, TimelineBind);
	if (PlayOnce) 
	{
		Timeline->SetLooping(false);
	}
	else 
	{
		Timeline->SetLooping(true);
	}
	Timeline->PlayFromStart();
}

void APlayerCharacter::CheckWalkTimeline(float DeltaTime)
{
	m_MeshC->SetWorldScale3D(FMath::Lerp(MeshScale, WalkScale, DeltaTime));
}

void APlayerCharacter::CheckAttackTimeline(float DeltaTime)
{
	m_MeshC->SetWorldRotation(FRotator(0, MeshRotation.Yaw+DeltaTime, 0));
}

void APlayerCharacter::CheckStretchTimeline(float DeltaTime)
{
	m_MeshC->SetWorldScale3D(FMath::Lerp(MeshScale, StretchScale, DeltaTime));
}

void APlayerCharacter::CheckDoubleJumpTimeline(float DeltaTime)
{
	m_MeshC->SetWorldRotation(FRotator(MeshRotation.Pitch+DeltaTime, MeshRotation.Yaw, 0));
}

void APlayerCharacter::CheckSquashTimeline(float DeltaTime)
{
	m_MeshC->SetWorldScale3D(FMath::Lerp(MeshScale, SquashScale, DeltaTime));
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

		if (!((AC_Box*)OtherActor)->bDestroyTriggered) 
		{
			((AC_Box*)OtherActor)->OnDestroy();
		}
	}

	if (strActorName.StartsWith(TEXT("BP_Death")))
	{
		m_GameState->bGameOver = true;
		this->Destroy();
	}
}