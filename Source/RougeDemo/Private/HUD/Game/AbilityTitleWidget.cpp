// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Game/AbilityTitleWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UAbilityTitleWidget::SetUp(int32 Level, UTexture2D* Texture)
{
	TB_Level->SetText(UKismetTextLibrary::Conv_IntToText(Level));
	Img_Icon->SetBrushFromTexture(Texture);
}
