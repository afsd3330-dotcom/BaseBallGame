#include "Game/BBGGameStateBase.h"
#include "kismet/GameplayStatics.h"
#include "Player/BBGPlayerController.h"


void ABBGGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ABBGPlayerController* BBGPC = Cast<ABBGPlayerController>(PC);
			if (IsValid(BBGPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game ");
				BBGPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
