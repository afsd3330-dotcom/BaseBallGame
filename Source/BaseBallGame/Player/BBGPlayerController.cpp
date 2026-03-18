#include "Player/BBGPlayerController.h"
#include "UI/BBGChatInput.h"
#include "BaseBallGame.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/BBGGameModeBase.h"
#include "BBGPlayerState.h"
#include "Net/UnrealNetwork.h"

ABBGPlayerController::ABBGPlayerController()
{
	bReplicates = true;
}

void ABBGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsLocalController() == false)
	{
		return;
	}

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UBBGChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}


void ABBGPlayerController::SetChatMessageString(const FString& InChatMessgeString)
{
	ChatMessageString = InChatMessgeString;
	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessgeString);
		ABBGPlayerState* BBGPS = GetPlayerState<ABBGPlayerState>();
		if (IsValid(BBGPS) == true)
		{
			//FString CombinedMessageString = BBGPS->PlayerNameString + TEXT(":") + InChatMessgeString;
			FString CombinedMessageString = BBGPS->GetPlayerInfoString() + TEXT(":") + InChatMessgeString;
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ABBGPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::White, 10.0f);
	//FString NetModeString = BaseBallGameFunctionLibrary::GetNetModeString(this);
	//FString CombinedMessageString = FString::Printf(TEXT("%s:%s"), *NetModeString, *InChatMessageString);
	BaseBallGameFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ABBGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ABBGPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
//	for (TActorIterator<ABBGPlayerController> It(GetWorld()); It; ++It)
//	{
//		ABBGPlayerController* BBGPlayerController = *It;
//		if (IsValid(BBGPlayerController) == true)
//		{
//			BBGPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
//		}
//	}
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ABBGGameModeBase* BBGGM = Cast<ABBGGameModeBase>(GM);
		if (IsValid(BBGGM) == true)
		{
			BBGGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ABBGPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
