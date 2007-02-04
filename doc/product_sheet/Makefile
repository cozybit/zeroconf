
# Directory where freefont fonts live (e.g. FreeSans.ttf,...)
CWD=$(shell pwd)
FREEFONTS = $(CWD)/art/freefont

all: all_figures all_pdf all_ps

tex_sources := $(wildcard *.tex) 
sub_docs := $(wildcard sub/*.tex)
figures := $(wildcard figures/*.svg)

all_pdf: $(patsubst %.tex,%.pdf,$(tex_sources)) $(figures)
all_ps: $(patsubst %.tex,%.ps,$(tex_sources)) $(figures)
all_txt: $(patsubst %.tex,%.txt,$(tex_sources))

all_figures: $(patsubst %.svg,%.eps,$(figures))

%.pdf: %.tex $(sub_docs)

%.ps: %.tex $(sub_docs)
	pdflatex $*.tex

%.eps: %.svg
	# Remove yFiles watermark (super long path d element)
	sed -e 's/<path d=.\{1000,\}>/<path d=""\/>/' $*.svg > tmp.svg
	inkscape -z -T -E $*.eps tmp.svg
	\rm -f tmp.svg

%.pdf: %.ps
	GS_FONTPATH=$(FREEFONTS) ps2pdf $*.ps $*.pdf

2on1: mesh-sdd.ps
	\rm -f mesh-sdd.2on1.ps
	pstops "4:0L@.7(20cm,0.4cm)+1L@.7(20cm,13.25cm),2R@.7(1cm,28.25cm)+3R@.7(1cm,15.25cm)" mesh-sdd.ps > mesh-sdd.2on1.ps

milestones.tab: $(tex_sources)
	../scripts/summary.py -m $^ $@

extdeps.tab: $(tex_sources)
	../scripts/summary.py -x $^ $@

deliverables.tab: $(tex_sources)
	../scripts/summary.py -d $^ $@

%.txt: %.tex
	latex $*.tex
	latex $*.tex
	dvi2tty -w80 -o $*.txt $*.dvi

clean:
	rm -f *.aux *.dvi *.log *.ps *.pdf *.toc *.out *.tab
	rm -f figures/*.eps

estimates:
	../scripts/summary.py -s $(tex_sources)

.PHONY: estimates
