// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"
#include "SkillBase.h"

// Sets default values
ASkillActor::ASkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	detectionMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");

	if (detectionMesh != nullptr && RootComponent != nullptr)
		detectionMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(detectionMesh)
	{
		detectionMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASkillActor::TriggerBeginOverlap);
		detectionMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ASkillActor::TriggerEndOverlap);
	}
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::Init(USkillBase* _skill)
{
	if (_skill)
	{
		Skill = _skill;
	}
}

void ASkillActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Skill)
	{
		Skill->SkillBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void ASkillActor::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Skill)
	{
		Skill->SkillEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	}
}

