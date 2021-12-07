// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectKapy/Gameplay/Abilities/AbilityBase.h"
#include "ProjectKapy/Gameplay/Combat/EffectReceiverComponent.h"
#include "ProjectKapy/Gameplay/Combat/LifeSystemComponent.h"
#include "ProjectKapy/Gameplay/Combat/StatusEffect.h"
#include "ProjectKapy/ProjectKapyCharacter.h"
#include "ProjectKapy/Gameplay/Abilities/ParryAbility.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	// SphereCollision->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetAutoActivate(false);
	// ProjectileMovement->SetUpdatedComponent(SphereCollision);

	ObjectQueryFilter.Add(
		(TEnumAsByte<EObjectTypeQuery>)UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectQueryFilter.
		Add((TEnumAsByte<EObjectTypeQuery>)UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectQueryFilter.Add(
		(TEnumAsByte<EObjectTypeQuery>)UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnAbilityActorBeginOverlap);
	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnAbilityActorHit);
}

void AProjectile::Init(UAbilityBase* Ability)
{
	Super::Init(Ability);
	if (Ability->GetCharacter())
	{
		ShooterType = Ability->GetCharacter()->CharacterType;
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTimeSinceSpawn += DeltaTime;
	DistanceTraveled += GetVelocity().Size() * DeltaTime;

	if (IsValid(this))
	{
		if (ElapsedTimeSinceSpawn > Properties.DeathTime || DistanceTraveled > Properties.DistanceMax)
		{
			if (Properties.bCanExplode)
			{
				ExplodeProjectile();
			}
			bMustBeDestroyed = true;
		}

		if (bMustBeDestroyed)
		{
			Destroy();
		}
	}
}

void AProjectile::OnAbilityActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	ULifeSystemComponent* LifeSystemComponent = OtherActor->FindComponentByClass<ULifeSystemComponent>();
	if (LifeSystemComponent == nullptr)
	{
		return;
	}
	
	if (LifeSystemComponent->IsDamageSensible(ShooterType) && !ActorsHit.Contains(OtherActor))
	{
		
		Super::OnAbilityActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
                                  SweepResult);
		bool isBlock = false;

		AProjectKapyCharacter* KapyChar = Cast<AProjectKapyCharacter>(OtherActor);
		if (KapyChar != nullptr)
		{
			if (KapyChar->AbilityManager->GetAbilityExecuting() != nullptr)
			{
				UParryAbility* ParryAbility = Cast<UParryAbility>(KapyChar->AbilityManager->GetAbilityExecuting());
				if (ParryAbility)
				{
					FParryResult ParryResult;
					isBlock = ParryAbility->BlockProjectile(ParryResult, this, OverlappedComponent);
				}
			}
		}

		if (!isBlock)
		{
			ApplyProjectileDamageToActor(OtherActor, Properties.DamageDirectHit, ShooterType);
			if (Properties.bCanExplode)
			{
				ExplodeProjectile(OtherActor);
			}

			OnProjectileHit(OtherActor);
		}
		ActorsHit.Add(OtherActor);
		bMustBeDestroyed = true;
	}
}

void AProjectile::OnAbilityActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(OtherActor))
	{
		return;
	}
	Super::OnAbilityActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if (!(AbilityOwner && OtherActor == AbilityOwner->GetCharacter()))
	{

		ULifeSystemComponent* LifeSystemComponent = OtherActor->FindComponentByClass<ULifeSystemComponent>();
		if (LifeSystemComponent != nullptr && !ActorsHit.Contains(OtherActor))
		{
			ApplyProjectileDamageToActor(OtherActor, Properties.DamageDirectHit, ShooterType);
			ActorsHit.Add(OtherActor);
		}
		if (Properties.bCanExplode)
		{
			ExplodeProjectile(OtherActor);
		}
		OnProjectileHit(OtherActor);
		bMustBeDestroyed = true;
	}
}

void AProjectile::SetupProjectile(FVector Direction)
{
	SetActorRotation(Direction.GetSafeNormal().Rotation());
	ProjectileMovement->SetVelocityInLocalSpace(
		FVector::ForwardVector * Properties.LaunchSpeed);
	if (Properties.bUseGravity)
	{
		ProjectileMovement->ProjectileGravityScale = Properties.GravityOverride;
	}
	else
	{
		ProjectileMovement->ProjectileGravityScale = 0.f;
	}


	ProjectileMovement->Activate();
}

void AProjectile::OverrideProperties(FProjectileProperties NewProperties)
{
	Properties = NewProperties;
}


void AProjectile::ApplyProjectileDamageToActor(AActor* ActorHit, float Damage, ECharacterType _type)
{
	if (ActorHit == nullptr)
		return;

	ULifeSystemComponent* LifeSystem = ActorHit->FindComponentByClass<ULifeSystemComponent>();
	if (LifeSystem == nullptr)
		return;
	LifeSystem->ApplyDamageAtPos(Damage, GetActorLocation(), this, _type, DT_Distance);

	UEffectReceiverComponent* EffectReceiver = ActorHit->FindComponentByClass<UEffectReceiverComponent>();
	if (EffectReceiver == nullptr)
		return;

	for (TSubclassOf<AStatusEffect> Effect : Properties.EffectsToApply)
	{
		if (Effect != nullptr)
		{
			EffectReceiver->AddEffectByClass(Effect);
		}
	}
}


void AProjectile::ExplodeProjectile(AActor* ActorHit)
{
	if (bHaveExplosed)
	{
		return;
	}
	
	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(this);
	if (ActorHit != nullptr)
		actorsToIgnore.Add(ActorHit);


	if (AbilityOwner && !Properties.bCanHitShooter)
	{
		actorsToIgnore.Add(AbilityOwner->GetCharacter());
	}

	TArray<AActor*> ActorsHitByExplosion;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Properties.RadiusExplosion,
	                                          ObjectQueryFilter, nullptr,
	                                          actorsToIgnore, ActorsHitByExplosion);

	for (int32 i = 0; i < ActorsHitByExplosion.Num(); i++)
	{
		AActor* actorInExplo = ActorsHitByExplosion[i];
		float damage = Properties.DamageExplosion;
		if (Properties.bDamageExplosionIsScaled)
		{
			FVector vecToActor = actorInExplo->GetActorLocation() - GetActorLocation();
			damage = Properties.DamageExplosion * (FVector::DistSquared(
					GetActorLocation(), actorInExplo->GetActorLocation()) /
				(Properties.RadiusExplosion * Properties.RadiusExplosion));
		}

		if (Properties.bCanHitShooter)
		{
			ApplyProjectileDamageToActor(actorInExplo, damage, CT_None);
		}
		else
		{
			ApplyProjectileDamageToActor(actorInExplo, damage, ShooterType);
		}
	}
	BP_OnProjectileExplode(ActorHit);
	bHaveExplosed = true;
}


void AProjectile::OnProjectileHit(AActor* ActorHit)
{
	BP_OnProjectileHit(ActorHit);
}
