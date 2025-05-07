#!/bin/make
.SILENT:

sinclude makefile.user
VERSION := 1.2.0

CXX := $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ --target=aarch64-linux-android26
CXXFLAGS := -std=c++20 -fPIC -fvisibility=hidden -Wall -Wno-dollar-in-identifier-extension -Wno-zero-length-array -Wno-gnu-statement-expression \
	-Wno-format-pedantic -Wno-vla-extension -Wno-unused-function -Werror -pedantic-errors -isystem extern/includes/libil2cpp/il2cpp/libil2cpp \
	-isystem extern/includes -isystem extern/includes/fmt/fmt/include -isystem extern/includes/libil2cpp/il2cpp/external/baselib/Include \
	-isystem extern/includes/libil2cpp/il2cpp/external/baselib/Platforms/Android/Include -DVERSION=\"$(VERSION)\" -DUNITY_2021 -DFMT_HEADER_ONLY
LDFLAGS = -static-libstdc++ -shared -Wl,--no-undefined,--gc-sections,--fatal-warnings -Lextern/libs -l:$(notdir $(wildcard extern/libs/libbeatsaber-hook*.so)) -lpaper2_scotland2 -lsl2 -llog
ifdef NDK
OBJDIR := .obj/$(shell $(CXX) -dumpmachine)
else
OBJDIR := .obj/unknown
ndk:
	$(error Android NDK path not set)
endif
FILES := main.cpp extern/includes/beatsaber-hook/shared/inline-hook/And64InlineHook.cpp
OBJS := $(FILES:%=$(OBJDIR)/%.o)

libFunnySpinnyButtonEnabler.so: $(OBJS) | ndk
	@echo "[cxx $@]"
	$(CXX) $(LDFLAGS) $(OBJS) -o "$@"

$(OBJDIR)/%.cpp.o: %.cpp extern makefile | ndk
	@echo "[cxx $(notdir $@)]"
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c "$<" -o "$@" -MMD -MP

.obj/mod.json: extern makefile
	@echo "[printf $(notdir $@)]"
	@mkdir -p "$(@D)"
	printf "{\n\
		\"\$$schema\": \"https://raw.githubusercontent.com/Lauriethefish/QuestPatcher.QMod/main/QuestPatcher.QMod/Resources/qmod.schema.json\",\n\
		\"_QPVersion\": \"1.2.0\",\n\
		\"modloader\": \"Scotland2\",\n\
		\"name\": \"FunnySpinnyButtonEnabler\",\n\
		\"id\": \"FunnySpinnyButtonEnabler\",\n\
		\"author\": \"rcelyte\",\n\
		\"version\": \"$(VERSION)\",\n\
		\"packageId\": \"com.beatgames.beatsaber\",\n\
		\"packageVersion\": \"1.40.4_5283\",\n\
		\"description\": \"Enables 360 and 90 maps in multiplayer\",\n\
		\"coverImage\": \"cover.png\",\n\
		\"dependencies\": [\n\
			{\n\
				\"version\": \"^4.6.2\",\n\
				\"id\": \"paper2_scotland2\",\n\
				\"downloadIfMissing\": \"https://github.com/Fernthedev/paperlog/releases/download/v4.6.2/paper2_scotland2.qmod\"\n\
			}\n\
		],\n\
		\"lateModFiles\": [\"libFunnySpinnyButtonEnabler.so\"],\n\
		\"libraryFiles\": [\"$(notdir $(wildcard extern/libs/libbeatsaber-hook*.so))\"],\n\
		\"fileCopies\": [],\n\
		\"copyExtensions\": []\n\
	}" > .obj/mod.json

FunnySpinnyButtonEnabler.qmod: libFunnySpinnyButtonEnabler.so .obj/mod.json
	@echo "[zip $@]"
	zip -j "$@" cover.png extern/libs/libbeatsaber-hook*.so libFunnySpinnyButtonEnabler.so .obj/mod.json

run: FunnySpinnyButtonEnabler.qmod

extern:
	@echo "[qpm restore]"
	qpm-rust restore

clean:
	@echo "[cleaning]"
	rm -rf .obj/ extern/ include/ FunnySpinnyButtonEnabler.qmod libFunnySpinnyButtonEnabler.so
	qpm-rust clear || true

.PHONY: clean ndk run

sinclude $(FILES:%=$(OBJDIR)/%.d)
