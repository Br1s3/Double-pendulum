CC = gcc


ifeq ($(OS), Windows_NT)
	PLATFORM := Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		PLATFORM := Linux
	else ifeq ($(UNAME_S), Darwin)
		PLATFORM := macos
	else
		PLATFORM := unknown
	endif
endif


ifeq ($(PLATFORM), Windows)
	MD = mkdir
	RM = del
	EXEC = .\main.exe
else ifeq ($(PLATFORM), Linux)
	MD = mkdir -p
	RM = rm -rf
	EXEC   = main
endif


BUILD_DIR 	:= project
PICTURE_DIR 	:= stock

PICTURE_DB_PENDULE := Double_pendule_%03d.ppm
VIDEO_DB_PENDULE := Double_pendule.mp4

FLAGS += 		\
-Wall -Wextra 		\
-Wno-unused-function 	\

RAY_FLAGS += 	\
-lraylib	\
-lGL		\
-lm		\
-lpthread	\
-ldl		\
-lrt		\
-lX11 		\



.PHONY: all clean_all help create_picture create_video play_video create_gif



all: $(BUILD_DIR) $(PICTURE_DIR) $(BUILD_DIR)/simple_pendule $(BUILD_DIR)/double_pendule_console $(BUILD_DIR)/double_pendule_video $(BUILD_DIR)/double_pendule


$(BUILD_DIR):
	$(MD) $(BUILD_DIR)

$(PICTURE_DIR):
	$(MD) $(PICTURE_DIR)

# $(BUILD_DIR)/simple_pendule $(BUILD_DIR)/double_pendule $(BUILD_DIR)/double_pendule_console $(BUILD_DIR)/double_pendule_video: | $(BUILD_DIR)
# $(BUILD_DIR)/double_pendule_video: | $(PICTURE_DIR)

$(BUILD_DIR)/simple_pendule: simple_pendule.c | $(BUILD_DIR)
	$(CC) simple_pendule.c -o $(BUILD_DIR)/simple_pendule $(FLAGS) $(RAY_FLAGS)

$(BUILD_DIR)/double_pendule: double_pendule.c | $(BUILD_DIR)
	$(CC) double_pendule.c EDOsolver.c -o $(BUILD_DIR)/double_pendule $(FLAGS) $(RAY_FLAGS)

$(BUILD_DIR)/double_pendule_console: double_pendule_console.c | $(BUILD_DIR)
	$(CC) double_pendule_console.c EDOsolver.c graph.c -o $(BUILD_DIR)/double_pendule_console $(FLAGS) -lm

$(BUILD_DIR)/double_pendule_video: double_pendule_video.c | $(BUILD_DIR) $(PICTURE_DIR)
	$(CC) double_pendule_video.c EDOsolver.c graph.c PPMfile.c -o $(BUILD_DIR)/double_pendule_video $(FLAGS) -lm



play_video: $(PICTURE_DIR)/Double_pendule.mp4
	mpv $(PICTURE_DIR)/Double_pendule.mp4 --loop-file=yes

create_gif: $(PICTURE_DIR)/Double_pendule.mp4
	ffmpeg -i $(PICTURE_DIR)/Double_pendule.mp4 -vf "fps=15,scale=500:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" $(PICTURE_DIR)/Double_pendule.gif

create_video: $(PICTURE_DIR)/Double_pendule.mp4

$(PICTURE_DIR)/Double_pendule.mp4: $(PICTURE_DIR)/Double_pendule_000.ppm
	ffmpeg -i $(PICTURE_DIR)/Double_pendule_%03d.ppm -r 60 $(PICTURE_DIR)/Double_pendule.mp4

$(PICTURE_DIR)/Double_pendule_000.ppm: $(BUILD_DIR)/double_pendule_video
	./$(BUILD_DIR)/double_pendule_video

clean_all:
	$(RM) $(PICTURE_DIR)
	$(RM) $(BUILD_DIR)
	$(RM) *.csv

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean_all"
	@echo "... create_video"
	@echo "... play_video"
	@echo "... create_gif"
	@echo "... help"
