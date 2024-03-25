// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "../Settings/UI_Settings.h"
#include "ThreadManage.h"
#include "UI_Item.h"

void UUI_ItemPanel::NativeConstruct()
{
	Super::NativeConstruct();

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&]()
		{
			LayoutItem();
		});
}

void UUI_ItemPanel::LayoutItem()
{
	if (!ItemArray->GetChildrenCount())
	{
		//添加属性修改Ui
		if (ItemClass)
		{
			for (int32 MyColum = 0; MyColum < 2; MyColum++)
			{
				if (UUI_Item * ItemWidget = CreateWidget<UUI_Item>(GetWorld(), ItemClass))
				{
					if (UUniformGridSlot * GridSlot = ItemArray->AddChildToUniformGrid(ItemWidget))
					{
						GridSlot->SetColumn(MyColum);
					}

					if (MyColum)
					{
						ItemWidget->SetModifyType(EElementModifyType::MODIFY_LOCATION);
					}
					else
					{
						ItemWidget->SetModifyType(EElementModifyType::MODIFY_ROTATION);
					}
				}
			}
		}

		//添加设置Ui
		if (UUI_Settings * ItemWidget = CreateWidget<UUI_Settings>(GetWorld(), SettingsClass))
		{
			if (UUniformGridSlot * GridSlot = ItemArray->AddChildToUniformGrid(ItemWidget))
			{
				GridSlot->SetColumn(3);
			}
		}
	}
}
