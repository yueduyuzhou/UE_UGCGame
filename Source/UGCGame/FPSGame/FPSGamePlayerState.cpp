// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGamePlayerState.h"

AFPSGamePlayerState::AFPSGamePlayerState()
{
	Health = 100.f;
	MaxHealth = 100.f;
}

void AFPSGamePlayerState::DeathResetData()
{
	Health = MaxHealth;
}
