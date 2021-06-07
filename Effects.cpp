// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Cub.h"
#include "PlayerCam.h"

// Sets default values
AEffects::AEffects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentEffect = RandEffects(FMath::RandRange(0, 2));
	//CurrentEffect = TIME_SPEEDUP;


	
}

void AEffects::BeginPlay()
{
	Super::BeginPlay();

	/*FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEffects::DecreaseLifeTime, 1.f, true);*/
}

// Called every frame
void AEffects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
void AEffects::DecreaseLifeTime()
{
	--LifeTime;
	if (IsLife())
	{
		KillHimSelf();
	}
}


void AEffects::AttachToCub(AActor* Cubus)
{
	if (Cubus != nullptr)
	{
		AttachedCub = Cubus;
	}
}
void AEffects::AttachToPlayerCam(AActor* PC)
{
	if (PC != nullptr)
	{
		AttachedPlayerCam = PC;
	}
}
void AEffects::KillHimSelf(bool activate)
{
	

	APlayerCam* cam = Cast<APlayerCam>(AttachedPlayerCam);
	ACub* cub = Cast<ACub>(AttachedCub);

	if (cam)
	{
		cam->ClearEffect();
		if (activate)
		{
			cam->ActivateEffect(CurrentEffect);
		}
	}
	if (cub)
	{
		cub->ClearEffect();
	}
	Destroy();
}