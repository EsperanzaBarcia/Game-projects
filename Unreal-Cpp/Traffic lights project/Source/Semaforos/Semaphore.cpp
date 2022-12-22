// Fill out your copyright notice in the Description page of Project Settings.


#include "Semaphore.h"
#include "Engine.h"
#include "Displacer.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASemaphore::ASemaphore()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Asignamos el componente de luz 
	light = CreateDefaultSubobject<UPointLightComponent>("Light");

	//Asignamos el componente de luz como componente ra�z, por la jerarqu�a
	SetRootComponent(light);

	//Creamos el collider
	collider = CreateDefaultSubobject<USphereComponent>("SphereCollider");

	//Unimos el collider a la ra�z, por la jerarqu�a
	collider->AttachToComponent(light, FAttachmentTransformRules::KeepRelativeTransform);

	//Cambiamos el radio del collider seg�n queramos
	collider->SetSphereRadius(COLLIDER_RADIUS);

}

// Called when the game starts or when spawned
void ASemaphore::BeginPlay()
{
	Super::BeginPlay();

	//Asignamos que el sem�foro en un inicio est� en verde
	currentSemaphoreState = SemaphoreState::Green;
	//Llamamos a la funci�n que cambia el color del sem�foro seg�n este estado
	SetColor();


	//Suscribimos al collider a los eventos overlap
	if (collider != nullptr)
	{
		collider->OnComponentEndOverlap.AddDynamic(this, &ASemaphore::OnTriggerEnd);
	}

	
}

// Called every frame
void ASemaphore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Comprobamos si alguien est� cruzando
	if (!someonePassing)
	{
		//Puede estar en verde y no estar disponible

		//Si no hay nadie cruzando, el sem�foro estar� en verde
		currentSemaphoreState = SemaphoreState::Green;


		//y solo si est� disponible dejar� pasar a los displacer
		if (disponible)
		{
			//En caso de haber alguien en la cola le deja pasar y asigna el booleano a true
			if (displacersList.Num() > 0)
			{
				
				Cast<ADisplacer>(displacersList[0])->speed = Cast<ADisplacer>(displacersList[0])->originalSpeed;

				someonePassing = true;
			}
		}
	}

	//Si hay alguien cruzando el sem�foro ser� rojo
	else {
		currentSemaphoreState = SemaphoreState::Red;
		disponible = false;
	}

	//Cambiamos el color ya que hemos cambiado estados
	SetColor();
}

//Cambia el color dependiendo del estado actual del sem�foro
void ASemaphore::SetColor()
{
	if (currentSemaphoreState == SemaphoreState::Green)
	{
		currentColor = FLinearColor::Green;
	}
	else if (currentSemaphoreState == SemaphoreState::Red)
	{
		currentColor = FLinearColor::Red;
	}

	//Asignamos que la luz sea del color de currentColor
	light->SetLightColor(currentColor);

}



//Funci�n con la que se comunican los displacer para pedir permiso, los a�ade a la cola
void ASemaphore::AskSemaphore(ADisplacer* other)
{
	//comprueba si existe other y si la lista no lo contiene lo a�ade a ella
	if (other && !displacersList.Contains(other))
	{
		displacersList.Add(other);
	}

}

//End Overlap Callbacks
void ASemaphore::OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//hace un cast del actor colisionador a displacer y lo elimina de la lista porque ya ha cruzado
	if (OtherActor && Cast<ADisplacer>(OtherActor))
	{
		displacersList.Remove(Cast<ADisplacer>(OtherActor));

		//vuelve a permitir que se cruce
		someonePassing = false;

		//Al salir se guarda el mundo en una variable
		UWorld* world = GetWorld();

		if (world != nullptr)
		{
			//Y se espera un tiempo asignado en el editor para volver a poner el sem�foro disponible
			world->GetTimerManager().SetTimer(timerID, this, &ASemaphore::SetDisponibleTrue, FMath::RandRange(minTime, maxTime),true);
		}

	}

}

//Sets the semaphore disponible
void ASemaphore::SetDisponibleTrue()
{
	disponible = true;
}

