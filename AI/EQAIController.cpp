// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQAIController.h"
#include "Core/EQGameInstance.h"
#include "AI/EQDecision.h"
#include "TimerManager.h"
#include "Utility/EQFunctionBlueprintLibrary.h"
#include "Utility/EQGameplayBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "AI/EQBaseNPCCharacter.h"

const FName AEQAIController::FunctionPrefix = FName("Decision_");

AEQAIController::AEQAIController()
{
    WhiteBoard = CreateDefaultSubobject<UEQWhiteBoard>(TEXT("WhiteBoard"));
    CurrentNPCState = EEQNPCStateEnum::EQNPCS_Idle;
}

void AEQAIController::BeginPlay()
{
    Super::BeginPlay();

    const UEQGameInstance* EQGameInstance = Cast<UEQGameInstance>(GetGameInstance());
    check(EQGameInstance);
    
    DecisionSet = EQGameInstance->GetDecisionSetByTag(DefaultsDecisionSet);
    GetWorldTimerManager().SetTimer(TimeHandler_TickBrain, this, &AEQAIController::TickBrain, 1.0f, true);

    WhiteBoard->Instigator = this;
    WhiteBoard->LastStimulusTime = -1000.0f;

    ControlledNPC = Cast<AEQBaseNPCCharacter>(GetPawn());
}

void AEQAIController::TickBrain()
{
    check(DecisionSet);
    WhiteBoard->CurrentTime = GetWorld()->TimeSeconds;
    CurrentDecision = GetBestDecision();
    Excecute_Decision();
}

FUEQDecisionContext AEQAIController::GetBestDecision()
{
    FUEQDecisionContext BestDecisionContext = DecisionSet->DefaultDecision->CreateDecisionContext(this, nullptr, WhiteBoard);    
    float CutOff = 0.0f;

    TArray<UObject*> EmptyTargets{ nullptr };
    TArray<UObject*>* pTargets;

    for (auto& Decision : DecisionSet->Decisions)
    {
        pTargets = Decision->bHasTargetDependency
            ? &WhiteBoard->Targets
            : &EmptyTargets;

        for (auto& Target : *pTargets)
        {
            FUEQDecisionContext DecisionContext = Decision->CreateDecisionContext(this, Target, WhiteBoard);
            //TODO: Calculate the real bonus
            float Bonus = Decision->Weight;
            DecisionContext.Score = UEQDecision::CalculateScore(Decision, &DecisionContext, Bonus, CutOff);
            if (DecisionContext.Score > CutOff)
            {
                BestDecisionContext = DecisionContext;
                CutOff = DecisionContext.Score;
            }
        }
    }
    return BestDecisionContext;
}

void AEQAIController::SetNPCState(EEQNPCStateEnum InNPCState)
{
    if (CurrentNPCState == InNPCState)
    {
        return;
    }

    switch (CurrentNPCState)
    {
    case EEQNPCStateEnum::EQNPCS_Idle:
        break;
    case EEQNPCStateEnum::EQNPCS_Fire:
        ControlledNPC->StopFire();
        break;
    case EEQNPCStateEnum::EQNPCS_Move:
        StopMovement();
        break;
    default:
        break;
    }
    CurrentNPCState = InNPCState;
}

void AEQAIController::Excecute_Decision()
{
    const FString FunctionString = FunctionPrefix.ToString() + CurrentDecision.Decision->FunctionName.ToString();
    const FName FunctionName = FName(*FunctionString);
    UEQFunctionBlueprintLibrary::ExecuteFunctionByName<bool, bool>(FunctionName, this, nullptr);
}

void AEQAIController::Decision_MoveTo()
{
}

void AEQAIController::Decision_SaySomething()
{
    UE_LOG(LogTemp, Warning, TEXT("SAY SOMETHING"));
}

void AEQAIController::Decision_Idle()
{
    UE_LOG(LogTemp, Warning, TEXT("Idle"));

    // TODO: Cancel all previous decisions
    SetNPCState(EEQNPCStateEnum::EQNPCS_Idle);
}

void AEQAIController::Decision_Patrol()
{
    UE_LOG(LogTemp, Warning, TEXT("Patrol"));

    // TODO: Create real behavior
    SetNPCState(EEQNPCStateEnum::EQNPCS_Move);
    Decision_FreePatrol();
}

void AEQAIController::Decision_FreePatrol()
{
    UE_LOG(LogTemp, Warning, TEXT("FreePatrol"));

    // TODO: Cancel previous decision if needed
    SetNPCState(EEQNPCStateEnum::EQNPCS_Move);

    FVector Result;
    if (GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Idle) {
        if (UEQGameplayBlueprintLibrary::GetRandomPointInRadius(GetPawn()->GetActorLocation(), 2000, Result, GetWorld())) {
            MoveToLocation(Result);
        }
    }    
}

void AEQAIController::Decision_MoveTowardsEnemy()
{
    UE_LOG(LogTemp, Warning, TEXT("MoveTowardsEnemy"));

    // TODO: Cancel previous decision if needed
    SetNPCState(EEQNPCStateEnum::EQNPCS_Move);
        
    MoveToActor(Cast<AActor>(CurrentDecision.Target));
}

void AEQAIController::Decision_Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire"));
    
    AActor* TargetActor = Cast<AActor>(CurrentDecision.Target);
    SetNPCState(EEQNPCStateEnum::EQNPCS_Fire);
    ControlledNPC->Fire(TargetActor);
}

void AEQAIController::Decision_SearchEnemy()
{
    UE_LOG(LogTemp, Warning, TEXT("SearchEnemy"));

    // TODO: Create real behavior
    SetNPCState(EEQNPCStateEnum::EQNPCS_Move);

    Decision_FreePatrol();
}

void AEQAIController::Decision_MoveToLastKnownEnemyLocation()
{
    UE_LOG(LogTemp, Warning, TEXT("MoveToLastKnownEnemyLocation"));

    // TODO: Cancel previous decision if needed
    SetNPCState(EEQNPCStateEnum::EQNPCS_Move);

    MoveToLocation(CurrentDecision.WhiteBoard->LastKnownEnemyLocation);
}
