#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBGGameStateBase.generated.h"

UCLASS()
class BASEBALLGAME_API ABBGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:

	UFUNCTION(NetMulticast,Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
};
