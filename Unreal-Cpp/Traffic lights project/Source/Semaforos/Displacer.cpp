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

	//Asignamos la malla como componente ra�z, por la jerarqu�a
	SetRootComponent(mesh);

	//Creamos el collider
	collider = CreateDefaultSubobject<USphereComponent>("Collider");

	//Unimos el collider a la ra�z, por la jerarqu�a
	collider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Cambiamos el radio del collider seg�n queramos
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

	//Si estamos en modo ruta seguir� la ruta calculando distancias
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

//funci�n que desplaza al actor
void ADisplacer::Move(float DeltaTime)
{
	if (followPath)
	{
		//Comprobamos que existe distancia y que el actor no est� esperando en el sem�foro, entonces le movemos
		if (!distance.IsNearlyZero() && speed != 0)
			SetActorLocation(GetActorLocation() + distance * speed * DeltaTime);
	}

	else
	{
		//Si no estamos en ruta el objeto simplemente se mover� hacia su eje forward
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);
	}

}


//Funci�n que cambia el �ndice para que el actor se dirija al siguiente punto
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
	//comprobamos si el actor est� colisionando con un sem�foro y si es as� le pide permiso
	if (OtherActor && Cast<ASemaphore>(OtherActor))
	{
		speed = 0;
		Cast<ASemaphore>(OtherActor)->AskSemaphore(this);
	}

	//Si el actor colisiona con un l�mite de la escena cambia su direcci�n
	else if( !followPath && limits.Contains(OtherActor) )
	{
		ChangeDirection();
	}
}

//Funci�n que cambia la direcci�n del displacer
void ADisplacer::ChangeDirection()
{
	speed *= -1;
	originalSpeed = speed;
}


