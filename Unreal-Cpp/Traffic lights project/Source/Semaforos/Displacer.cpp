// Fill out your copyright notice in the Description page of Project Settings.

#include "Displacer.h"
#include "Engine.h"
#include "Semaphore.h"

// Sets default values
ADisplacer::ADisplacer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creamos la malla
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");//Devuelve un puntero

	//Asignamos la malla como componente raíz, por la jerarquía
	SetRootComponent(mesh);

	//Creamos el collider
	collider = CreateDefaultSubobject<USphereComponent>("Collider");

	//Unimos el collider a la raíz, por la jerarquía
	collider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Cambiamos el radio del collider según queramos
	collider->SetSphereRadius(COLLIDER_RADIUS);



}

// Called when the game starts or when spawned
void ADisplacer::BeginPlay()
{
	Super::BeginPlay();

	originalSpeed = speed;

	//Suscribimos al collider a los eventos overlap
	if (collider != nullptr)
	{
		collider->OnComponentBeginOverlap.AddDynamic(this, &ADisplacer::OnTriggerEnter);
	}

	//Si estamos en modo ruta seguirá la ruta calculando distancias
	if (followPath)
	{
		//calculamos la distancia entre el actor y el destino actual
		distance = points[currentIndexPoint]->GetActorLocation() - GetActorLocation();
		distance.GetSafeNormal();

	}



}

// Called every frame
void ADisplacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ADisplacer::Move(DeltaTime);
}

//función que desplaza al actor
void ADisplacer::Move(float DeltaTime)
{
	if (followPath)
	{
		//Comprobamos que existe distancia y que el actor no está esperando en el semáforo, entonces le movemos
		if (!distance.IsNearlyZero() && speed != 0)
			SetActorLocation(GetActorLocation() + distance * speed * DeltaTime);
	}

	else
	{
		//Si no estamos en ruta el objeto simplemente se moverá hacia su eje forward
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);
	}

}


//Función que cambia el índice para que el actor se dirija al siguiente punto
void ADisplacer::UpdateIndex()
{
	if (currentIndexPoint + 1 < pathPointsLength)
	{
		currentIndexPoint++;
	}
	else
	{
		currentIndexPoint = 0;
	}

	//calculamos de nuevo la distancia
	distance = points[currentIndexPoint]->GetActorLocation() - GetActorLocation();
	distance.GetSafeNormal();
}

//Begin overlap callbacks
void ADisplacer::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//comprobamos si el actor está colisionando con un semáforo y si es así le pide permiso
	if (OtherActor && Cast<ASemaphore>(OtherActor))
	{
		speed = 0;
		Cast<ASemaphore>(OtherActor)->AskSemaphore(this);
	}

	//Si el actor colisiona con un límite de la escena cambia su dirección
	else if( !followPath && limits.Contains(OtherActor) )
	{
		ChangeDirection();
	}
}

//Función que cambia la dirección del displacer
void ADisplacer::ChangeDirection()
{
	speed *= -1;
	originalSpeed = speed;
}


