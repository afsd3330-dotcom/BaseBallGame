#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBGPlayerState.generated.h"

UCLASS()
class BASEBALLGAME_API ABBGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABBGPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)const override;
	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
