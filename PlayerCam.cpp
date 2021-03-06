// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/StaticMeshActor.h"
#include "PlayerCam.h"

// Sets default values
APlayerCam::APlayerCam()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootModel"));
	RootComponent = MyRootComponent;
	MyRootComponent->SetWorldLocation(FVector(-500.0f, -1000.0f, 0.0f));

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	CameraSpring->SetRelativeLocation(FVector(0, 0, 0));
	CameraSpring->AttachTo(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(CameraSpring, USpringArmComponent::SocketName);

	CameraSpring->SetRelativeRotation(FRotator(-90, -90, 180));
	CameraSpring->TargetArmLength = 2500.0f;
	CameraSpring->bDoCollisionTest = false;


	UMaterialInstance* BlueColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>
		(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/BlueColor_Inst.BlueColor_Inst'")).Get();
	UMaterialInstance* RedColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>
		(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/RedColor_Inst.RedColor_Inst'")).Get();
	UMaterialInstance* GreenColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>
		(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/GreenColor_Inst.GreenColor_Inst'")).Get();
	UMaterialInstance* OrangeColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>
		(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/OrangeColor_Inst.OrangeColor_Inst'")).Get();



	ColorsForBlocks.Add(BlueColor);
	ColorsForBlocks.Add(RedColor);
	ColorsForBlocks.Add(GreenColor);
	ColorsForBlocks.Add(OrangeColor);





	GM = MENU;
}

// Called when the game starts or when spawned
void APlayerCam::BeginPlay()
{
	Super::BeginPlay();
	
	GM = MENU;
	
}

// Called every frame
void APlayerCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Spawned && (GM == START_GAME))
	{

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

	PlayerInputComponent->BindAction("DownDir", IE_Pressed, this, &APlayerCam::DownDirection);
	PlayerInputComponent->BindAction("DownDir", IE_Released, this, &APlayerCam::ChangeTime);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCam::PauseGame);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void APlayerCam::FillArrays()
{
	TArray<int> Figures;

	FiguresArray.Add(Figures = { 5 ,15,25,35 }); // ????? ???????????? |
	FiguresArray.Add(Figures = { 14,24,25,35 }); // ?????? '-.
	FiguresArray.Add(Figures = { 15,25,24,34 }); // ?????? .-'
	FiguresArray.Add(Figures = { 15,25,24,35 }); // ???????? -|
	FiguresArray.Add(Figures = { 24,25,35,45 }); // ???????? ????? `|
	FiguresArray.Add(Figures = { 25,35,45,44 }); // ???????? ????? _|*/
	FiguresArray.Add(Figures = { 14,15,24,25 }); // ??????? 

	for (int i = 0; i < FieldLength; ++i)
	{
		TArray<int> Hight;
		Hight.Init(0, FieldHight);
		LogicArray.Add(Hight);

		TArray<ACub*> Cubs;
		Cubs.Init(nullptr, FieldHight);
		LogicPtrArray.Add(Cubs);
	}
	
	CurrentPoint point;
	CurrentFigureB.Init(point, 4);
	CurrentFigureA.Init(point, 4);


	CurrentNumberFigure = 0;
}

void APlayerCam::SetOnBoard()
{
	
	Spawned = false;
	CurrentNumberFigure = FMath::RandRange(0, 6);
	int n = CurrentNumberFigure;
	int RandColor = FMath::RandRange(0, ColorsForBlocks.Num() - 1);

	for (int i = 0; i < 4; ++i)
	{
		CurrentFigureA[i].x = FiguresArray[n][i] % 10;
		CurrentFigureA[i].y = FiguresArray[n][i] / 10;
	}

	if (CheckEndGame())
	{
		GM = END_GAME;
		return;
	}


	CurrentFigure.Reset(0);
	for (int i = 0; i < 4; ++i)
	{
		ACub* Cubus = GetWorld()->SpawnActor<ACub>();
		
		if (Cubus)
		{
			Cubus->SetColor(ColorsForBlocks[RandColor]);
			Cubus->SetActorLocation(FVector(CurrentFigureA[i].x * 100, CurrentFigureA[i].y * 100, 400.0f));
			CurrentFigureA[i].FigureNum = i;
			CurrentFigure.Add(Cubus);
		}
	}
	Spawned = true;
}
void APlayerCam::SetOnField()
{
	for (int i = 0; i < CurrentFigureB.Num(); ++i)
	{
		LogicArray[CurrentFigureB[i].x][CurrentFigureB[i].y] = 1;
		LogicPtrArray[CurrentFigureB[i].x][CurrentFigureB[i].y] = CurrentFigure[CurrentFigureB[i].FigureNum];


	}

	++UserScore;
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
		SetOnField();

	

		CheckLine();
		RefreshPtrArray();

		SetOnBoard();
	}
}
void APlayerCam::DownDirection()
{
	Time = 0.1f;
}
void APlayerCam::ChangeTime()
{
	Time = 1.0f;
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
		CurrentFigureB[i].FigureNum = CurrentFigureA[i].FigureNum;
	}
}
void APlayerCam::ReturnCoords()
{
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		CurrentFigureA[i].x = CurrentFigureB[i].x;
		CurrentFigureA[i].y = CurrentFigureB[i].y;
		CurrentFigureA[i].FigureNum = CurrentFigureB[i].FigureNum;
	}
}
void APlayerCam::CheckLine()
{
	int k = FieldHight - 1;
	for (int i = FieldHight - 1; i > 0; --i)
	{
		int count = 0;
		for (int j = 0; j < FieldLength; ++j)
		{
			if (LogicArray[j][i] != 0)
			{
				++count;
			}
			LogicArray[j][k] = LogicArray[j][i];

		}
		if (count < FieldLength && (k != 0))
		{
			--k;
		}
	}
	
	
}
void APlayerCam::RefreshPtrArray()
{
	
	for (int i = FieldHight - 1; i > 0; --i)
	{
		int count = 0;
		for (int j = 0; j < FieldLength; ++j)
		{
			if (LogicPtrArray[j][i])
			{
				++count;
			}
		}

		if (!(count < FieldLength))
		{
			for (int c = 0; c < FieldLength; ++c)
			{
				LogicPtrArray[c][i]->Destroy();
				LogicPtrArray[c][i] = nullptr;
			}

			for (int h = i - 1; h > 0; --h)
			{
				for (int y = 0; y < FieldLength; ++y)
				{
					if (LogicPtrArray[y][h] != nullptr)
					{
						FVector Location = LogicPtrArray[y][h]->GetActorLocation();
						Location.Y += 100.0f;
						LogicPtrArray[y][h]->SetActorLocation(Location);
						LogicPtrArray[y][h + 1] = LogicPtrArray[y][h];
						LogicPtrArray[y][h] = nullptr;
					}
				}
			}
			//????? ????? ?? ???????????? ????? ?????, ??????? ????? ???????
			++i;
			UserScore += FieldLength;
		}
	}
}
bool APlayerCam::CheckEndGame()
{
	for (int i = 0; i < CurrentFigureA.Num(); ++i)
	{
		//???? ????? ?????? ??? ?????? ??????? ???????? - ?? ???????? ????? ?????? ???? ? ???? ????????
		if (LogicArray[CurrentFigureA[i].x][CurrentFigureA[i].y])
		{
			return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void APlayerCam::BeginGame()
{
	
	FillArrays();
	SetOnBoard();

	GM = START_GAME;
}
void APlayerCam::PauseGame()
{
	if (GM == START_GAME)
	{
		GM = PAUSE;
	}
	else if (GM == PAUSE)
	{
		GM = START_GAME;
	}
}
void APlayerCam::RestartGame()
{
	ClearLogicAndPtrArrays();




	UserScore = 0;

	FillArrays();
	SetOnBoard();
	GM = START_GAME;
}
void APlayerCam::ClearLogicAndPtrArrays()
{
	for (int i = 0; i < FieldHight; ++i)
	{
		for (int j = 0; j < FieldLength; ++j)
		{
			if (LogicArray[j][i])
			{
				LogicArray[j][i] = 0;
			}
			if (LogicPtrArray[j][i])
			{
				LogicPtrArray[j][i]->Destroy();
				LogicPtrArray[j][i] = nullptr;
			}
		}
	}
}