// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Creamos el spriteComponent
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");

	Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Cargamos el sprite desde su carpeta
	//Sprite genérico
	ConstructorHelpers::FObjectFinder<UObject> floorSprite(TEXT("PaperSprite'/Game/Sprites/Square_Sprite.Square_Sprite'"));

	//y lo asignamos al Sprite component
	Sprite->SetSprite((UPaperSprite*)floorSprite.Object);

	Tags.Add("cell");

}



