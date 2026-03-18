#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BBGPawn.generated.h"

UCLASS()
class BASEBALLGAME_API ABBGPawn : public APawn
{
	GENERATED_BODY()

public:
	ABBGPawn();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController)override;
};
