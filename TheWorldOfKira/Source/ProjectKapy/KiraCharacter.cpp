// Fill out your copyright notice in the Description page of Project Settings.


#include "KiraCharacter.h"

#include "CatCharacterMovement.h"
#include "EnumCustomMovement.h"
#include "Gameplay/Character/Component/QuestLogComponent.h"
#include "ProjectKapyCharacter.h"
#include "Camera/KapySpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/Abilities/AbilityManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKapy/Gameplay/Quest/QuestMacros.h"

AKiraCharacter::AKiraCharacter()
{
    QuestLog = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogSystem"));
    TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
}

void AKiraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AKiraCharacter::DoubleJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("JumpClimb", IE_Pressed, this, &AKiraCharacter::JumpWhileClimb);

    PlayerInputComponent->BindAction("Descent", IE_Pressed, this, &AKiraCharacter::Descent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AKiraCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AKiraCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AKiraCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AKiraCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AKiraCharacter::AttackPressed);
    PlayerInputComponent->BindAction("Attack", IE_Released, this, &AKiraCharacter::AttackRelease);

    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AKiraCharacter::BlockPressed);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &AKiraCharacter::BlockRelease);

    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AKiraCharacter::AimPressed);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &AKiraCharacter::AimRelease);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AKiraCharacter::FirePressed);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AKiraCharacter::FireRelease);

    PlayerInputComponent->BindAction("Spell", IE_Pressed, this, &AKiraCharacter::SpellPressed);
    PlayerInputComponent->BindAction("Spell", IE_Released, this, &AKiraCharacter::SpellRelease);

    // PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AKiraCharacter::Dash);
    PlayerInputComponent->BindAction("SwitchActiveQuest", IE_Released, this, &AKiraCharacter::SwitchQuest);
}

void AKiraCharacter::AimPressed(FKey key)
{
    SetCharacterState(ECharacterState::STATE_Shoot);
}

void AKiraCharacter::AimRelease(FKey key)
{
    if (CharacterState == STATE_Shoot)
    {
        SetCharacterState(ECharacterState::STATE_None);
    }
}

void AKiraCharacter::TurnAtRate(float Rate)
{
    float DeltaTime = GetWorld()->GetDeltaSeconds() * UGameplayStatics::GetGlobalTimeDilation(GetWorld());
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * DeltaTime);
}

void AKiraCharacter::LookUpAtRate(float Rate)
{
    float DeltaTime = GetWorld()->GetDeltaSeconds() * UGameplayStatics::GetGlobalTimeDilation(GetWorld());
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * DeltaTime);
}

void AKiraCharacter::MoveForward(float Value)
{
    if (CatCharacterMovement == nullptr)
        return;

    if ((Controller != NULL) && (Value != 0.0f))
    {
        //Assign climbing direction
        if (Value > 0.0f)
        {
            CatCharacterMovement->climbDirectionUp = GetActorUpVector();
        }
        else
        {
            CatCharacterMovement->climbDirectionUp = FVector::ZeroVector;
        }

        //Check the current direction and the movemement mode to give climbing velocity
        if (CatCharacterMovement->MovementMode == EMovementMode::MOVE_Custom
            && CatCharacterMovement->CustomMovementMode == EmovementModeCustom::CUSTOM_Climbing)
        {
            FRotator tempRotation = CatCharacterMovement->WayToLook.Rotation();

            SetActorRotation(tempRotation);

            const FVector Direction = FRotationMatrix(tempRotation).GetUnitAxis(EAxis::Z);

            if (Value < 0.0f)
            {
                CatCharacterMovement->bOrientRotationToMovement = false;
                AddMovementInput(Direction, Value);
            }
            else
            {
                CatCharacterMovement->bOrientRotationToMovement = false;
                AddMovementInput(Direction, Value);
            }
        }
        else //Move forward
        {
            // find out which way is forward
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            if (CatCharacterMovement
                && CatCharacterMovement->MovementMode != EMovementMode::MOVE_Custom)
            {
                FVector Direction = FVector::ZeroVector;

                CatCharacterMovement->bOrientRotationToMovement = true;
                Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

                // get forward vector
                AddMovementInput(Direction, Value);
            }
        }
    }
    else
    {
        CatCharacterMovement->climbDirectionUp = FVector::ZeroVector;
    }
}

void AKiraCharacter::MoveRight(float Value)
{
    if (CatCharacterMovement == nullptr)
        return;

    //Check Movement mode climb to allow movement against the wall 
    if (CatCharacterMovement->MovementMode == EMovementMode::MOVE_Custom
        && CatCharacterMovement->CustomMovementMode == EmovementModeCustom::CUSTOM_Climbing)
    {
        //Assign climbing direction
        if (Value > 0.0f)
        {
            CatCharacterMovement->climbDirectionRight = GetActorRightVector();
        }
        else if (Value < 0.0f)
        {
            CatCharacterMovement->climbDirectionRight = - GetActorRightVector();
        }
        else
        {
            CatCharacterMovement->climbDirectionRight = FVector::ZeroVector;
        }

        AddMovementInput(GetActorRightVector(), Value);
    }
    else if ((Controller != NULL) && (Value != 0.0f)
        && CatCharacterMovement->MovementMode != EMovementMode::MOVE_Custom) //MoveRight
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
    else
    {
        CatCharacterMovement->climbDirectionRight = FVector::ZeroVector;
    }
}

void AKiraCharacter::DoubleJump(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityPress("Jump");
    // if (CatCharacterMovement)
    //     CatCharacterMovement->DoubleJump();
}

void AKiraCharacter::JumpWhileClimb(FKey key)
{
    if (CatCharacterMovement)
        CatCharacterMovement->JumpWhileClimb();
}

void AKiraCharacter::Climb(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& hit)
{
    if (CatCharacterMovement)
        CatCharacterMovement->Climb(HitComponent, OtherActor, OtherComp, NormalImpulse, hit);
}

void AKiraCharacter::Descent(FKey key)
{
    if (CatCharacterMovement)
        CatCharacterMovement->Descent();
}

void AKiraCharacter::AttackPressed(FKey key)
{
    if (BP_CanInteract() && key.IsGamepadKey())
    {
        return;
    }

    if (AbilityManager)
        AbilityManager->RequestAbilityPress("Attack");
}

void AKiraCharacter::AttackRelease(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityRelease("Attack");
}

void AKiraCharacter::FirePressed(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityPress("Fire");
}

void AKiraCharacter::FireRelease(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityRelease("Fire");
}

void AKiraCharacter::BlockPressed(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityPress("Block");
}

void AKiraCharacter::BlockRelease(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityRelease("Block");
}

void AKiraCharacter::SpellPressed(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityPress("Spell");
}

void AKiraCharacter::SpellRelease(FKey key)
{
    if (AbilityManager)
        AbilityManager->RequestAbilityRelease("Spell");
}

void AKiraCharacter::AutoSelectQuest()
{
    if (QuestLog != nullptr)
    {
        if (QuestLog->ActiveQuest == nullptr
            && QuestLog->Quests.Num() > 0)
        {
            auto quests = QuestLog->Quests;

            for (auto* Element : quests)
            {
                if (!Element->bIsComplete)
                {
                    QuestLog->ActiveQuest = Element;
                    QuestLog->OnActiveQuestChanged(Element);
                    Element->UpdateUI_Event();
                    break;
                }
            }
        }
    }
}

void AKiraCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (GetCapsuleComponent() != nullptr)
    {
        if (CatCharacterMovement)
            CatCharacterMovement->capsuleComponent = GetCapsuleComponent();

        GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &AKiraCharacter::Climb);
    }

    LevelSystemComponent = FindComponentByClass<ULevelSystemComponent>();
}

void AKiraCharacter::Tick(float Deltatime)
{
    Super::Tick(Deltatime);

    AutoSelectQuest();
}

void AKiraCharacter::CheckCollectionObjective()
{
    if (QuestLog != nullptr
        && QuestLog->Quests.Num() > 0)
    {
        auto quests = QuestLog->Quests;

        for (auto* Element : quests)
        {
            auto Objectives = Element->Objectives;

            int index = 0;

            for (auto Objective : Objectives)
            {
                if (Element->CheckPreviousObjective(Objectives, index)
                    && Element->CheckSpecificObjective(Objectives, index, Objective.indexSpecificObjective))
                {
                    int goalNumber = Objective.Number;

                    if (Objective.Type == Collect && !Objective.bIsComplete)
                    {
                        if (ItemsCollected.Contains(Objective.IDItem))
                        {
                            if (ItemsCollected[Objective.IDItem] >= goalNumber)
                            {
                                Element->CustomObjectiveComplete(index);
                                Element->UpdateUI_Event();
                            }
                        }
                    }
                }

                index++;
            }
        }
    }
}

ULevelSystemComponent* AKiraCharacter::GetLevelSystemComp()
{
    return LevelSystemComponent;
}

TMap<int32, int32> AKiraCharacter::GetItemsCollected()
{
    return ItemsCollected;
}

void AKiraCharacter::AddItemToCollection(int32 _index, int32 _value)
{
    //This item does not exist yet
    if (!ItemsCollected.Contains(_index))
    {
        ItemsCollected.Add(_index, _value);
    }
    else
    {
        //Item exist
        if (ItemsCollected[_index] > 0)
        {
            //add items value
            ItemsCollected[_index] += _value;
        }
    }
}

//Change the active quest by taking the next one in the quest log
void AKiraCharacter::SwitchQuest()
{
    if (QuestLog != nullptr)
    {
        if (QuestLog->Quests.Num() <= 1)
            return;

        int size = QuestLog->Quests.Num();
        int currentQuestIndex = 0;

        for (int i = 0; i < size; i++)
        {
            if (QuestLog->Quests[i] == QuestLog->ActiveQuest)
            {
                currentQuestIndex = i;
                break;
            }
        }

        do
        {
            if (currentQuestIndex == size - 1)
            {
                currentQuestIndex = 0;
            }
            else
            {
                currentQuestIndex++;
            }
        }
        while (QuestLog->Quests[currentQuestIndex]->bIsComplete);

        const auto newQuest = QuestLog->Quests[currentQuestIndex];

        if (QuestLog->Quests[currentQuestIndex] != QuestLog->ActiveQuest)
            QuestLog->OnActiveQuestChanged(newQuest);
    }
}
