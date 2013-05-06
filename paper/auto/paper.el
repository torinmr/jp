(TeX-add-style-hook "paper"
 (lambda ()
    (LaTeX-add-bibliographies
     "ref")
    (TeX-add-symbols
     '("tr" 1)
     '("note" 2)
     '("placeholder" 1)
     '("comment" 1)
     "AUTHORS"
     "TITLE"
     "KEYWORDS"
     "CONFERENCE"
     "PAGENUMBERS"
     "COLOR"
     "showComments"
     "onlyAbstract"
     "ToAppear"
     "HonorCode")
    (TeX-run-style-hooks
     "textpos"
     "absolute"
     "soul"
     "graphicx"
     "url"
     "hyperref"
     "colorlinks"
     "helvet"
     "courier"
     "mathptmx"
     "float"
     "colortbl"
     "color"
     "booktabs"
     "natbib"
     "numbers"
     "fancyhdr"
     "enumitem"
     "titlesec"
     "caption"
     "noheadfoot"
     "geometry"
     "nohead"
     "ifthen"
     "latex2e"
     "extarticle10"
     "extarticle"
     "pdftex"
     "twocolumn"
     "10pt"
     "letterpaper"
     "abstract"
     "intro"
     "design"
     "eval"
     "related"
     "concl")))

