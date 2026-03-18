#include "BBGGameStateBase.h"
#include "Game/BBGGameModeBase.h"
#include "Player/BBGPlayerController.h"
#include "EngineUtils.h"
#include "Player/BBGPlayerState.h"

void ABBGGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABBGPlayerController* BBGPlayerController = Cast<ABBGPlayerController>(NewPlayer);
	if (IsValid(BBGPlayerController) == true)
	{
		BBGPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(BBGPlayerController);
		ABBGPlayerState* BBGPS = Cast<ABBGPlayerState>(BBGPlayerController->PlayerState);
		if (IsValid(BBGPlayerController) == true)
		{
			BBGPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		ABBGGameStateBase* BBGGameStateBase = GetGameState<ABBGGameStateBase>();
		if (IsValid(BBGGameStateBase) == true)
		{
			BBGGameStateBase->MulticastRPCBroadcastLoginMessage(BBGPS->PlayerNameString);
		}
	}
}

FString ABBGGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i) Numbers.Add(i);

	for (int32 i = Numbers.Num() - 1; i > 0; i--)
	{
		int32 J = FMath::RandRange(0, i);
		Numbers.Swap(i, J);
	}

	FString Result = FString::Printf(TEXT("%d%d%d"), Numbers[0], Numbers[1], Numbers[2]);
	return Result;
}

bool ABBGGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;
	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		if (bIsUnique == false)
		{
			break;
		}
		bCanPlay = true;
	}
	while (false);
	return bCanPlay;
}
FString ABBGGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABBGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}

void ABBGGameModeBase::PrintChatMessageString(ABBGPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	ABBGPlayerState* BBGPS = InChattingPlayerController->GetPlayerState<ABBGPlayerState>();
	if (IsValid(BBGPS) == false) return;
	if (BBGPS->CurrentGuessCount >= BBGPS->MaxGuessCount)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("기회를 모두 소진하셨습니다."));
		return;
	}
	FString GuessNumber = InChatMessageString.Right(3);
	FString GuessNumberString = InChatMessageString;
	if (IsGuessNumberString(GuessNumber) == true)
	{
		BBGPS->CurrentGuessCount++;
		FString ResultStr = JudgeResult(SecretNumberString, GuessNumber);
		FString FinalMsg = FString::Printf(TEXT("%s: %s -> %s"), *BBGPS->GetPlayerInfoString(), *GuessNumber, *ResultStr);
		for (auto PC : AllPlayerControllers)
		{
			if (IsValid(PC) == true) PC->ClientRPCPrintChatMessageString(FinalMsg);
		}
		int32 StrikeCount = FCString::Atoi(*ResultStr.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else if (GuessNumberString.Len() != 3)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("3자리 숫자를 입력해주세요"));
	}
	else
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("다시 입력하세요 "));
	}
}
void ABBGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (auto PC : AllPlayerControllers)
	{
		if (IsValid(PC) == true)
		{
			if (auto PS = PC->GetPlayerState<ABBGPlayerState>()) PS->CurrentGuessCount = 0;
			PC->NotificationText = FText::GetEmpty();
			PC->ClientRPCPrintChatMessageString(TEXT("새 게임이 시작되었습니다!"));
		}
	}
}

void ABBGGameModeBase::JudgeGame(ABBGPlayerController* InCgattingPlayerController, int InStrikeCount)
{
	bool bGameEnded = false;
	FString EndMessage;
	if (InStrikeCount == 3)
	{
		if (ABBGPlayerState* PS = InCgattingPlayerController->GetPlayerState<ABBGPlayerState>())
		{
			EndMessage = FString::Printf(TEXT("Winner: %s!"), *PS->PlayerNameString);
			bGameEnded = true;
		}
	}
	else
	{
		bool bAnyOneHasChance = false;
		for (auto PC : AllPlayerControllers)
		{
			if (auto PS = PC->GetPlayerState<ABBGPlayerState>())
			{
				if (PS->CurrentGuessCount < PS->MaxGuessCount) { bAnyOneHasChance = true; break; }
			}
		}
		if (bAnyOneHasChance == false)
		{
			EndMessage = FString::Printf(TEXT("무승부! 정답은 %s 였습니다."), *SecretNumberString);
			bGameEnded = true;
		}
	}
	if (bGameEnded == true)
	{
		for (auto PC : AllPlayerControllers)
		{
			if (IsValid(PC) == true) PC->NotificationText = FText::FromString(EndMessage);
		}
		FTimerHandle ResetHandle;
		GetWorldTimerManager().SetTimer(ResetHandle, this, &ABBGGameModeBase::ResetGame, 5.0f, false);
	}
}