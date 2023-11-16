#include "LoadingScreen.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SThrobber.h"


struct FRPGLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	//构造函数
	FRPGLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		//设置笔刷资源
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	//GC
	//FReferenceCollector 垃圾回收收集对象
	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		//Gets the UObject that represents the brush resource, if any.
		// 获取UObject中的笔刷资源
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			//把当前笔刷资源添加进对象中
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

class RougeDemoLoadingScreen : public SCompoundWidget
{
	public:
		//添加支持
		SLATE_BEGIN_ARGS(RougeDemoLoadingScreen) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
			static const FName LoadingScreenName(TEXT("/Game/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

			//初始化图像Brush
			LoadingScreenBrush = MakeShareable(new FRPGLoadingScreenBrush(LoadingScreenName, FVector2D(1024, 256)));

			FSlateBrush *BGBrush = new FSlateBrush();
			BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);

			//图像布局
			ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)	
					.BorderImage(BGBrush)
				]
				+SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SImage)
					.Image(LoadingScreenBrush.Get())
				]
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.VAlign(VAlign_Bottom)
					.HAlign(HAlign_Right)
					.Padding(FMargin(10.0f))
					[
						SNew(SThrobber)
						.Visibility(this, &RougeDemoLoadingScreen::GetLoadIndicatorVisibility)
					]
				]
			];
		}
	
	private:
		/** Rather to show the ... indicator */
		EVisibility GetLoadIndicatorVisibility() const
		{
			bool Vis =  GetMoviePlayer()->IsLoadingFinished();
			return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
		}
		
		/** Loading screen image brush */
		TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FRougeDemoLoadingScreenModule :  public IRougeDemoLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// Force load for cooker reference
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo") );

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(RougeDemoLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	
	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreen.WidgetLoadingScreen = SNew(RougeDemoLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};
IMPLEMENT_GAME_MODULE(FRougeDemoLoadingScreenModule, LoadingScreen);
