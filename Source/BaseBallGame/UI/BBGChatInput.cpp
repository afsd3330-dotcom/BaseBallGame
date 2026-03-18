#include "UI/BBGChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/BBGPlayerController.h"

void UBBGChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UBBGChatInput::NativeDestruct()
{
		Super::NativeDestruct();

		if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
		{
			EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
		}
}

void UBBGChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ABBGPlayerController* OwningBBGPlayerController = Cast<ABBGPlayerController>(OwningPlayerController);
			if (IsValid(OwningBBGPlayerController) == true)
			{
				OwningBBGPlayerController->SetChatMessageString(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}