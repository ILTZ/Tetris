// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


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
	
	int LifeTime = 10;

	void DecreaseLifeTime();



	RandEffects CurrentEffect;
	
	
	bool GC = false; //GetController

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	RandEffects GetEffect() { return CurrentEffect; }

	bool IsLife() { return ((LifeTime < 1) ? true : false); }



	void ActivateEffect();



	void EffectSpeedUp();
	void EffectDestroyLineVertical();
	void EffectOnlyPalka();
	void EffectTimeSlowDown();


};
