#include "Player/BBGPawn.h"
#include "BaseBallGame.h"

ABBGPawn::ABBGPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABBGPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = BaseBallGameFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("BaseBallGame::BeginPlay() %s[%s]"), *BaseBallGameFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BaseBallGameFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ABBGPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = BaseBallGameFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("BaseBallGame::PossessedBy() %s[%s]"), *BaseBallGameFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BaseBallGameFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


