// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCam.h"

#include "Effects.generated.h"



enum RandEffects
{
	TIME_SPEEDUP,
	DESTROY_LINE_VERTICAL,
	ONLY_PALKA,
	TIME_SLOWDOWN,
};




UCLASS()
class TETRISV2_API AEffects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffects();

protected:

	virtual void BeginPlay() override;

private:
	int StartTime = 0;
	int DeadTime = 0;
	int LifeTime = 10;

	FTimerManager* TimerBlet;

	void DecreaseLifeTime() { --LifeTime; }



	RandEffects CurrentEffect;
	
	
	bool GC = false; //GetController

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	RandEffects GetEffect() { return CurrentEffect; }
	void SetLifeTime(int time) { LifeTime = time; }
	void ActivateEffect();



	void EffectSpeedUp();
	void EffectDestroyLineVertical();
	void OnlyPalka();
	void TimeSlowDown();


};
