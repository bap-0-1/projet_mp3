# Makefile

# Spécification des fichiers source
SIM_SOURCES = audisen_sim.c ams.c amp.c frame.c
USB_SOURCES = amp.c ams.c frame.c audisen_sim.c usb.c
LIBRARY = ftd2xx.lib

# Spécification des options de compilation
CFLAGS = -g
LDFLAGS =

# Cible par défaut
all: sim

# Compilation pour la cible sim
sim: $(SIM_SOURCES)
	gcc $(SIM_SOURCES) $(CFLAGS) $(LDFLAGS) -o main

# Compilation pour la cible usb
usb: $(USB_SOURCES)
	gcc $(USB_SOURCES) $(LIBRARY) $(CFLAGS) $(LDFLAGS) -o main

# Nettoyage
clean:
	rm -rf *.o main

