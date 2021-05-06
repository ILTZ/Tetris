// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCam.h"

// Sets default values
APlayerCam::APlayerCam()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootModel"));
	RootComponent = MyRootComponent;

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	CameraSpring->SetRelativeLocation(FVector(0, 0, 0));
	CameraSpring->AttachTo(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(CameraSpring, USpringArmComponent::SocketName);

	CameraSpring->SetRelativeRotation(FRotator(-90, -90, 180));
	CameraSpring->TargetArmLength = 1500.0f;
	CameraSpring->bDoCollisionTest = false;

	
}

// Called when the game starts or when spawned
void APlayerCam::BeginPlay()
{
	Super::BeginPlay();
	
	FillArrays();
	SetOnBoard();
}

// Called every frame
void APlayerCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Spawned)
	{
		int n = 3;
		for (int i = 4; i < CurrentFigureA.Num(); ++i)
		{
			CurrentFigureA[i].x = FiguresArray[n][i] % 2;
			CurrentFigureA[i].y = FiguresArray[n][i] / 2;
		}
		for (int i = 0; i < CurrentFigure.Num(); ++i)
		{
			CurrentFigure[i]->SetActorLocation(FVector(CurrentFigureA[i].x * 100, CurrentFigureA[i].y * 100, 0.0f));
		}

		bufferTime += DeltaTime;
		if (bufferTime >= Time)
		{
			MoveDown();
			bufferTime = 0;
			static int count = 0;
			for (auto pont : CurrentFigureA)
			{
				/*GEngine->AddOnScreenDebugMessage(count, .5f, FColor::Red, TEXT("Point x is:") + FString::FromInt(pont.x) + " Point y is:" + FString::FromInt(pont.y));
				count += 1;*/
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::FromInt(LogicArray.Num()));
				GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::FromInt(LogicArray[0].Num()));
			}
		}

		



	}


}

// Called to bind functionality to input
void APlayerCam::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftDir", IE_Pressed, this, &APlayerCam::LeftMove);
	PlayerInputComponent->BindAction("RightDir", IE_Pressed, this, &APlayerCam::RightMove);
	PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &APlayerCam::Rotate);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void APlayerCam::FillArrays()
{
	TArray<int> Figures;
	FiguresArray.Add(Figures = { 1,3,5,7 });
	FiguresArray.Add(Figures = { 2,4,5,7 });
	FiguresArray.Add(Figures = { 3,5,4,6 });
	FiguresArray.Add(Figures = { 3,5,4,7 });
	FiguresArray.Add(Figures = { 2,3,5,7 });
	FiguresArray.Add(Figures = { 3,5,7,6 });
	FiguresArray.Add(Figures = { 2,3,4,5 });

	for (int i = 0; i < 10; ++i)
	{
		TArray<int> Hight;
		Hight.Init(0, 20);
		LogicArray.Add(Hight);
	}
	for (int i = 0; i < 4; ++i)
	{
		CurrentPoint point;
		CurrentFigureB.Add(point);
	}
}

void APlayerCam::SetOnBoard()
{
	int n = 3;
	for (int i = 0; i < 4; ++i)
	{
		CurrentPoint point;
		CurrentFigureA.Add(point);
		CurrentFigureA[i].x = FiguresArray[n][i] % 2;
		CurrentFigureA[i].y = FiguresArray[n][i] / 2;
	}
	for (int i = 0; i < 4; ++i)
	{
		ACub* Cubus = GetWorld()->SpawnActor<ACub>();

		if (Cubus)
		{
			Cubus->SetActorLocation(FVector(CurrentFigureA[i].x * 100, CurrentFigureA[i].y * 100, 0.0f));
			CurrentFigure.Add(Cubus);
		}
	}
	Spawned = true;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void APlayerCam::LeftMove()
{
	SyncArrays();
	for (int i = 0; i < CurrentFigure.Num(); ++i)
	{
		int Direction = -1;
		CurrentFigureA[i].x += Direction;
	}
	if (!FieldCheck())
	{
		ReturnCoords();
	}
}
void APlayerCam::RightMove()
{
	SyncArrays();
	for (int i = 0; i < CurrentFigure.Num(); ++i)
	{
		int Direction = 1;
		CurrentFigureA[i].x += Direction;
	}
	if (!FieldCheck())
	{
		ReturnCoords();
	}
}
void APlayerCam::Rotate()
{
	SyncArrays();

	CurrentPoint point = CurrentFigureA[1];
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		int x = CurrentFigureA[i].y - point.y;
		int y = CurrentFigureA[i].x - point.x;
		CurrentFigureA[i].x = point.x - x;
		CurrentFigureA[i].y = point.y + y;
	}

	if (!FieldCheck())
	{
		ReturnCoords();
	}
}
void APlayerCam::MoveDown()
{
	SyncArrays();
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		CurrentFigureA[i].y += 1;
	}
	if (!FieldCheck())
	{
		ReturnCoords();
	}
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool APlayerCam::FieldCheck()
{
	for (auto Cub : CurrentFigureA)
	{
		if (Cub.x < 0 || Cub.x >= FieldLength || Cub.y >= FieldHight)
		{
			return false;
		}
		else if (LogicArray[Cub.x][Cub.y])
		{
			return false;
		}
	}


	return true;
}
void APlayerCam::SyncArrays()
{
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		CurrentFigureB[i].x = CurrentFigureA[i].x;
		CurrentFigureB[i].y = CurrentFigureA[i].y;
	}
}
void APlayerCam::ReturnCoords()
{
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		CurrentFigureA[i].x = CurrentFigureB[i].x;
		CurrentFigureA[i].y = CurrentFigureB[i].y;
	}
}