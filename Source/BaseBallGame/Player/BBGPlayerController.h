#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBGPlayerController.generated.h"

class UBBGChatInput;
class UUserWidget;

UCLASS()
class BASEBALLGAME_API ABBGPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ABBGPlayerController();

	virtual void BeginPlay() override;
	void SetChatMessageString(const FString& InChatMessgeString);
	void PrintChatMessageString(const FString& InChatMessageString);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Client,Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server,Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBBGChatInput>ChatInputWidgetClass;
	UPROPERTY()
	TObjectPtr<UBBGChatInput>ChatInputWidgetInstance;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	FString ChatMessageString;
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
