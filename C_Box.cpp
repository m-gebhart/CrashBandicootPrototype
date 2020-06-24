#include "CrashBandicootTest/C_Box.h"
#include "CrashBandicootTest/PlayerCharacter.h"
#include "GameFramework/Actor.h" 
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


AC_Box::AC_Box()
{
	PrimaryActorTick.bCanEverTick = false;
}