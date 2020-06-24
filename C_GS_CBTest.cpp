// Fill out your copyright notice in the Description page of Project Settings.


#include "CrashBandicootTest/C_GS_CBTest.h"

AC_GS_CBTest::AC_GS_CBTest()
	: intPlayerScore(20)
{}

void AC_GS_CBTest::BeginPlay() 
{
	Super::BeginPlay();
	bGameOver = false;
}