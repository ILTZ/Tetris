// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Cub.h"

#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


#include "PlayerCam.generated.h"

UCLASS()
class TETRISV2_API APlayerCam : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Элементы камеры 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = RootComp)
		UBoxComponent* MyRootComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraSpring)
		USpringArmComponent* CameraSpring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
		UCameraComponent* Camera;
	/////////////////////////////////////////////////////////////////////


private:
	//Массив для хранения координат кубов
	TArray<TArray<int>> FiguresArray;

	struct CurrentPoint
	{
		int x;
		int y;
	};

	TArray<CurrentPoint> CurrentFigureA;
	TArray<CurrentPoint> CurrentFigureB;


	
	

	//Используются TArray вложенные в TArray, чтобы сохранить кроссплатформенность
	void FillArrays();
	void SetOnBoard();
	bool Spawned = false;
	/// Фнукции движения влево/вправо
	void LeftMove();
	void RightMove();
	void Rotate();

	void MoveDown();
	bool FieldCheck();
	//Используется 2-й доп массив для сохранения координат
	void SyncArrays();
	void ReturnCoords();


	float Time = 1.0f;
	float bufferTime = 0.0f;

	//Хранит текущую фигуру
	TArray<ACub*> CurrentFigure;


	TArray<TArray<int>> LogicArray;
	const int FieldLength = 10;
	const int FieldHight = 20;

};
