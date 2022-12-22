// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Añadimos los componentes necesarios
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScenePoint.generated.h"

UCLASS()
class SEMAFOROS_API AScenePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScenePoint();
	//COMPONENTES
	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//BeginOverlap callbacks
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
