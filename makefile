# Nota: A volte fallisce il linker poiche il vpath potrebbe non intervenire, ma rieseguendo make
#	 	funziona. Non so a cosa sia dovuto nello specifico e in questo contesto non importa. 
#		Potrebbe essere un bug o una mia distrazione dei vpath (in ogni caso mi prenderebbe troppo
#		tempo risolvere amesso fosse possibile). Quindi si prega se succede di rieseguire il make
#		finche il vpath smette di fare capricci.
# Nota: Dopo un ricerca sul web il problema sembrerebbe che il vpath non sia stato progettato per i 
#		target ma se fallisce almeno una volta allora lo usi effetivamente. Notare che la faccenda è
#		piu ambigua di cosi perche il vpath non fallisce mai (da quello che ho potuto osservare) di 
#		risolvere i target %.o generati dal comando gcc -MM percio presumo che sia un problema di 
#		run-time (come se andasse forzato a farlo) o un errore implementativo del programma di make.
#		In generale make con il suo linguaggio è poco affidabile, ha molti comportamenti impliciti
#		relativamente "poco" documentati per un "semplice" strumento di compilazione (molto frustrante).
# Post: https://stackoverflow.com/questions/10435173/in-a-makefile-how-vpath-works
# Post: https://make.mad-scientist.net/papers/how-not-to-use-vpath/		

# Docs: 		https://www.gnu.org/software/make/manual/make.html#General-Search
# Good article: https://makefiletutorial.com/

# Nota: Tutti i commenti che sono presenti non sono rivolti chiaramente 
#       al professore. Vengono utilizzati per poter fissare meglio concetti.
#       In particolar modo i commenti che sono presenti su "strumenti/soluzioni/..."
#       che non ho mai utilizzato come i make/makefile. Spero che non siano troppo
#       fastidiosi, in un progetto reale non overcommenterei ma produrrei appunti :).

# Compilation variables
CC 		    := gcc
CFLAGS      := -Wvla -Wextra -Werror -D_GNU_SOURCE -I ./include
SRC_DIR     := src
SRC_EXEC 	:= threads
BUILD_DIR   := build
START 		:= $(BUILD_DIR)/exe/master
ENVFILEPATH := config/timeout.env.mk

# Load .env file 
# Nota: In realta i file .env che vengono utilizzati non rispettano
#		la specifica nei .env perche includono spazi e commenti percio
#		utilizzo i makefile come dei .env tanto la sintassi è uguale,
# 		si potrebbe dire che i make sono un superset degli env.
# 		Come ulteriore vantaggio questa soluzioni non contamina gli 
#		env del SO ma le copia nel processo, i figli faranno ridondaza 
#		ma è un problema dei processi.
include $(ENVFILEPATH)
export 	$(ENVFILEPATH)

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c') 			  # All sources file on SRC_DIR
SRC_DIRS  := $(shell find $(SRC_DIR) -type d)				  # All directory on SRC_DIR
NAMES     := master alimentazione attivatore atomo inibitore  # Threads names (.o --> exe) 

# Liste di target per all
EXE_TARGETS := $(addprefix $(BUILD_DIR)/exe/, $(NAMES))   
OBJ_TARGETS := $(patsubst %.c,%.o, 									\
				$(filter-out 										\
					$(notdir $(wildcard $(SRC_DIR)/threads/*.c)), 	\
					$(notdir $(shell find $(SRC_DIR) -name '*.c'))	\
				)													\
			)

# Specifica dove cercare se non trova nella work dir
vpath %.o $(BUILD_DIR)/objects 			# (Se si commenta ricompila sempre poiche li considera non trovati)
vpath %.c $(SRC_DIRS)					# ... 

.PHONY: all clean init obj-dep run

all: init obj-dep $(OBJ_TARGETS) $(EXE_TARGETS) run				

e: 
	@echo $(BUILD_DIR)/objects

# Regola generica per la compilazione degli oggetti
%.o: %.c 
	@$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/objects/$@

# Inclusione delle dipendeze generete
# Nota: Per un progetto giocattolo come questo forse è esagerato ma in ogni caso 
#		è proprio comodo. L'utilizzo di -MM e non di M è stato deciso per rendere 
#		piu leggibile deps.d ma -M è chiaramente meglio.
-include $(BUILD_DIR)/deps.d

# Compilazione di tutti gli exec (file dotati di main)
OBJ_TARGETS_PREFIX = $(addprefix $(BUILD_DIR)/objects/, $(OBJ_TARGETS))
$(BUILD_DIR)/exe/%: %.o $(OBJ_TARGETS_PREFIX)				
	@$(CC) $(CFLAGS) $(OBJ_TARGETS_PREFIX) $< -lm -o $@ 

init:
	@mkdir -p build build/exe build/objects

obj-dep:
	@$(CC) -I ./include -MM $(SRC_FILES) > $(BUILD_DIR)/deps.d

clean:
	@rm -r $(BUILD_DIR)/*

run:
	@./$(START)