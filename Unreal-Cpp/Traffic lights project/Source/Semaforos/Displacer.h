// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Añadimos los componentes necesarios
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Displacer.generated.h"

UCLASS()
class SEMAFOROS_API ADisplacer : public AActor
{
	GENERATED_BODY()

public:

	//PROPIEDADES
	//Velocidad actual de desplazamiento
	float originalSpeed;

	//velocidad inicial asignada por el editor
	UPROPERTY(EditAnywhere, Category = "Displacer properties")
		float speed = 0.3f;

	//COMPONENTES
	//Malla del actor
	UPROPERTY(EditAnywhere, Category = "Displacer Components")
		class UStaticMeshComponent* mesh;

	//Collider del actor, en este caso es una esfera
	UPROPERTY(EditAnywhere, Category = "Displacer Components")
		class USphereComponent* collider;

	//Radio que queremos que tenga el collider
	const int COLLIDER_RADIUS = 50;

	//Triggers que delimitan la escena
	UPROPERTY(EditAnywhere, Category = "Displacer Components")
		 TArray<AActor*>limits;

	//PATRULLA
	UPROPERTY(EditAnywhere, Category = "Patrol components")
	bool followPath;

	//Puntos de destino entre los que se moverá el actor
	UPROPERTY(EditAnywhere, Category = "Patrol components")
		class AActor* points[2];

	//índice de punto de patrulla
	int currentIndexPoint = 0;

	//distancia entre el punto de destino y el actor para calcular su desplazamiento
	FVector distance;

	//número de puntos de patrulla
	int pathPointsLength = 2;

	// Sets default values for this actor's properties
	ADisplacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//función que desplaza al actor
	UFUNCTION()
		void Move(float DeltaTime);

	//Function que cambia el índice para que el actor se dirija al siguiente punto
	UFUNCTION()
		void UpdateIndex();

	//BeginOverlap callbacks
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Función para cambiar la dirección
	UFUNCTION()
		void ChangeDirection();
};

