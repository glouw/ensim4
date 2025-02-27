set term qt font ",12"
set grid
set key top right

set multiplot layout 8,1 title "Flow Properties Over Time"

    # 1
    set ylabel "Total Pressure (pa)"
    plot "plot/simple_flow.txt" using 1:2 with lines linewidth 2 title "chamber x", \
         "plot/simple_flow.txt" using 1:3 with lines linewidth 2 title "chamber y"

    # 2
    set ylabel "Temperature (k)"
    plot "plot/simple_flow.txt" using 1:4 with lines linewidth 2 title "total x", \
         "plot/simple_flow.txt" using 1:5 with lines linewidth 2 title "total y", \
         "plot/simple_flow.txt" using 1:6 with lines linewidth 2 title "static x", \
         "plot/simple_flow.txt" using 1:7 with lines linewidth 2 title "static y"

    # 3
    set ylabel "Mass (kg)"
    plot "plot/simple_flow.txt" using 1:8 with lines linewidth 2 title "chamber x", \
         "plot/simple_flow.txt" using 1:9 with lines linewidth 2 title "chamber y"

    # 4
    set ylabel "Momentum (kg * m/s)"
    plot "plot/simple_flow.txt" using 1:10 with lines linewidth 2 title "chamber x", \
         "plot/simple_flow.txt" using 1:11 with lines linewidth 2 title "chamber y"

    # 5
    set ylabel "Bulk Mach"
    plot "plot/simple_flow.txt" using 1:12 with lines linewidth 2 title "chamber x", \
         "plot/simple_flow.txt" using 1:13 with lines linewidth 2 title "chamber y"

    # 6
    set ylabel "Nozzle Mach"
    plot "plot/simple_flow.txt" using 1:14 with lines linewidth 2 title "nozzle"

    # 7
    set ylabel "Nozzle Velocity"
    plot "plot/simple_flow.txt" using 1:15 with lines linewidth 2 title "nozzle"

    # 8
    set ylabel "Nozzle Mass Flow"
    plot "plot/simple_flow.txt" using 1:16 with lines linewidth 2 title "nozzle"

unset multiplot

pause mouse close
