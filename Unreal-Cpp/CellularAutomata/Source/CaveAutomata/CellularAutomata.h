// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell.h"
#include "GameFramework/Actor.h"
#include "CellularAutomata.generated.h"

UCLASS()
class CAVEAUTOMATA_API ACellularAutomata : public AActor
{
	GENERATED_BODY()

public:

	//Prefab de célula
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		UClass* genericCellPrefab;

	//Datos de la dungeon 
	//Ancho
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		int width;

	//Alto
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		int height;

	//Porcentaje de relleno	
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		int randomFillPercent;

	//Sprite para suelo
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		FLinearColor liveColor;

	//Sprite para muros
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		FLinearColor deadColor;

	//Iteraciones que se harán para suavizar el mapa
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		int iterations;

private:

	//array que guarda el mapa
	TArray<int> map;

	//enumeración para saber en qué estado esta la caverna para evitar que se colapse
	enum CellState { Idle, Process };
	CellState currentState;

	//Si hay menos vecinos que este número la célula morirá
	const int dieNeighbours = 4;

	//Lista de las celdas para cambiar sus sprites
	TArray<ACell*>cellList;

	//Referencia al Player controller
	APlayerController* controller;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Constructor por defecto
	ACellularAutomata();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	//Lógica
	//Función que asigna los datos del mapa
	void SetUpMap();

	//Función que genera el mapa
	void GenerateMap();

	//Rellena la cueva aleatoriamente
	void RandomFill();

	//Función que asigna el valor de cada célula según sus vecinos vivos
	void CellRules();

	//Cuenta los vecinos vivos de la célula
	int CountNeighbours(int x, int y);

	//Visual
	//Función que spawnea las celdas en las posiciones adecuadas
	void FillMap();

	//Función que asigna los sprites según la vida de cada celda
	void FillCells();

	//Función para responder al input del jugador
	UFUNCTION()
		void InputKey();

};
