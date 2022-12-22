// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class CAVEAUTOMATA_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	//sprite
	UPROPERTY(EditAnywhere, Category = "Sprite Component")
		class UPaperSpriteComponent* Sprite;
	//posición
	UPROPERTY(EditAnywhere, Category = "Cell Info")
		FVector2D info;

	// Constructor por defecto
	ACell();

};
