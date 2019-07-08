// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "DebugPrinter.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "PlayerCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Classes/AIController.h"
#include "GameFramework/Controller.h"
#include "TwinStickGameMode.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCharacter::BeginPlay() {
	Super::BeginPlay();

	if (!DamageBox) {
		DebugPrinter::Print("Enemy damage collision box not set", EMessageType::Error);
	}
	else {
		DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBoxBeginOverlap);
		DamageBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);
	}
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerCharacter) {
		DebugPrinter::Print("Could not find target player", EMessageType::Error);
	}

}


void AEnemyCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor) == PlayerCharacter) {
		GetWorldTimerManager().SetTimer(
			DamageTimerHandle,
			this,
			&AEnemyCharacter::DealDamage,
			DamageRate,
			true,
			DamageDelay);
	}

}

void AEnemyCharacter::DealDamage()
{
	if (!ensure(DamageBox)) {
		return;
	}
	PlayerCharacter->TakeDamage(DamagePerHit);
}

void AEnemyCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor) == PlayerCharacter) {
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}


// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyCharacter::Die_Implementation() {
	Super::Die_Implementation();

	SetActorEnableCollision(false);
	GetController()->StopMovement();
	GetController()->Destroy();

	ATwinStickGameMode* GameMode = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode());
	if (ensure(GameMode)) {
		GameMode->IncrementScore(Score);
	}
}
