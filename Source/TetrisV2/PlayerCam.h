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
		int FigureNum;
	};

	TArray<CurrentPoint> CurrentFigureA;
	TArray<CurrentPoint> CurrentFigureB;

	//Отрисовка игрового поля;
	void DrowGameField();
	//Все цвета
	TArray<UMaterialInstance*> ColorsForBlocks;

	//Используются TArray вложенные в TArray, чтобы сохранить кроссплатформенность
	void FillArrays();
	//Спавним фигуру
	void SetOnBoard();
	bool Spawned = false;
	int CurrentNumberFigure = 0;
	//Добавляет фигуру с текущими координатами в логический массив
	void SetOnField();
	///
	void LeftMove();
	void RightMove();
	void Rotate();
	//При нажатии на кнопку "вниз" ускоряем таймер в 10 раз
	void DownDirection();
	//При отжатии кнопки "вниз" таймер возращается к значению "1 сек"
	void ChangeTime();

	//Принудительное опускание фигуры по таймеру
	void MoveDown();
	//Проверяет, заняты ли сейчас в логическом массиве те клетки поля, куда хочет переместится блок
	bool FieldCheck();
	//Используется 2-й доп массив для сохранения координат (для их отката)
	void SyncArrays();
	//Возвращает фигуре координаты из B-массива
	void ReturnCoords();
	//Если на второй линии массива что-то есть - игра окончена
	bool CheckEndGame();

	float Time = 1.0f;
	float bufferTime = 0.0f;

	//Хранит текущую фигуру
	TArray<ACub*> CurrentFigure;

	//Хранит все кубы, что были на поле (чтобы корректно уничтожить всё при рестарте)
	TArray<ACub*> AllFigures;

	//Синхронно с логическим массивом хранит указатели на кубы(для уничтожения
	//в случае построения линии блоков
	TArray<TArray<ACub*>> LogicPtrArray;
	//Проход снизу вверх по логичесому массиву для нахождения линии из 1-ц
	void CheckLine();
	//В случае нахождения целой линии просто уничтожаем блоки и двигаем вышестоящие на 100 вниз
	// + перемещаем указатели
	void RefreshPtrArray();


	// Хранит "местоположение" на доске (1-ми и 0-ми)
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
