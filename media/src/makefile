#makefile for processing svg files to png and packing them.

svgfiles=$(shell ls *.svg | grep -v icon.svg )
pngfiles=$(svgfiles:.svg=.png)

atlas.png atlas.gorilla :	imagelist.txt packatlas.lua $(pngfiles) $(svgfiles)
	./packatlas.lua

.PHONY: clean
clean:
	rm -f $(pngfiles)

%.png:	%.svg
	./convert_svg.sh $< $@
