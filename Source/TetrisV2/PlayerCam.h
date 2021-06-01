// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Cub.h"

#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Effects.h"

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

	void DebugFieldRend();

	struct CurrentPoint
	{
		int x;
		int y;
		int FigureNum;
	};

	TArray<CurrentPoint> CurrentFigureA;
	TArray<CurrentPoint> CurrentFigureB;


	

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
	//Располагает блоки на поле в зависимости от их х/y значений
	void RendOnField();
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
	void ClearLogicAndPtrArray();


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

	UFUNCTION(BlueprintCallable)
	bool EffectOnBoard() { return (CurrentEffect ? true : false); }

	UFUNCTION(BlueprintCallable)
	bool EffectWasActivated() { return EffectActivated; }

	UFUNCTION(BlueprintCallable)
	int LifeTimeRamaninf() { return LifeTimeEffect; }

	UFUNCTION(BlueprintCallable)
	FString WhatsEffect() { return BuildStringEffect(); }



/////////////////////////Рандомно генерящиеся эффекты
private:
	AEffects* CurrentEffect = nullptr;
	UMaterialInstance* EffectInst = nullptr;
	//Возбращает правду, если число (0-10) <= "RandChance"
	bool TryGenEffect();
	//Выбирает 
	ACub* ChouseCub();
	const int RandChance = 3;
	void TrySetEffect();
	//Для запоминания ненулевых позиций в массиве указателей(для последующего рандома куба, на котором будет висеть эффект)
	struct ValuableCoords //Можно было бы и из имеющейся структуры, но так как-то понятнее
	{
		int x;
		int y;
	};
	TArray<ValuableCoords> ValCoords;
	//Добавляем в массим выше координаты кубов, находящихся на поле
	void FillValCoords();
	//Сколько эффект будет действовать
	int LifeTimeEffect = 10;
	void DecreaseLifeTime();
	bool EffectActivated = false;
	bool SpeedLock = false;
	//Следит за неактивированным и активированным эффектом
	void CheckEffect();

	void ReturnToNormal();
public:
	void ClearEffect();
	void ActivateEffect(RandEffects Effect);

private:
	void SpeedUpActivate();
	void SpeedDownActivate();

	bool OnlyPalka = false;
	void OnlyPalkaActivate();
	
	FString CurrentEffectString;
	FString BuildStringEffect();
};
