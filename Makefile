MD = mkdir -p
RM = rm -rf
CC = gcc

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


<<<<<<< HEAD
all: $(BUILD_DIR)/simple_pendule $(BUILD_DIR)/double_pendule $(BUILD_DIR)/double_pendule_console $(BUILD_DIR)/double_pendule_video
	@echo BUILDING: $?
=======
>>>>>>> 84ad484bb12c0e6ba06931869846593acc169db0

.PHONY: all clean_all help create_picture create_video play_video create_gif



$(BUILD_DIR):
	$(MD) $(BUILD_DIR)

$(PICTURE_DIR):
	$(MD) $(PICTURE_DIR)


all: $(BUILD_DIR) $(PICTURE_DIR) $(BUILD_DIR)/simple_pendule $(BUILD_DIR)/double_pendule_console $(BUILD_DIR)/double_pendule_video $(BUILD_DIR)/double_pendule

# $(BUILD_DIR)/simple_pendule $(BUILD_DIR)/double_pendule $(BUILD_DIR)/double_pendule_console $(BUILD_DIR)/double_pendule_video: | $(BUILD_DIR)
# $(BUILD_DIR)/double_pendule_video: | $(PICTURE_DIR)

$(BUILD_DIR)/simple_pendule: simple_pendule.c | $(BUILD_DIR)
	$(CC) simple_pendule.c -o $(BUILD_DIR)/simple_pendule $(FLAGS) $(RAY_FLAGS)

$(BUILD_DIR)/double_pendule: double_pendule.c | $(BUILD_DIR)
	$(CC) double_pendule.c resolveur_EDO.c -o $(BUILD_DIR)/double_pendule $(FLAGS) $(RAY_FLAGS)

$(BUILD_DIR)/double_pendule_console: double_pendule_console.c | $(BUILD_DIR)
	$(CC) double_pendule_console.c resolveur_EDO.c graphique.c -o $(BUILD_DIR)/double_pendule_console $(FLAGS) -lm

$(BUILD_DIR)/double_pendule_video: double_pendule_video.c | $(BUILD_DIR) $(PICTURE_DIR)
	$(CC) double_pendule_video.c resolveur_EDO.c graphique.c PPMfile.c -o $(BUILD_DIR)/double_pendule_video $(FLAGS) -lm

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

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean_all"
	@echo "... create_video"
	@echo "... play_video"
	@echo "... create_gif"
	@echo "... help"
