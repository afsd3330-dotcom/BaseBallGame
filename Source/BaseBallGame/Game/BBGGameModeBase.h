#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGGameModeBase.generated.h"

class ABBGPlayerController;

UCLASS()
class BASEBALLGAME_API ABBGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	virtual void BeginPlay() override;
	void PrintChatMessageString(ABBGPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(ABBGPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgeGame(ABBGPlayerController* InCgattingPlayerController, int InStrikeCount);

	 


protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ABBGPlayerController>> AllPlayerControllers;
	FTimerHandle GameResetTimerHandle;
};
