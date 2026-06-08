#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleWindZone.generated.h"

class USphereComponent;
class UArrowComponent;

UCLASS(Blueprintable)
class SIMPLEWIND_API ASimpleWindZone : public AActor
{
    GENERATED_BODY()

public:
    ASimpleWindZone();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simple Wind")
    USphereComponent* WindRadiusComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simple Wind")
    UArrowComponent* WindDirectionArrow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    float Strength = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    float GustStrength = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    float GustSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    float Radius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    bool bAffectPhysics = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Wind")
    FName AffectedActorTag = "WindAffected";

    UFUNCTION(BlueprintCallable, Category = "Simple Wind")
    FVector GetWindDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Simple Wind")
    float GetCurrentWindStrength() const;

private:
    float RunningTime = 0.0f;

    void ApplyWindToPhysicsActors();
};