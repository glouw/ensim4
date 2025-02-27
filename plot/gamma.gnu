set term qt font ",16"

set title "Variation of Specific Heat Ratio (gamma) with Static Temperature (K)"
set xlabel "Static Temperature (K)"
set ylabel "Specific Heat Ratio (gamma)"
set grid

plot "plot/gamma.txt" using 1:2 with lines linewidth 2 title "n2", \
     "plot/gamma.txt" using 1:3 with lines linewidth 2 title "o2", \
     "plot/gamma.txt" using 1:4 with lines linewidth 2 title "ar", \
     "plot/gamma.txt" using 1:5 with lines linewidth 2 title "c8h18", \
     "plot/gamma.txt" using 1:6 with lines linewidth 2 title "co2", \
     "plot/gamma.txt" using 1:7 with lines linewidth 2 title "h2o"

pause mouse close
