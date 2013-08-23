#makefile for processing svg files to png and packing them.

svgfiles=$(shell ls *.svg | grep -v icon.svg )
pngfiles=$(svgfiles:.svg=.png)

atlas.png atlas.gorilla :	$(shell cat imagelist.txt) packatlas.lua $(svgfiles) imagelist.txt
	./packatlas.lua

.PHONY: clean
clean:
	rm -f $(pngfiles)

%.png:	%.svg
	./convert_svg.sh $< $@
