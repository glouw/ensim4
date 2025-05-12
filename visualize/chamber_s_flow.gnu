set term qt font ",9"
set grid

set multiplot layout 7,2

    unset xlabel

    set ylabel "Velocity (m/s)"
    plot "visualize/chamber_s_flow.txt" using 1:2 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:3 with lines linewidth 2 title "Chamber y"

    set ylabel "Speed of Sound (m/s)"
    plot "visualize/chamber_s_flow.txt" using 1:4 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:5 with lines linewidth 2 title "Chamber y"

    set ylabel "Mach"
    plot "visualize/chamber_s_flow.txt" using 1:6 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:7 with lines linewidth 2 title "Chamber y"

    set ylabel "Max Momentum (kg·m/s)"
    plot "visualize/chamber_s_flow.txt" using 1:8 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:9 with lines linewidth 2 title "Chamber y"

    set ylabel "Momentum (kg·m/s)"
    plot "visualize/chamber_s_flow.txt" using 1:10 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:11 with lines linewidth 2 title "Chamber y"

    set ylabel "Static Pressure (Pa)"
    plot "visualize/chamber_s_flow.txt" using 1:12 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:13 with lines linewidth 2 title "Chamber y"

    set ylabel "Static Temperature (K)"
    plot "visualize/chamber_s_flow.txt" using 1:14 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:15 with lines linewidth 2 title "Chamber y"

    set ylabel "Total Pressure (Pa)"
    plot "visualize/chamber_s_flow.txt" using 1:16 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:17 with lines linewidth 2 title "Chamber y"

    set ylabel "Total Temperature (K)"
    plot "visualize/chamber_s_flow.txt" using 1:18 with lines linewidth 2 title "Chamber x", \
         "visualize/chamber_s_flow.txt" using 1:19 with lines linewidth 2 title "Chamber y"

    set xlabel "Time (seconds)"

    set ylabel "Mach number"
    plot "visualize/chamber_s_flow.txt" using 1:20 with lines linewidth 2 title "Nozzle"

    set ylabel "Mass flow kg/s"
    plot "visualize/chamber_s_flow.txt" using 1:21 with lines linewidth 2 title "Nozzle"

    set ylabel "Velocity m/s"
    plot "visualize/chamber_s_flow.txt" using 1:22 with lines linewidth 2 title "Nozzle"

    set ylabel "Speed of Sound m/s"
    plot "visualize/chamber_s_flow.txt" using 1:23 with lines linewidth 2 title "Nozzle"

unset multiplot
replot
pause mouse close
