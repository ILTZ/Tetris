// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Cub.h"

#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


#include "PlayerCam.generated.h"

UENUM(BlueprintType)
enum GameMode
{
	MENU		= 0		UMETA(DisplayName = "Menu"),
	START_GAME	= 1		UMETA(DisplayName = "StartGame"),
	PAUSE		= 2		UMETA(DisplayName = "Pause"),
	END_GAME	= 3		UMETA(DisplayName = "EndGame"),
};




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
	
	//Ёлементы камеры 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = RootComp)
		UBoxComponent* MyRootComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraSpring)
		USpringArmComponent* CameraSpring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
		UCameraComponent* Camera;
	/////////////////////////////////////////////////////////////////////


private:
	//ћассив дл€ хранени€ координат кубов
	TArray<TArray<int>> FiguresArray;

	struct CurrentPoint
	{
		int x;
		int y;
		int FigureNum;
	};

	TArray<CurrentPoint> CurrentFigureA;
	TArray<CurrentPoint> CurrentFigureB;


	
	

	//»спользуютс€ TArray вложенные в TArray, чтобы сохранить кроссплатформенность
	void FillArrays();
	//—павним фигуру
	void SetOnBoard();
	bool Spawned = false;
	int CurrentNumberFigure = 0;
	//ƒобавл€ет фигуру с текущими координатами в логический массив
	void SetOnField();
	///
	void LeftMove();
	void RightMove();
	void Rotate();
	//ѕри нажатии на кнопку "вниз" ускор€ем таймер в 10 раз
	void DownDirection();
	//ѕри отжатии кнопки "вниз" таймер возращаетс€ к значению "1 сек"
	void ChangeTime();

	//ѕринудительное опускание фигуры по таймеру
	void MoveDown();
	//ѕровер€ет, зан€ты ли сейчас в логическом массиве те клетки пол€, куда хочет переместитс€ блок
	bool FieldCheck();
	//»спользуетс€ 2-й доп массив дл€ сохранени€ координат (дл€ их отката)
	void SyncArrays();
	//¬озвращает фигуре координаты из B-массива
	void ReturnCoords();
	//≈сли на второй линии массива что-то есть - игра окончена
	bool CheckEndGame();

	float Time = 1.0f;
	float bufferTime = 0.0f;

	//’ранит текущую фигуру
	TArray<ACub*> CurrentFigure;
	//—инхронно с логическим массивом хранит указатели на кубы(дл€ уничтожени€
	//в случае построени€ линии блоков
	TArray<TArray<ACub*>> LogicPtrArray;
	//ѕроход снизу вверх по логичесому массиву дл€ нахождени€ линии из 1-ц
	void CheckLine();
	//¬ случае нахождени€ целой линии просто уничтожаем блоки и двигаем вышесто€щие на 100 вниз
	// + перемещаем указатели
	void RefreshPtrArray();


	// ’ранит "местоположение" на доске (1-ми и 0-ми)
	// 0 0 0 0 0 0
	// 0 0 1 0 0 0
	// 0 1 1 1 0 0
	TArray<TArray<int>> LogicArray;

	const int FieldLength = 10;
	const int FieldHight = 20;



	int UserScore = 0;
	GameMode GM;


public:

	UFUNCTION(BlueprintCallable)
	GameMode GetGM() const { return GM; }

	UFUNCTION(BlueprintCallable)
	int GetScore() const { return UserScore; }

	UFUNCTION(BlueprintCallable)
	void BeginGame();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void PauseGame();


};
