// Fill out your copyright notice in the Description page of Project Settings.


#include "Cub.h"

// Sets default values
ACub::ACub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* StaticMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("/Engine/BasicShapes/Cube")).Object;

	if (StaticMesh)
	{
		CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMesh"));
		RootComponent = CoreMesh;
		CoreMesh->SetStaticMesh(StaticMesh);
	}

}

// Called when the game starts or when spawned
void ACub::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACub::SetColor(UMaterialInstance* Color)
{
	if (CoreMesh)
	{
		CoreMesh->SetMaterial(0, Color);
	}
}

