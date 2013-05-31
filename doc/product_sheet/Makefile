
# Directory where freefont fonts live (e.g. FreeSans.ttf,...)
CWD=$(shell pwd)
FREEFONTS = $(CWD)/art/freefont

all: all_figures all_pdf

tex_sources := $(wildcard *.tex) 
sub_docs := $(wildcard sub/*.tex)
figures := $(wildcard figures/*.svg)

all_pdf: $(patsubst %.tex,%.pdf,$(tex_sources)) $(figures)
all_ps: $(patsubst %.tex,%.ps,$(tex_sources)) $(figures)
all_txt: $(patsubst %.tex,%.txt,$(tex_sources))

all_figures: $(patsubst %.svg,%.png,$(figures))

%.pdf: %.tex $(sub_docs)
	pdflatex $*.tex

%.png: %.svg
	inkscape -z -T -D -d 300 -e $*.png $^

clean:
	rm -f *.aux *.dvi *.log *.ps *.pdf *.toc *.out *.tab
	rm -f figures/*.png
