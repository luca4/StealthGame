// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackhole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
AFPSBlackhole::AFPSBlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh component"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("DestroySphere"));
	InnerSphereComponent->SetSphereRadius(200);
	
	InnerSphereComponent->SetupAttachment(MeshComp);
	
	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("AttractSphere"));
	OuterSphereComponent->SetSphereRadius(3000);
	OuterSphereComponent->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSBlackhole::BeginPlay()
{
	Super::BeginPlay();
	
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackhole::OverlapInnerSphere);
}

void AFPSBlackhole::OverlapInnerSphere(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphereComponent->GetOverlappingComponents(OverlappingComps);

	for (UPrimitiveComponent *c : OverlappingComps)
	{
		c->AddRadialForce(GetActorLocation(), OuterSphereComponent->GetScaledSphereRadius(), -2000, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

