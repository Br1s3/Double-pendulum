MD = mkdir
RM = rm
CC = cc

EXE_DIR 	:= project
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


all: simple_pendule double_pendule double_pendule_console double_pendule_video

$(EXE_DIR):
	$(MD) -p $(EXE_DIR)

simple_pendule: simple_pendule.c $(EXE_DIR)
	$(CC) simple_pendule.c -o $(EXE_DIR)/simple_pendule $(FLAGS) $(RAY_FLAGS)

double_pendule: double_pendule.c $(EXE_DIR)
	$(CC) double_pendule.c resolveur_EDO.c -o $(EXE_DIR)/double_pendule $(FLAGS) $(RAY_FLAGS)

double_pendule_console: double_pendule_console.c $(EXE_DIR)
	$(CC) double_pendule_console.c resolveur_EDO.c graphique.c -o $(EXE_DIR)/double_pendule_console $(FLAGS) -lm

$(PICTURE_DIR):
	$(MD) -p $(PICTURE_DIR)

double_pendule_video: double_pendule_video.c $(EXE_DIR) $(PICTURE_DIR)
	$(CC) double_pendule_video.c resolveur_EDO.c graphique.c PPMfile.c -o $(EXE_DIR)/double_pendule_video $(FLAGS) -lm

create_picture: double_pendule_video
	./$(EXE_DIR)/double_pendule_video

stock/Double_pendule_%03d.ppm: double_pendule_video
	./$(EXE_DIR)/double_pendule_video

create_video: stock/Double_pendule_%03d.ppm
	ffmpeg -i stock/Double_pendule_%03d.ppm -r 60 stock/Double_pendule.mp4

stock/Double_pendule.mp4: create_video

play_video: stock/Double_pendule.mp4
	mpv stock/Double_pendule.mp4 --loop-file=yes

create_gif: $(PICTURE_DIR) stock/Double_pendule.mp4 
	ffmpeg -i stock/Double_pendule.mp4 -vf "fps=15,scale=500:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" stock/Double_pendule.gif

clean_all:
	$(RM) -rf $(PICTURE_DIR)
	$(RM) -rf $(EXE_DIR)

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean_all"
	@echo "... simple_pendule"
	@echo "... double_pendule"
	@echo "... double_pendule_video"
	@echo "... create_picture"
	@echo "... create_video"
	@echo "... play_video"
	@echo "... create_gif"
