#include <scotland2/shared/loader.hpp>
#include <beatsaber-hook/shared/inline-hook/And64InlineHook.hpp>
#include <beatsaber-hook/shared/utils/il2cpp-utils.hpp>
#include <beatsaber-hook/shared/utils/typedefs-string.hpp>

#define DL_EXPORT __attribute__((visibility("default")))

const Paper::ConstLoggerContext<25> logger = {"FunnySpinnyButtonEnabler"};

struct SongPackMask {
	uint64_t d0, d1;
};

static Il2CppString *Polyglot_Localization_Get(Il2CppString *key) {
	static const MethodInfo *method = THROW_UNLESS(il2cpp_utils::FindMethod("BGLib.Polyglot", "Localization", "Get", std::array<const Il2CppType*, 1>{
		&il2cpp_utils::GetClassFromName("System", "String")->byval_arg,
	}));
	Il2CppException* err = NULL;
	return (Il2CppString*)il2cpp_functions::runtime_invoke(method, nullptr, (void**)&key, &err);
}

static void (*base)(void *self, SongPackMask songPackMask, uint8_t allowedBeatmapDifficultyMask, Il2CppArray *notAllowedCharacteristics, bool hidePacksIfOneOrNone, bool hidePracticeButton, Il2CppString *actionButtonText, void* levelPackToBeSelectedAfterPresent, int startLevelCategory, void* beatmapLevelToBeSelectedAfterPresent, bool enableCustomLevels) = NULL;
static void LevelSelectionNavigationController_Setup(void *self, SongPackMask songPackMask, uint8_t allowedBeatmapDifficultyMask, Il2CppArray *notAllowedCharacteristics, bool hidePacksIfOneOrNone, bool hidePracticeButton, Il2CppString *actionButtonText, void* levelPackToBeSelectedAfterPresent, int startLevelCategory, void* beatmapLevelToBeSelectedAfterPresent, bool enableCustomLevels) {
	IL2CPP_CATCH_HANDLER(
		if(il2cpp_utils::detail::strcomp(actionButtonText, Polyglot_Localization_Get(il2cpp_utils::detail::alloc_str("BUTTON_SELECT")))) {
			Il2CppArray *empty = il2cpp_functions::array_new(il2cpp_utils::GetClassFromName("", "BeatmapCharacteristicSO"), 0);
			if(empty != NULL)
				notAllowedCharacteristics = empty;
		}
		base(self, songPackMask, allowedBeatmapDifficultyMask, notAllowedCharacteristics, hidePacksIfOneOrNone, hidePracticeButton, actionButtonText, levelPackToBeSelectedAfterPresent, startLevelCategory, beatmapLevelToBeSelectedAfterPresent, enableCustomLevels);
	)
}

extern "C" DL_EXPORT void setup(CModInfo *const modInfo) {
	*modInfo = {
		.id = "FunnySpinnyButtonEnabler",
		.version = VERSION,
		.version_long = 3,
	};
	logger.info("Completed setup");
}

extern "C" DL_EXPORT void late_load() {
	logger.info("loading @ %.*s", 2, &__TIME__[3]);
	il2cpp_functions::Init();
	const MethodInfo *baseInfo = il2cpp_utils::FindMethod("", "LevelSelectionNavigationController", "Setup", std::array<const Il2CppType*, 10>{
		&il2cpp_utils::GetClassFromName("", "SongPackMask")->byval_arg,
		&il2cpp_utils::GetClassFromName("", "BeatmapDifficultyMask")->byval_arg,
		&il2cpp_functions::array_class_get(::il2cpp_utils::GetClassFromName("", "BeatmapCharacteristicSO"), 1)->byval_arg,
		&il2cpp_utils::GetClassFromName("System", "Boolean")->byval_arg,
		&il2cpp_utils::GetClassFromName("System", "Boolean")->byval_arg,
		&il2cpp_utils::GetClassFromName("System", "String")->byval_arg,
		&il2cpp_utils::GetClassFromName("", "BeatmapLevelPack")->byval_arg,
		&il2cpp_utils::GetClassFromName("", "SelectLevelCategoryViewController/LevelCategory")->byval_arg,
		&il2cpp_utils::GetClassFromName("", "BeatmapLevel")->byval_arg,
		&il2cpp_utils::GetClassFromName("System", "Boolean")->byval_arg,
	});
	if(!baseInfo) {
		logger.critical("Attempting to install hook: LevelSelectionNavigationController_Setup, but method could not be found!");
		return;
	}
	void *addr = (void*)baseInfo->methodPointer;
	logger.info("Installing hook: LevelSelectionNavigationController_Setup to offset: %p", addr);
	A64HookFunction(addr, (void*)&LevelSelectionNavigationController_Setup, (void**)&base);
	logger.info("DONE LOADING");
}
