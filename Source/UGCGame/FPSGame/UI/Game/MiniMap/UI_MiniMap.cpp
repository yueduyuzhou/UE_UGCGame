// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MiniMap.h"
#include "ThreadManage.h"
#include "../../../FPSGamePlayerController.h"
#include "Components/Image.h"

UUI_MiniMap::UUI_MiniMap()
	:LocalPlayerController(nullptr)

{

}

void UUI_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterToPlayerController();
}

void UUI_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LocalPlayerController)
	{
		if (APawn * PlayerPawn = LocalPlayerController->GetPawn())
		{
			if (PlayerArrow)
			{
				FTransform CharacterTransform = PlayerPawn->GetActorTransform();
				PlayerArrow->SetRenderTransformAngle(CharacterTransform.Rotator().Yaw);

				FVector2D NewTranslation = FVector2D(
					CharacterTransform.GetLocation().Y * -0.25f,
					CharacterTransform.GetLocation().X * 0.25f);
				MapImage->SetRenderTranslation(NewTranslation);
			}
		}
	}
}

void UUI_MiniMap::RegisterToPlayerController()
{
	if (AFPSGamePlayerController * PlayerController = Cast<AFPSGamePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		LocalPlayerController = PlayerController;
		PlayerController->MiniMapUI = this;
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.2f, [&]()
			{
				RegisterToPlayerController();
			});
	}
}

void UUI_MiniMap::UpdateMiniMapImage(UTexture2D* InTexture)
{
	if (InTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InTexture);
		MapImage->SetBrush(Brush);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[class UUI_MiniMap] : Update MiniMap Image, InTexture Is Null"));
	}
}
