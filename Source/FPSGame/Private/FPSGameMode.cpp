// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissonSucess)
{
	if (InstigatorPawn) {
		//If this class exists on the map
		if (SpectatingViewpointClass) {

			//Getting all actors of this class
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			// Change view target if any valid found
			if (ReturnedActors.Num() > 0) {
				AActor* NewViewTarget = ReturnedActors[0];

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
					APlayerController* PC = It->Get();
					if (PC) {
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SpecatingViewpointClass is nullptr. Please update GameMode class with valid subclass. Can't change spectating view"));
		}

		
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS) {
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissonSucess);
	}
	
	OnMissionCompleted(InstigatorPawn, bMissonSucess);
}
