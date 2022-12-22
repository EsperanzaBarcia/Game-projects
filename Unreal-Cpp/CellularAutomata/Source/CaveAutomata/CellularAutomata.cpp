// Fill out your copyright notice in the Description page of Project Settings.


#include "CellularAutomata.h"
#include "Cell.h"
#include "PaperSpriteComponent.h"

// Sets default values
ACellularAutomata::ACellularAutomata()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACellularAutomata::BeginPlay()
{

	Super::BeginPlay();

	//Referencia a player controller
	controller = GEngine->GetFirstLocalPlayerController(GetWorld());

	//Input del jugador, teclado
	EnableInput(controller);

	//Permite los eventos de input
	InputComponent->BindAction("Generate new map", IE_Pressed, this, &ACellularAutomata::InputKey);

	//Instancia el mapa con las células base
	SetUpMap();

	//Genera el mapa de forma procedural
	GenerateMap();

}

// Called every frame
void ACellularAutomata::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACellularAutomata::SetUpMap()
{
	//Asigna el tamaño al array
	map.AddDefaulted(width * height);

	//Llena el mapa vacío
	FillMap();
}


void ACellularAutomata::GenerateMap()
{
	currentState = CellState::Process;

	//Rellena aleatoriamente el mapa
	RandomFill();

	//Aplicamos la lógica de las celdas
	for (int index = 0; index < iterations; index++)
	{
		CellRules();
	}

	currentState = CellState::Idle;

	////Cambiamos los sprites de las celdas
	FillCells();
}

void ACellularAutomata::FillCells()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//Guardamos en un array las referencias a todos los sprites
			UActorComponent* sprite = cellList[(y * width) + x]->GetComponentByClass(UPaperSpriteComponent::StaticClass());

			if (sprite)
			{
				//Si esta viva es suelo
				if (map[(y * width) + x] == 1)
				{
					((UPaperSpriteComponent*)sprite)->SetSpriteColor(liveColor);
				}
				//Si está muerta es muro
				else
				{
					((UPaperSpriteComponent*)sprite)->SetSpriteColor(deadColor);
				}

			}

		}

	}
}

void ACellularAutomata::RandomFill()
{
	//Generador de números aleatorios
	FMath::RandInit(FPlatformTime::Seconds());

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//Asignamos los límites
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
			{
				map[(y * width) + x] = 1;
			}
			else
			{
				//Según el número la celda tendrá vida o no
				if (FMath::FRandRange(0, 100) < randomFillPercent)
				{
					map[(y * width) + x] = 1;
				}
				else
				{
					map[(y * width) + x] = 0;
				}
			}

		}
	}


}

void ACellularAutomata::CellRules()
{
	//Por cada celda
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//contamos sus vecinos
			int neighbours = CountNeighbours(x, y);

			//Asignamos las reglas
			if (neighbours > dieNeighbours)
			{
				map[(y * width) + x] = 1;
			}
			else if (neighbours < dieNeighbours)
			{
				map[(y * width) + x] = 0;
			}
		}

	}
}

int ACellularAutomata::CountNeighbours(int x, int y)
{
	int neighboursCount = 0;

	//Recorremos los vecinos en un grid de 6 	

	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++)
	{
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++)
		{
			if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height)
			{
				//Evitamos contarnos a nosotros
				if (neighbourX != x || neighbourY != y)
				{
					neighboursCount += map[(neighbourY * width) + neighbourX];
				}
			}
			else
			{
				neighboursCount++;
			}
		}

	}

	return neighboursCount;
}

void ACellularAutomata::FillMap()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			//posición y rotación en las que vamos a spawnear
			FVector* location = new FVector((-width * 100) / 2 + (x * 100) + 50, 0, (-height / 2) + (y * 100) + 50);
			FRotator* rotation = new FRotator(0, 0, 0);
			FActorSpawnParameters params;

			//spawn
			ACell* cell = (ACell*)GetWorld()->SpawnActor(genericCellPrefab, location, rotation, params);
			cell->info = FVector2D(x, y);
			cellList.Add(cell);
		}

	}

}

void ACellularAutomata::InputKey()
{
	if (currentState == CellState::Idle)
	{
		GenerateMap();
	}
}

