// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include "Effects.h"

#include "Cub.generated.h"

UCLASS()
class TETRISV2_API ACub : public AActor
{
	GENERATED_BODY()

		


	
public:	
	// Sets default values for this actor's properties
	ACub();

	void SetColor(UMaterialInstance* Color);
	void ReturnCoreColor();

	void SetEffect(AEffects* effect);
	void ClearEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
private:
	UStaticMeshComponent* CoreMesh;
	UMaterialInstance* CoreColor = nullptr;
	AEffects* CurrentEffect = nullptr;

public:

	//чтобы не переопределять деструктор или "this->Destroy()"
	void DestroyThisCub();

};
