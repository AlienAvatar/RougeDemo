// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Main/ButtonComponentWidget.h"
#include "Components/Button.h"
#include "RougeDemo/RougeDemo.h"

void UButtonComponentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Element->OnClicked.AddDynamic(this, &UButtonComponentWidget::OnElementButtonClicked);
}

void UButtonComponentWidget::OnElementButtonClicked()
{
	
	WIDGET_LOG(Warning, TEXT("OnQuitButtonClicked"));
}
