#include "SimpleWindZone.h"

#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ASimpleWindZone::ASimpleWindZone()
{
    PrimaryActorTick.bCanEverTick = true;

    WindRadiusComponent = CreateDefaultSubobject<USphereComponent>(TEXT("WindRadius"));
    RootComponent = WindRadiusComponent;

    WindRadiusComponent->SetSphereRadius(Radius);
    WindRadiusComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    WindDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WindDirection"));
    WindDirectionArrow->SetupAttachment(RootComponent);
    WindDirectionArrow->ArrowSize = 2.0f;
}

void ASimpleWindZone::BeginPlay()
{
    Super::BeginPlay();

    if (WindRadiusComponent)
    {
        WindRadiusComponent->SetSphereRadius(Radius);
    }
}

void ASimpleWindZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RunningTime += DeltaTime;

    if (WindRadiusComponent)
    {
        WindRadiusComponent->SetSphereRadius(Radius);
    }

    if (bAffectPhysics)
    {
        ApplyWindToPhysicsActors();
    }
}

FVector ASimpleWindZone::GetWindDirection() const
{
    if (WindDirectionArrow)
    {
        return WindDirectionArrow->GetForwardVector().GetSafeNormal();
    }

    return GetActorForwardVector().GetSafeNormal();
}

float ASimpleWindZone::GetCurrentWindStrength() const
{
    const float Gust = FMath::Sin(RunningTime * GustSpeed) * GustStrength;
    return Strength + Gust;
}

void ASimpleWindZone::ApplyWindToPhysicsActors()
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    TArray<AActor*> TaggedActors;
    UGameplayStatics::GetAllActorsWithTag(World, AffectedActorTag, TaggedActors);

    const FVector WindOrigin = GetActorLocation();
    const FVector WindDirection = GetWindDirection();
    const float CurrentStrength = GetCurrentWindStrength();

    for (AActor* Actor : TaggedActors)
    {
        if (!Actor)
        {
            continue;
        }

        const float Distance = FVector::Distance(WindOrigin, Actor->GetActorLocation());

        if (Distance > Radius)
        {
            continue;
        }

        const float DistanceAlpha = 1.0f - FMath::Clamp(Distance / Radius, 0.0f, 1.0f);
        const FVector Force = WindDirection * CurrentStrength * DistanceAlpha;

        TArray<UPrimitiveComponent*> PrimitiveComponents;
        Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

        for (UPrimitiveComponent* Primitive : PrimitiveComponents)
        {
            if (Primitive && Primitive->IsSimulatingPhysics())
            {
                Primitive->AddForce(Force * Primitive->GetMass());
            }
        }
    }
}