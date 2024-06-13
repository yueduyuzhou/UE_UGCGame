// Fill out your copyright notice in the Description page of Project Settings.


#include "HypermarketCharacterBase.h"
#include "HypermarketGameState.h"

AHypermarketCharacterBase::AHypermarketCharacterBase()
	:WeaponPrimaryServer(nullptr),
	WeaponSecondaryServer(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHypermarketCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHypermarketCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHypermarketCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
}

void AHypermarketCharacterBase::EquipItem(const int32& InTableID)
{
	if (AGameStateBase * MyGS = GetWorld()->GetGameState())
	{
		if (AHypermarketGameState * MyHGS = Cast<AHypermarketGameState>(MyGS))
		{
			if (const FHypermarketTable * HTable = MyHGS->GetWeaponTableTemplate(InTableID))
			{
				PurchaseWeapon(HTable);
			}
		}
	}
}

void AHypermarketCharacterBase::PurchaseWeapon(const FHypermarketTable* InHTable)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//生成
	AWeaponBaseServer* ServerWeapon = GetWorld()->SpawnActor<AWeaponBaseServer>(InHTable->ItemClass, GetActorTransform(), SpawnInfo);
	//预处理并装备当前持有武器
	ServerWeapon->EquipWeapon();

	switch (InHTable->WeaponType)
	{
		case EWeaponType::AK47:
		{
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
		case EWeaponType::M4A1:
		{
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
		case EWeaponType::MP7:
		{
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
		case EWeaponType::DESERTEAGLE:
		{
			EquipSecondaryWeapon(ServerWeapon);
			break;
		}
		case EWeaponType::SNIPER:
		{
			EquipPrimaryWeapon(ServerWeapon);
			break;
		}
	}
}

void AHypermarketCharacterBase::EquipPrimaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponPrimaryServer)
	{
		WeaponPrimaryServer->Destroy();
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : EquipPrimaryWeapon, Weapon Type = %d"), InWeaponBaseServer->WeaponType));
	UE_LOG(LogTemp, Warning, TEXT("[class AHypermarketCharacterBase] : Equip Primary Weapon"));

	ActiveWeapon = InWeaponBaseServer->WeaponType;

	WeaponPrimaryServer = InWeaponBaseServer;
	WeaponPrimaryServer->SetOwner(this);

	WeaponPrimaryServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
}

void AHypermarketCharacterBase::EquipSecondaryWeapon(AWeaponBaseServer* InWeaponBaseServer)
{
	if (WeaponSecondaryServer)
	{
		WeaponSecondaryServer->Destroy();
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("[class AFPSGameCharacterBase] : EquipSecondaryWeapon, Weapon Type = %d"), InWeaponBaseServer->WeaponType));
	UE_LOG(LogTemp, Warning, TEXT("[class AHypermarketCharacterBase] : Equip Secondary Weapon"));

	ActiveWeapon = InWeaponBaseServer->WeaponType;

	WeaponSecondaryServer = InWeaponBaseServer;
	WeaponSecondaryServer->SetOwner(this);

	WeaponSecondaryServer->K2_AttachToComponent(Mesh, TEXT("Weapon_Rifle"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
}