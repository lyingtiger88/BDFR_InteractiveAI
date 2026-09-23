#include "Health/BDFRHealthComponent.h"

#include "GameFramework/Actor.h"

UBDFRHealthComponent::UBDFRHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);
    RecalculateHealthState(nullptr, 0.0f);
}

void UBDFRHealthComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bBleeding || BleedRatePerSecond <= 0.0f || HealthState == EBDFRHealthState::Dead)
    {
        return;
    }

    SetHealth(Health - BleedRatePerSecond * DeltaTime, LastDamageCauser.Get());
}

void UBDFRHealthComponent::ApplyHealthDamage(const float DamageAmount, AActor* DamageCauser)
{
    if (DamageAmount <= 0.0f || HealthState == EBDFRHealthState::Dead)
    {
        return;
    }

    LastDamageCauser = DamageCauser;
    const float PreviousHealth = Health;
    SetHealth(Health - DamageAmount, DamageCauser);

    if (Health < PreviousHealth && HealthState != EBDFRHealthState::Dead)
    {
        OnWounded.Broadcast(DamageCauser, DamageAmount, HealthState);
    }
}

void UBDFRHealthComponent::RestoreHealth(const float Amount)
{
    if (Amount <= 0.0f || HealthState == EBDFRHealthState::Dead)
    {
        return;
    }

    SetHealth(Health + Amount, nullptr);
}

void UBDFRHealthComponent::StartBleeding(const float InBleedRatePerSecond)
{
    if (HealthState == EBDFRHealthState::Dead)
    {
        return;
    }

    BleedRatePerSecond = FMath::Max(0.0f, InBleedRatePerSecond);
    const bool bWasBleeding = bBleeding;
    bBleeding = BleedRatePerSecond > 0.0f;

    if (bWasBleeding != bBleeding)
    {
        OnBleedingChanged.Broadcast(bBleeding);
    }
}

void UBDFRHealthComponent::StopBleeding()
{
    const bool bWasBleeding = bBleeding;
    bBleeding = false;
    BleedRatePerSecond = 0.0f;

    if (bWasBleeding)
    {
        OnBleedingChanged.Broadcast(false);
    }
}

void UBDFRHealthComponent::Stabilize(const float RestoredHealth)
{
    StopBleeding();

    if (RestoredHealth > 0.0f)
    {
        RestoreHealth(RestoredHealth);
    }
}

FBDFRHealthSnapshot UBDFRHealthComponent::GetHealthSnapshot() const
{
    FBDFRHealthSnapshot Snapshot;
    Snapshot.Health = Health;
    Snapshot.MaxHealth = MaxHealth;
    Snapshot.HealthState = HealthState;
    Snapshot.bBleeding = bBleeding;
    Snapshot.BleedRatePerSecond = BleedRatePerSecond;
    Snapshot.bCanMove = HealthState != EBDFRHealthState::Incapacitated && HealthState != EBDFRHealthState::Dead;
    Snapshot.bCanFight = HealthState != EBDFRHealthState::Incapacitated && HealthState != EBDFRHealthState::Dead;
    return Snapshot;
}

float UBDFRHealthComponent::GetHealthNormalized() const
{
    return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;
}

void UBDFRHealthComponent::SetHealth(const float NewHealth, AActor* DamageCauser)
{
    const float PreviousHealth = Health;
    const EBDFRHealthState PreviousState = HealthState;

    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    RecalculateHealthState(DamageCauser, FMath::Max(0.0f, PreviousHealth - Health));

    if (!FMath::IsNearlyEqual(PreviousHealth, Health))
    {
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    if (PreviousState != HealthState)
    {
        OnHealthStateChanged.Broadcast(PreviousState, HealthState);

        if (HealthState == EBDFRHealthState::Dead)
        {
            StopBleeding();
            OnDeath.Broadcast(DamageCauser);
        }
    }
}

void UBDFRHealthComponent::RecalculateHealthState(AActor* DamageCauser, const float DamageAmount)
{
    const float Normalized = GetHealthNormalized();

    if (Health <= 0.0f)
    {
        HealthState = EBDFRHealthState::Dead;
    }
    else if (Normalized <= IncapacitatedThreshold)
    {
        HealthState = EBDFRHealthState::Incapacitated;
    }
    else if (Normalized <= CriticalThreshold)
    {
        HealthState = EBDFRHealthState::Critical;
    }
    else if (Normalized <= WoundedThreshold)
    {
        HealthState = EBDFRHealthState::Wounded;
    }
    else
    {
        HealthState = EBDFRHealthState::Healthy;
    }
}
