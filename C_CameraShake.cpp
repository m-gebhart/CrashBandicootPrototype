#include "CrashBandicootTest/C_CameraShake.h"
#include "CrashBandicootTest/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


UC_CameraShake::UC_CameraShake()
{
	APlayerCharacter* CameraPlayer = (APlayerCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (CameraPlayer)
	{
		OscillationDuration = CameraPlayer->fDuration;
		OscillationBlendInTime = CameraPlayer->BlendInOut.X;
		OscillationBlendOutTime = CameraPlayer->BlendInOut.Y;

		RotOscillation.Pitch.Amplitude = FMath::RandRange(CameraPlayer->PitchAmplitudeRange.X, CameraPlayer->PitchAmplitudeRange.Y);
		RotOscillation.Pitch.Frequency = FMath::RandRange(CameraPlayer->PitchFrequencyRange.X, CameraPlayer->PitchFrequencyRange.Y);
	
		RotOscillation.Yaw.Amplitude = FMath::RandRange(CameraPlayer->YawAmplitudeRange.X, CameraPlayer->YawAmplitudeRange.Y);
		RotOscillation.Yaw.Frequency = FMath::RandRange(CameraPlayer->YawFrequencyRange.X, CameraPlayer->YawFrequencyRange.Y);
	}
}