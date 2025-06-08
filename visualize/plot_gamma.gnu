set term qt font ",12"
set grid

set multiplot layout 3,1

    set ylabel "Gamma"
    unset logscale y
    unset xlabel
    plot "visualize/gamma.txt" using 1:2 with lines linewidth 2 title "n2", \
         "visualize/gamma.txt" using 1:3 with lines linewidth 2 title "o2", \
         "visualize/gamma.txt" using 1:4 with lines linewidth 2 title "ar", \
         "visualize/gamma.txt" using 1:5 with lines linewidth 2 title "c8h18", \
         "visualize/gamma.txt" using 1:6 with lines linewidth 2 title "co2", \
         "visualize/gamma.txt" using 1:7 with lines linewidth 2 title "h2o"

    set ylabel "Cp J/(mol·K)"
    set logscale y
    unset xlabel
    plot "visualize/gamma.txt" using 1:8 with lines linewidth 2 title "n2", \
         "visualize/gamma.txt" using 1:9 with lines linewidth 2 title "o2", \
         "visualize/gamma.txt" using 1:10 with lines linewidth 2 title "ar", \
         "visualize/gamma.txt" using 1:11 with lines linewidth 2 title "c8h18", \
         "visualize/gamma.txt" using 1:12 with lines linewidth 2 title "co2", \
         "visualize/gamma.txt" using 1:13 with lines linewidth 2 title "h2o"

    set ylabel "Cv J/(mol·K)"
    set logscale y
    set xlabel "Static Temperature (K)"
    plot "visualize/gamma.txt" using 1:14 with lines linewidth 2 title "n2", \
         "visualize/gamma.txt" using 1:15 with lines linewidth 2 title "o2", \
         "visualize/gamma.txt" using 1:16 with lines linewidth 2 title "ar", \
         "visualize/gamma.txt" using 1:17 with lines linewidth 2 title "c8h18", \
         "visualize/gamma.txt" using 1:18 with lines linewidth 2 title "co2", \
         "visualize/gamma.txt" using 1:19 with lines linewidth 2 title "h2o"

unset multiplot
replot
pause mouse close
