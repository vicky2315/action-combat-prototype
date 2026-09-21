# Soulslike Action Combat System

A melee combat prototype in **Unreal Engine 5.6 (C++)** using Paragon assets — hit detection, combo
and charged attacks, and enemy AI that spots the player, picks an attack based on distance, and reacts.

[![Watch the demo](https://img.youtube.com/vi/6WAuoxmsgwI/maxresdefault.jpg)](https://www.youtube.com/watch?v=6WAuoxmsgwI)

## Key Classes

### `UCombatComponent`
Runs attack state for a character — combos, charged heavy attacks, block, dodge. Fires a delegate every
time an attack lands, so other systems (stamina, animation, UI) can respond without depending on this
class directly.

```cpp
void ComboAttack();
void BeginCharge();
void ReleaseCharge(float DamageMultiplier);
void HeavyAttack(float DamageMultiplier);
void BlockAttack();
void DodgeAttack();

int ComboCounter;
FOnAttackPerformedSignature OnAttackPerformedDelegate;
```

### `UTraceComponent`
Does the actual hit detection. Traces a box from one weapon socket to another on every swing, keeps
track of who's already been hit this swing so nobody gets hit twice, and spawns VFX + camera shake on
impact.

```cpp
FName StartSocket;
FName EndSocket;
double BoxCollisionLength;

TArray<AActor*> ActorsDamaged;   // stops double hits per swing
UNiagaraSystem* HitNiagaraSystem;
TSubclassOf<UCameraShakeBase> HitCameraShake;
```

### `ULockOnComponent`
Handles lock-on targeting. Grabs a target within a radius, drops it automatically if the player moves
too far away, and tells other systems when the target changes.

```cpp
double BreakDistance;

void StartLockOn(float Radius);
void ToggleLockOn(float Radius);

FOnUpdatedTargetSignature OnUpdatedTargetDelegate;
```

### `AEnemyAIController`
Gives enemies sight — how far they can see, how far before they lose you, how wide their vision cone is
— and picks light or heavy attacks based on how far away the player is.

```cpp
float SightDistance;
float LoseSightDistance;
float PeripheralVisionDegrees;

void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
EAttackType ChooseAttack(float DistanceToPlayer, bool bCanLight, bool bCanHeavy);
```

### `IFighterInterface`
One shared contract for combat, so the same code path handles damage, weapon-collision toggling, and
combat state for both the player and enemies — no separate logic for each.

```cpp
float GetDamage();
void EnableSwordCollision();
void DisableSwordCollision();
ECombatState GetCombatState();
```

## Animation Notifies
The combat timing is driven straight off the animation montages using two `AnimNotifyState` windows,
so hit detection and dodge invincibility only run for the exact frames they're meant to — not the whole
animation.

- **`UTraceNotifyState`** sits on the swing frames of an attack montage. It flips
  `TraceComponent::bIsAttacking` on at `NotifyBegin` and off at `NotifyEnd`, so the weapon hit-trace only
  runs during the actual swing, not the wind-up or recovery.
- **`UDodgeNotifyState`** sits on the dodge montage. `NotifyBegin` sets combat state to `Dodge` (the
  invincibility window), and `NotifyEnd` calls `CombatComponent::HandleResetAttack()` to drop back to
  neutral once the dodge animation finishes.

```cpp
void UTraceNotifyState::NotifyBegin(...) { TraceRef->bIsAttacking = true; }
void UTraceNotifyState::NotifyEnd(...)   { TraceRef->bIsAttacking = false; }

void UDodgeNotifyState::NotifyBegin(...) { CombatComp->CharacterRef->CurrentState = ECombatState::Dodge; }
void UDodgeNotifyState::NotifyEnd(...)   { CombatComp->HandleResetAttack(); }
```

## Enemy AI
Behavior Trees and EQS (Environment Query System) handle positioning and higher-level decisions, built
on top of the perception and attack-selection logic above.

## Architecture
- Gameplay Tags track combat state
- Data Assets hold stat configuration (`EStats`)
- Debug overlay for inspecting combat state live

## Tech
Unreal Engine 5.6, C++, Paragon assets

## Author
Vignesh Suresh — [portfolio](https://vicky2315.github.io)
