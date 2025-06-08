set term qt font ",9"
set grid

set multiplot layout 6,2

    unset xlabel

    set ylabel "Cp J/(mol·K)"
    plot "visualize/chamber_s_mix.txt" using 1:2 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:3 with lines linewidth 2 title "Chamber y"

    set ylabel "Cv J/(mol·K)"
    plot "visualize/chamber_s_mix.txt" using 1:4 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:5 with lines linewidth 2 title "Chamber y"

    set ylabel "Cp J/K"
    plot "visualize/chamber_s_mix.txt" using 1:6 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:7 with lines linewidth 2 title "Chamber y"

    set ylabel "Cv J/K"
    plot "visualize/chamber_s_mix.txt" using 1:8 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:9 with lines linewidth 2 title "Chamber y"

    set ylabel "Moles"
    plot "visualize/chamber_s_mix.txt" using 1:10 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:11 with lines linewidth 2 title "Chamber y"

    set ylabel "Molar Mass kg/mol"
    plot "visualize/chamber_s_mix.txt" using 1:12 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:13 with lines linewidth 2 title "Chamber y"

    set ylabel "Gamma"
    plot "visualize/chamber_s_mix.txt" using 1:14 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:15 with lines linewidth 2 title "Chamber y"

    set ylabel "Rs J/(kg·K)"
    plot "visualize/chamber_s_mix.txt" using 1:16 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_mix.txt" using 1:17 with lines linewidth 2 title "Chamber y"

    set ylabel "Mach number"
    plot "visualize/chamber_s_mix.txt" using 1:18 with lines linewidth 2 title "Nozzle"

    set xlabel "Time s"

    set ylabel "Mass flow kg/s"
    plot "visualize/chamber_s_mix.txt" using 1:19 with lines linewidth 2 title "Nozzle"

    set ylabel "Velocity m/s"
    plot "visualize/chamber_s_mix.txt" using 1:20 with lines linewidth 2 title "Nozzle"

    set ylabel "Speed of Sound m/s"
    plot "visualize/chamber_s_mix.txt" using 1:21 with lines linewidth 2 title "Nozzle"

unset multiplot
replot
pause mouse close
