

NAME_MODULE	= fanning_spi

CXXFLAGS	+= -I./
CFLAGS	+= -I./

OBJ_DIR		= $(NAME_MODULE)_build

VPATH += ./

OBJ += $(OBJ_DIR)/PJ_RPI.o
OBJ += $(OBJ_DIR)/main.o

#CXX		= g++
#CC		= gcc

CXX		= arm-linux-gnueabihf-g++-4.9
CC		= arm-linux-gnueabihf-gcc-4.9

all: create $(OBJ_DIR)/$(NAME_MODULE)

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@echo CXX $<
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CC) -o $@ $^ $(CXXFLAGS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

copy:
	sshpass -p "mttnttpkl" scp -r * root@192.168.1.7:/home/workspace/spi

log:
	sshpass -p "Xk4S/n" ssh device@192.168.168.138

clean:
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

flash:
	@sudo $(OBJ_DIR)/$(NAME_MODULE)

flash2 :
	${CC} ${LDFLAGS} -c -Wall -Werror -fpic PJ_RPI.c
	${CC} ${LDFLAGS} -shared -o libfanning.so PJ_RPI.o
	${CC} ${LDFLAGS} -c main.c
	${CC} ${LDFLAGS} -o program -L. main.o -lfanning
	${CC} ${LDFLAGS} -o test test.c
