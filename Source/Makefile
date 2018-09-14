driver = main.c
clipsGen = clipsGen.h clipsGen.c
driverFiles = $(driver) $(clipsGen)
exe = graphColor graphGen
accFiles = color.clips coloring.dot
dir = Clips_Core

all:
	gcc graphGen.c -o graphGen;\
	cp $(driverFiles) $(dir); \
	cd $(dir); \
	gcc -g $(driverFiles) -L./ -lclips -lm -o graphColor; \
	mv graphColor ./..;

png:
	dot -Tps coloring.dot -o out.ps
	convert out.ps result.png

jpg:
	dot -Tjpg coloring.dot -o result.jpg

clean: 
	rm -rf $(exe) $(accFiles); \
	cd $(dir); \
	rm -rf $(driverFiles);

