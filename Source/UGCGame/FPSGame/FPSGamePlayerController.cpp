// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGamePlayerController.h"
#include "FPSGameCharacterBase.h"
#include "FPSGameHUD.h"
#include "UGCGame/UGCGameInstance.h"
#include "FPSGameGameMode.h"

AFPSGamePlayerController::AFPSGamePlayerController()
{

}

void AFPSGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AFPSGamePlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AFPSGamePlayerController::OnLeftMouseButtonUp);

	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AFPSGamePlayerController::OnRightMouseButtonDown);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AFPSGamePlayerController::OnRightMouseButtonUp);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSGamePlayerController::Jump);

	InputComponent->BindAction("LowSpeedWalk", IE_Pressed, this, &AFPSGamePlayerController::OnLeftShiftButtonDown);
	InputComponent->BindAction("LowSpeedWalk", IE_Released, this, &AFPSGamePlayerController::OnLeftShiftButtonUp);

	InputComponent->BindAction("AmmoReload", IE_Pressed, this, &AFPSGamePlayerController::AmmoReload);

	InputComponent->BindAxis("MoveForward", this, &AFPSGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSGamePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSGamePlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AFPSGamePlayerController::LookUpAtRate);
}

void AFPSGamePlayerController::OnLeftMouseButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponFirePressed();
	}
}

void AFPSGamePlayerController::OnLeftMouseButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponFireReleassed();
	}
}

void AFPSGamePlayerController::OnRightMouseButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponAimingPressed();
	}
}

void AFPSGamePlayerController::OnRightMouseButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->WeaponEndAimingReleassed();
	}
}

void AFPSGamePlayerController::Jump()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->Jump();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonDown()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LowSpeedWalk();
	}
}

void AFPSGamePlayerController::OnLeftShiftButtonUp()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->NormalSpeedWalk();
	}
}

void AFPSGamePlayerController::AmmoReload()
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->AmmoReload();
	}
}

void AFPSGamePlayerController::MoveForward(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveForward(Value);
	}
}

void AFPSGamePlayerController::MoveRight(float Value)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->MoveRight(Value);
	}
}

void AFPSGamePlayerController::TurnAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->TurnAtRate(Rate);
	}
}

void AFPSGamePlayerController::LookUpAtRate(float Rate)
{
	if (AFPSGameCharacterBase * MyCharacter = Cast<AFPSGameCharacterBase>(GetCharacter()))
	{
		MyCharacter->LookUpAtRate(Rate);
	}
}

void AFPSGamePlayerController::ServerCallClientSendPlayerData_Implementation()
{
	if (UGameInstance * MyGI = GetWorld()->GetGameInstance())
	{
		if (UUGCGameInstance * MyUGCGI = Cast<UUGCGameInstance>(MyGI))
		{
			SendPlayerDataToServer(MyUGCGI->LocalPlayerData);
		}
	}
}

void AFPSGamePlayerController::SendPlayerDataToServer_Implementation(const FPlayerNetData& InPlayerData)
{
	if (AGameModeBase * MyGM = GetWorld()->GetAuthGameMode())
	{
		if (AFPSGameGameMode * MyFPSGM = Cast<AFPSGameGameMode>(MyGM))
		{
			SpawnPlayerCharacter(
				MyFPSGM->GetCharacterClass(InPlayerData.Team),
				MyFPSGM->GetNextSpawnTransform(InPlayerData));
		}
	}
}

void AFPSGamePlayerController::SpawnPlayerCharacter(UClass* InCharacterClass, const FTransform& InTransform)
{
	APawn* NewCharacter = GetWorld()->SpawnActor<APawn>(InCharacterClass, InTransform);
	if (NewCharacter)
	{
		Possess(NewCharacter);
	}
}

void AFPSGamePlayerController::PlayerCameraShake(TSubclassOf<UCameraShake> InCameraShake)
{
	ClientPlayCameraShake(InCameraShake);
}

void AFPSGamePlayerController::CrosshairRecoil()
{
	if (CrosshairUI)
	{
		CrosshairUI->DoCrosshairRecoil();
	}
}

void AFPSGamePlayerController::UpdateAmmo(const int32& InCurrentClipAmmo, const int32& InCurrentAmmo)
{
	if (CrosshairUI)
	{
		CrosshairUI->UpdateAmmo(InCurrentClipAmmo, InCurrentAmmo);
	}
}

void AFPSGamePlayerController::UpdateHealth(const float& InHealth, const float& InMaxHealth)
{
	if (CrosshairUI)
	{
		CrosshairUI->UpdateHealth(InHealth, InMaxHealth);
	}
}
