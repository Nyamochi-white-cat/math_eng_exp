# Run from nla/: gnuplot main.gp
# Columns: trial, ||A*x-b||_2, ||x_ref-x||_2 / ||x_ref||_2, time [us].
reset
set terminal pngcairo enhanced size 1800,600 font "Helvetica,14"
system "mkdir -p figure"
sizes = "100 200 400 800"
datafile(method, n) = sprintf("data/assignment%d_%d.dat", method, n)
method_name(method) = method == 1 ? "Gaussian elimination" : "LU decomposition"
positive(x) = x > 0 ? x : 1/0  # Zero cannot be displayed on a log axis.
set style line 1 lc rgb "#0072B2" pt 7 lw 2
set style line 2 lc rgb "#D55E00" pt 5 lw 2
set style line 3 lc rgb "#009E73" pt 9 lw 2
set style line 4 lc rgb "#CC79A7" pt 11 lw 2
set grid xtics ytics back
set key top left
set logscale y 10
set format y "10^{%L}"
set offsets graph 0.05, graph 0.05, graph 0.1, graph 0.1

# Inspect all 100 trials. The same color identifies n in each panel.
do for [method=1:2] {
    set output sprintf("figure/assignment%d_trials.png", method)
    set multiplot layout 1,3 title method_name(method) \
        margins 0.06,0.99,0.14,0.86 spacing 0.08,0.0
    set xlabel "Trial"
    set xrange [-1:100]
    set title "Residual norm"
    set ylabel "||A x - b||_2"
    plot for [k=1:4] datafile(method, int(word(sizes,k))) skip 1 \
        using 1:(positive($2)) with points ls k ps 0.5 \
        title sprintf("n = %s", word(sizes,k))
    set title "Relative error against QR reference"
    set ylabel "||x_{ref} - x||_2 / ||x_{ref}||_2"
    plot for [k=1:4] datafile(method, int(word(sizes,k))) skip 1 \
        using 1:(positive($3)) with points ls k ps 0.5 \
        title sprintf("n = %s", word(sizes,k))
    set title "Solve time"
    set ylabel "Time [ms]"
    plot for [k=1:4] datafile(method, int(word(sizes,k))) skip 1 \
        using 1:(positive($4/1000.0)) with points ls k ps 0.5 \
        title sprintf("n = %s", word(sizes,k))
    unset multiplot
    unset output
}

# Summaries preserve raw samples in the plots and use medians, since errors
# can span several orders of magnitude. Time ranges show min/max, not CI.
unset logscale y
set autoscale x
set autoscale y
set print $summary
do for [method=1:2] {
    do for [k=1:4] {
        n = int(word(sizes,k))
        stats datafile(method,n) skip 1 using 2 nooutput
        residual_median = STATS_median
        stats datafile(method,n) skip 1 using 3 nooutput
        error_median = STATS_median
        stats datafile(method,n) skip 1 using ($4/1000.0) nooutput
        print sprintf("%d %d %.12g %.12g %.12g %.12g %.12g", \
            method, n, residual_median, error_median, \
            STATS_median, STATS_min, STATS_max)
        if (method == 1 && k == 1) {
            cubic_anchor = STATS_median
        }
    }
}
set print
cubic(n) = cubic_anchor * (n/100.0)**3

set output "figure/assignments_comparison.png"
set multiplot layout 1,3 title "Dimension dependence: samples and medians" \
    margins 0.06,0.99,0.14,0.86 spacing 0.08,0.0
set logscale x 2
set logscale y 10
set xrange [80:1000]
set xtics ("100" 100, "200" 200, "400" 400, "800" 800)
set format x "%g"
set format y "10^{%L}"
set xlabel "Matrix dimension n (log scale)"
set key top left font ",10"
set title "Residual norm"
set ylabel "||A x - b||_2"
plot for [method=1:2] for [k=1:4] \
    datafile(method,int(word(sizes,k))) skip 1 \
    using (int(word(sizes,k))):(positive($2)) \
    with points ls method ps 0.3 notitle, \
    for [method=1:2] $summary using ($1==method ? $2 : 1/0):(positive($3)) \
    with linespoints ls method title sprintf("%s median",method_name(method))
set title "Relative error against QR reference"
set ylabel "||x_{ref} - x||_2 / ||x_{ref}||_2"
plot for [method=1:2] for [k=1:4] \
    datafile(method,int(word(sizes,k))) skip 1 \
    using (int(word(sizes,k))):(positive($3)) \
    with points ls method ps 0.3 notitle, \
    for [method=1:2] $summary using ($1==method ? $2 : 1/0):(positive($4)) \
    with linespoints ls method title sprintf("%s median",method_name(method))
set title "Solve time (bars: min/max)"
set ylabel "Time [ms]"
plot for [method=1:2] $summary \
    using ($1==method ? $2 : 1/0):(positive($5)):(positive($6)):(positive($7)) \
    with yerrorlines ls method title sprintf("%s median",method_name(method)), \
    cubic(x) with lines dt 2 lc rgb "#555555" \
    title "n^3 (anchored at elimination n=100)"
unset multiplot
unset output
