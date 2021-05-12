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
	
	//�������� ������ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = RootComp)
		UBoxComponent* MyRootComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraSpring)
		USpringArmComponent* CameraSpring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
		UCameraComponent* Camera;
	/////////////////////////////////////////////////////////////////////


private:
	//������ ��� �������� ��������� �����
	TArray<TArray<int>> FiguresArray;

	struct CurrentPoint
	{
		int x;
		int y;
		int FigureNum;
	};

	TArray<CurrentPoint> CurrentFigureA;
	TArray<CurrentPoint> CurrentFigureB;

	//��������� �������� ����;
	void DrowGameField();
	//��� �����
	TArray<UMaterialInstance*> ColorsForBlocks;

	//������������ TArray ��������� � TArray, ����� ��������� ��������������������
	void FillArrays();
	//������� ������
	void SetOnBoard();
	bool Spawned = false;
	int CurrentNumberFigure = 0;
	//��������� ������ � �������� ������������ � ���������� ������
	void SetOnField();
	///
	void LeftMove();
	void RightMove();
	void Rotate();
	//��� ������� �� ������ "����" �������� ������ � 10 ���
	void DownDirection();
	//��� ������� ������ "����" ������ ����������� � �������� "1 ���"
	void ChangeTime();

	//�������������� ��������� ������ �� �������
	void MoveDown();
	//���������, ������ �� ������ � ���������� ������� �� ������ ����, ���� ����� ������������ ����
	bool FieldCheck();
	//������������ 2-� ��� ������ ��� ���������� ��������� (��� �� ������)
	void SyncArrays();
	//���������� ������ ���������� �� B-�������
	void ReturnCoords();
	//���� �� ������ ����� ������� ���-�� ���� - ���� ��������
	bool CheckEndGame();

	float Time = 1.0f;
	float bufferTime = 0.0f;

	//������ ������� ������
	TArray<ACub*> CurrentFigure;

	//������ ��� ����, ��� ���� �� ���� (����� ��������� ���������� �� ��� ��������)
	TArray<ACub*> AllFigures;

	//��������� � ���������� �������� ������ ��������� �� ����(��� �����������
	//� ������ ���������� ����� ������
	TArray<TArray<ACub*>> LogicPtrArray;
	//������ ����� ����� �� ���������� ������� ��� ���������� ����� �� 1-�
	void CheckLine();
	//� ������ ���������� ����� ����� ������ ���������� ����� � ������� ����������� �� 100 ����
	// + ���������� ���������
	void RefreshPtrArray();


	// ������ "��������������" �� ����� (1-�� � 0-��)
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
