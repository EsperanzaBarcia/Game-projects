// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Añadimos los componentes necesarios
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "Semaphore.generated.h"
UCLASS()
class SEMAFOROS_API ASemaphore : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASemaphore();

	//COMPONENTES
	UPROPERTY(EditAnywhere, Category = "Components")
		USphereComponent* collider;

	UPROPERTY(EditAnywhere, Category = "Components")
	float minTime;

	UPROPERTY(EditAnywhere, Category = "Components")
	float maxTime;

	//booleano que se activa cuando alguien cruza el semáforo
	bool someonePassing = false;

	//Radio que queremos que tenga el collider
	const int COLLIDER_RADIUS = 400;


private:
	//COMPONENTES
	//Luz
		UPointLightComponent* light;

	//Color de la luz
		FLinearColor currentColor;

	//PERMISOS
	//Array de displacers que quieren cruzar
		TArray<ADisplacer*> displacersList;

	//Posibles estados del semáforo
	enum SemaphoreState { Green, Red };
	SemaphoreState currentSemaphoreState;

	FTimerHandle timerID;

	bool disponible = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Cambia el color dependiendo del estado actual del semáforo
	UFUNCTION()
		void SetColor();

	//Función con la que se comunican los displacer para pedir permiso, los añade a la cola
	UFUNCTION()
		void AskSemaphore(class ADisplacer* other);

	//End Overlap Callbacks
	UFUNCTION()
		void OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void SetDisponibleTrue();
};
