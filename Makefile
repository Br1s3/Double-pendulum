MD = mkdir
RM = rm
CC = cc

EXE_DIR 	:= project
PICTURE_DIR 	:= stock

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

double_pendule_video: double_pendule_video.c $(EXE_DIR)
	$(CC) double_pendule_video.c resolveur_EDO.c graphique.c PPMfile.c -o $(EXE_DIR)/double_pendule_video $(FLAGS) -lm

$(PICTURE_DIR):
	$(MD) -p $(PICTURE_DIR)

create_picture: $(EXE_DIR) $(PICTURE_DIR)
	./$(EXE_DIR)/double_pendule_video

create_video:
	ffmpeg -i $(PICTURE_DIR)/Double_pendule_%03d.ppm -r 60 $(PICTURE_DIR)/Double_pendule.mp4

play_video:
	mpv $(PICTURE_DIR)/Double_pendule.mp4 --loop-file=yes

clean_all:
	$(RM) -rf $(PICTURE_DIR)
	$(RM) -rf $(EXE_DIR)
