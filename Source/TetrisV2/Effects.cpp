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

	CurrentEffect = RandEffects(FMath::RandRange(0, 3));
	


	
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
	GEngine->AddOnScreenDebugMessage(20, 10.f, FColor::Blue, FString::FromInt(LifeTime) + " step remaning.");
	if (IsLife())
	{
		KillHimSelf();
	}
}


void AEffects::ActivateEffect()
{
	switch (CurrentEffect)
	{
	case TIME_SPEEDUP:
		EffectSpeedUp();
		break;

	case DESTROY_LINE_VERTICAL:
		EffectDestroyLineVertical();
		break;

	case ONLY_PALKA:
		EffectOnlyPalka();
		break;

	case TIME_SLOWDOWN:
		EffectTimeSlowDown();
		break;
	}
}

void AEffects::EffectSpeedUp()
{

}

void AEffects::EffectDestroyLineVertical()
{

}

void AEffects::EffectOnlyPalka()
{

}

void AEffects::EffectTimeSlowDown()
{

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
void AEffects::KillHimSelf()
{
	APlayerCam* cam = Cast<APlayerCam>(AttachedPlayerCam);
	ACub* cub = Cast<ACub>(AttachedCub);

	if (cam)
	{
		cam->ClearEffect();
	}
	if (cub)
	{
		cub->ClearEffect();
	}
	Destroy();
}