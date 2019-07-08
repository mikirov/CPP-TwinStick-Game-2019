// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DebugPrinter.h"
#include "EnemyCharacter.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!CapsuleComponent) {
		DebugPrinter::Print("Projectile capsule component not set", EMessageType::Warning);
	}
	else {
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::AProjectile::OnOverlapBegin);
	}
	if (!HitParticleSystem) {
		DebugPrinter::Print("Hit particle system not set", EMessageType::Warning);
	}
	
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetInstigator() == GetInstigator()) {
		return;
	}
	//DebugPrinter::Print("projectile overlapping");
	AEnemyCharacter* EnemyCharacter = dynamic_cast<AEnemyCharacter*>(OtherActor);
	if (EnemyCharacter) {
		EnemyCharacter->TakeDamage(Damage);	
	}
	if (ensure(HitParticleSystem)) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticleSystem,
			OtherActor->GetActorTransform()
		);
	}

	Destroy();
}

