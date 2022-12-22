// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenePoint.h"
#include "Displacer.h"

// Sets default values
AScenePoint::AScenePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creamos el collider
	collider = CreateDefaultSubobject<UBoxComponent>("Collider");
}

// Called when the game starts or when spawned
void AScenePoint::BeginPlay()
{
	Super::BeginPlay();

	//Suscribimos al collider a los eventos overlap
	if (collider != nullptr)
	collider->OnComponentBeginOverlap.AddDynamic(this, &AScenePoint::OnTriggerEnter);
	
}

// Called every frame
void AScenePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Begin overlap callbacks
void AScenePoint::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Comprueba si el otro elemento es un displacer y le cambia el índice
	if (OtherActor && Cast<ADisplacer>(OtherActor))
	{
		Cast<ADisplacer>(OtherActor)->UpdateIndex();
		
	}

}

