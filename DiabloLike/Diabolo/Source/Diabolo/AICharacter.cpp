// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include <string>


#include "AIControllerBase.h"
#include "HealthComponent.h"
#include "LevellingSystem.h"
#include "WeaponComponent.h"
#include "SkillManagerComponent.h"
#include "SkillBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    LifeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeWidget"));
    SkillManagerComponent = CreateDefaultSubobject<USkillManagerComponent>(TEXT("SkillManager"));

    if (LifeWidgetComponent)
    {
        LifeWidgetComponent->SetupAttachment(GetMesh());
        LifeWidgetComponent->RegisterComponent();
    }

    CurrentSkill = nullptr;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
    Super::BeginPlay();

    if (LifeWidgetComponent)
    {
        HealthWidget = Cast<UUserWidget>(LifeWidgetComponent->GetWidgetClass());
    }

    if (HealthComponent)
    {
        HealthComponent->OnDamageReceive.AddDynamic(this, &AAICharacter::ChasePlayerOnDamageReceive);
    }

    OnDeathCharacter.AddDynamic(this,  &AAICharacter::GiveExperienceToPlayer);
}

void AAICharacter::ChasePlayerOnDamageReceive()
{
    if (HealthComponent)
    {
        auto* playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

        if (playerController)
        {
            //Check AI On Damage Receive and make it chase the player
            auto* blackboardComp = UAIBlueprintHelperLibrary::GetBlackboard(this);
            auto* playerCharacter = Cast<ADiaboloCharacter>(playerController->GetPawn());

            if (blackboardComp)
            {
                blackboardComp->SetValueAsObject("TargetActor", playerCharacter);
            }
        }
    }
}


// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (state == Dead)
    {
        FadeActorDeath(DeltaTime);
    }

    //Check AI Perception to set the target Actor
    auto* blackboardComp = UAIBlueprintHelperLibrary::GetBlackboard(this);

    if (blackboardComp)
    {
        auto actor = blackboardComp->GetValueAsObject("TargetActor");

        if (actor
            && Cast<ADiaboloCharacter>(actor))
        {
            auto PlayerCharacter = Cast<ADiaboloCharacter>(actor);

            FVector location = PlayerCharacter->GetActorLocation();

            blackboardComp->SetValueAsVector("TargetLocation", location);
        }
    }
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::FadeActorDeath(float _DeltaTime)
{
    //Fade the actor on each tick/frame
    timerFadeOut += _DeltaTime;

    GetMesh()->SetMaterial(0, fadeOutMaterial);

    auto mat = GetMesh()->CreateDynamicMaterialInstance(0, fadeOutMaterial);

    if (remainingTimeFadeOut != 0)
    {
        float value = timerFadeOut * 0.75f;

        mat->SetTextureParameterValue("Base_Tex", textureCharacterDefault);
        mat->SetScalarParameterValue("fade", value);
    }

    //Destroy when done 
    if (timerFadeOut >= remainingTimeFadeOut)
    {
        Destroy();
    }
}

void AAICharacter::SetCurrentSkill(USkillBase* _skill)
{
    if (_skill)
        CurrentSkill = _skill;
}

USkillBase* AAICharacter::GetCurrentSkill()
{
    return CurrentSkill;
}

void AAICharacter::GiveExperienceToPlayer()
{
    auto controller = GetWorld()->GetFirstPlayerController();

    if (controller)
    {
        auto* character = Cast<ADiaboloCharacter>(controller->GetPawn());

        if (character && character->levelSystem)
        {
            character->levelSystem->AddExperience(experienceAmount);
        }
    }
}
