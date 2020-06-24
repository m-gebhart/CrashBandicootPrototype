#include "C_DeathWall.h"

AC_DeathWall::AC_DeathWall() :
	bIsMoving(true),
	bReachedStop(false),
	fXStopLocation(0.f),
	fStopTime(4.f)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AC_DeathWall::BeginPlay()
{
	Super::BeginPlay();
	currentLocation = this->GetActorLocation();
}

void AC_DeathWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsMoving) {
		SetActorLocation(FVector(currentLocation.X - fWallSpeed * DeltaTime, currentLocation.Y, currentLocation.Z));
	}
	currentLocation = this->GetActorLocation();

	/*if (!bReachedStop && currentLocation.X < fStopTime)
	{
		AC_DeathWall::InitializeStop();
	}

	if (bReachedStop)
	{
		fStopTime -= DeltaTime;
		if (fStopTime < 0.f)
		{
			bIsMoving = true;
		}
	}*/
}

void AC_DeathWall::InitializeStop()
{
	bIsMoving = false;
	bReachedStop = true;
	//CameraShake-Function
}