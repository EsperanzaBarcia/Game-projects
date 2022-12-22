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

	//Prefab de c�lula
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

	//Iteraciones que se har�n para suavizar el mapa
	UPROPERTY(Category = Generator, EditAnywhere, BlueprintReadWrite)
		int iterations;

private:

	//array que guarda el mapa
	TArray<int> map;

	//enumeraci�n para saber en qu� estado esta la caverna para evitar que se colapse
	enum CellState { Idle, Process };
	CellState currentState;

	//Si hay menos vecinos que este n�mero la c�lula morir�
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

	//L�gica
	//Funci�n que asigna los datos del mapa
	void SetUpMap();

	//Funci�n que genera el mapa
	void GenerateMap();

	//Rellena la cueva aleatoriamente
	void RandomFill();

	//Funci�n que asigna el valor de cada c�lula seg�n sus vecinos vivos
	void CellRules();

	//Cuenta los vecinos vivos de la c�lula
	int CountNeighbours(int x, int y);

	//Visual
	//Funci�n que spawnea las celdas en las posiciones adecuadas
	void FillMap();

	//Funci�n que asigna los sprites seg�n la vida de cada celda
	void FillCells();

	//Funci�n para responder al input del jugador
	UFUNCTION()
		void InputKey();

};
